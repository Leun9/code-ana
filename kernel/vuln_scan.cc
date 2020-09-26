#include "vuln_scan.h"

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <stack>
#include <unordered_map>
#include <iostream>
#include <QDebug>

#include "util/trie.h"
#include "util/type.h"
#include "func_scanner.h"

using std::string;
using std::vector;
using std::pair;
using std::list;
using std::stack;
using std::unordered_map;
using codeana::kernel::util::Trie;
using codeana::kernel::util::ISIDCHAR;
using codeana::kernel::util::ISBLANK;

namespace codeana {
namespace kernel {

static Trie buf_vuln_trie(VULN_FUNC_LIST);

#define PUSHARG(x, y, z) \
do { \
    args.push_back(x); \
    nums.push_back(y); \
    types.push_back(z); \
} while(0);

void GetArg(string const &str, size_t &i, vector<string> &args, vector<int> &types, vector<size_t> &nums) {
    args.clear();
    types.clear();
    nums.clear();
    while (str[i] != ')') {
        do {++i;} while (ISBLANK(str[i]));
        size_t start = i;
        size_t end;
        if (str[i] == '\"') {
            size_t len = 0;
            do {++i; ++len; if (str[i] == '\\') ++i;} while (str[i] != '\"');
            // qDebug() << i;
            PUSHARG(str.substr(start, i+1-start), len, KSTR);
            while (str[i] != ',' && str[i] != ')') ++i;
            continue;
        } else if (str[i] == 'L' && str[i+1] == '"') {
            ++i;
            size_t len = 0;
            do {++i; ++len; if (str[i] == '\\') ++i;} while (str[i] != '\"');
            // qDebug() << i;
            PUSHARG(str.substr(start, i+1-start), len*2, KSTR);
            while (str[i] != ',' && str[i] != ')') ++i;
            continue;
        } else if (str[i] >= '0' && str[i] <= '9'){
            while (str[i] >= '0' && str[i] <= '9') ++i;
            end = i;
            while (ISBLANK(str[i])) ++i;
            if (str[i] == ',' || str[i] == ')') {
                size_t x = 0;
                for (size_t i = start; i < end; ++i) {x *= 10; x += str[i] - '0';}
                PUSHARG(str.substr(start, end-start), x, KNUM);
                continue;
            }
        } else if (ISIDCHAR(str[i])) { // FIXME 类型强制
            while (ISIDCHAR(str[i])) ++i;
            end = i;
            while (ISBLANK(str[i])) ++i;
            if (str[i] == ',' || str[i] == ')') {
                PUSHARG(str.substr(start, end-start), 0, VALUE); // FIXME
                continue;
            }
        }
        while (str[i] != ',' && str[i] != ')') ++i;
        if (start == i) continue;
        //qDebug() << "EXP" << str.substr(start, i-start).c_str();
        PUSHARG(str.substr(start, i-start), 0, EXP);
    }
}

#define PUSHVULN(x, y, z, t) \
do { \
    pos.push_back(x); \
    func_type.push_back(now->leaf_num_); \
    info.push_back(y); \
    errlevel.push_back(z); \
    errtype.push_back(t); \
} while(0);

/***
 *
 * %[flags][width][.precision][length]specifier
 * flags : - or + or BLANK or # or 0
 * width : number or *
 * precision : number or *
 * length : h or L or l
 *
 ***/

void GetPrintFormatArg(string const &str, vector<string> &args, vector<int> &types, vector<size_t> &nums) {
    args.clear();
    types.clear();
    nums.clear();
    auto str_size = str.size();
    for (size_t i = 0; i < str_size; ) {
        if (str[i] != '%') {i++; continue;}
        if (str[i+1] == '%') {i+=2; continue;}

    }
}

void BufVulnScan(vector<int> &pos, vector<int> &func_type, vector<string> &info, vector<int> &errlevel,
                 vector<int> &errtype, string &str, size_t func_start, size_t func_end, ValueInfos &value_infos) {
  pos.clear();
  func_type.clear();
  info.clear();
  errlevel.clear();
  errtype.clear();

  unordered_map<string, stack<ValueInfo*>> value2info;
  auto vinfo_it = value_infos.begin();

  int deep = 0;
  for (size_t i = func_start; i < func_end;) {
    while (!ISIDCHAR(str[i]) && i < func_end) {
        //qDebug() << vinfo_it->name_.c_str() << vinfo_it->start_;
        while (vinfo_it != value_infos.end() && i >= vinfo_it->start_) {
            //qDebug() << vinfo_it->name_.c_str();
            value2info[vinfo_it->name_].push(&*vinfo_it);
            ++vinfo_it;
        }

        if (str[i] == '/') {
            if (str[i-1] == '/')
                while (str[i] != '\n') ++i;

        } else if (str[i] == '*') {
            if (str[i-1] == '/')
                do {++i;} while (str[i] != '/' || str[i-1] != '*');

        } else if (str[i] == '\"') {
            do {++i;} while (str[i] != '\"' && str[i-1] != '\\');

        } else if (str[i] == '{') {
            ++deep;

        } else if (str[i] == '}') { // 更新valuemap
            --deep;
            for (auto it = vinfo_it; ; --it) {
                if (it->deep_ == deep+1) value2info[it->name_].pop();
                if (it->deep_ < deep) break;
                if (it == value_infos.begin()) break;
            }
        }
        ++i;
    }

    size_t start = i;
    Trie::TrieNode* now = buf_vuln_trie.root_;
    while (buf_vuln_trie.Jump(now, str[i])) ++i;

    if (now->is_leaf_ && !ISIDCHAR(str[i])) {
        vector<string> args;
        vector<int> types;
        vector<size_t> nums;
        GetArg(str, i, args, types, nums);
        //qDebug() << str.substr(start, i-start).c_str();

        if (now->leaf_num_  == STRCPY || now->leaf_num_ == WCSCPY) {
            auto vinfo = value2info[args[0]].top(); // FIXME
            if (vinfo->len_ && types[0] == VALUE && types[1] == KSTR) {
                if (vinfo->len_ * vinfo->size_ >= nums[1]) {
                    PUSHVULN(start, "", LOW, vinfo->pos_);
                } else {
                    PUSHVULN(start, "拷贝的常量字符串长度大于可用长度", HIGH, vinfo->pos_);
                }
        } else {
            PUSHVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
        }

        } else if (now->leaf_num_ == STRNCPY || now->leaf_num_ == MEMCPY || now->leaf_num_ == MEMSET) {
            auto vinfo = value2info[args[0]].top(); // FIXME
              if (vinfo->len_ && types[0] == VALUE && types[2] == KNUM) {
                  //qDebug() << vinfo->name_.c_str() << vinfo->len_ * vinfo->size_ << nums[2];
                  if (vinfo->len_ * vinfo->size_ >= nums[2]) {
                      PUSHVULN(start, "", LOW, vinfo->pos_);
                  } else {
                      PUSHVULN(start, "指定的拷贝长度大于可用长度", HIGH, vinfo->pos_);
                  }
              } else {
                  PUSHVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
              }

        }  else if (now->leaf_num_ == WCSNCPY) {
            auto vinfo = value2info[args[0]].top(); // FIXME
              if (vinfo->len_ && types[0] == VALUE && types[2] == KNUM) {
                  if (vinfo->len_ * vinfo->size_ >= nums[2]*2) {
                      PUSHVULN(start, "", LOW, vinfo->pos_);
                  } else {
                      PUSHVULN(start, "指定的拷贝长度大于可用长度", HIGH, vinfo->pos_);
                  }
              } else {
                  PUSHVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
              }

        } else if ((now->leaf_num_ >= STRCAT && now->leaf_num_ <= GETS)) {
                //qDebug() << now->leaf_num_;
                PUSHVULN(start, "可能存在漏洞", MIDDLE, BUFOF);
                //qDebug() << func_type[func_type.size() - 1];

        } else if (now->leaf_num_ == FREAD) {
            auto vinfo = value2info[args[0]].top(); // FIXME
            if (vinfo->len_ && types[0] == VALUE && types[1] == KNUM && types[2] == KNUM) {
                if (vinfo->len_ * vinfo->size_ >= nums[1] * nums[2]) {
                    PUSHVULN(start, "", LOW, vinfo->pos_);
                } else {
                    PUSHVULN(start, "指定的读入长度大于可用长度", HIGH, vinfo->pos_);
                }
            } else {
                PUSHVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
            }

        } else if (now->leaf_num_ == MALLOC) { // FIXME
            size_t end = i;
            while (str[end] != '=') --end;
            do {--end;} while (ISBLANK(str[end]));
            size_t start = end;
            do {--start;} while (!ISIDCHAR(str[end]));
            auto vinfo = value2info[str.substr(start+1, end-start)].top(); // FIXME
            vinfo->pos_ = HEAP;
            if (types[0] == KNUM) vinfo->len_ = nums[0] / vinfo->size_;

        } else if (now->leaf_num_ == CALLOC) { // FIXME
            size_t end = i;
            while (str[end] != '=') --end;
            do {--end;} while (ISBLANK(str[end]));
            size_t start = end;
            do {--start;} while (!ISIDCHAR(str[end]));
            auto vinfo = value2info[str.substr(start+1, end-start)].top(); // FIXME
            vinfo->pos_ = HEAP;
            if (types[0] == KNUM && types[1] == KNUM)
                vinfo->len_ = nums[0] * nums[1] / vinfo->size_;

        }  else if (now->leaf_num_ == REALLOC) { // FIXME
            auto vinfo = value2info[args[0]].top();
            vinfo->pos_ = HEAP;
            if (types[1] == KNUM) vinfo->len_ = nums[1] / vinfo->size_;

        } else if (now->leaf_num_ == FREE) { // FIXME
            auto vinfo = value2info[args[0]].top();
            vinfo->len_ = 0;

        } else {
            //PUSHVULN(start, "", MIDDLE, BUFOF);
        }

    } else {
      while (ISIDCHAR(str[i])) ++i;
    }

  }
}

} // namespace kernel
} // namespace codeana
