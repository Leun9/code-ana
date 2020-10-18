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
using codeana::kernel::util::ISIDCHAR;
using codeana::kernel::util::ISBLANK;
using codeana::kernel::util::ISIDBEGIN;

namespace codeana {
namespace kernel {


#define PUSHARG(x, y, z) \
do { \
    args.push_back(x); \
    nums.push_back(y); \
    types.push_back(z); \
} while(0);

/*** 函数参数解析 ***/
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
            ////qDebug() << i;
            PUSHARG(str.substr(start, i+1-start), len, KSTR);
            while (str[i] != ',' && str[i] != ')') ++i;
            continue;
        } else if (str[i] == 'L' && str[i+1] == '"') {
            ++i;
            size_t len = 0;
            do {++i; ++len; if (str[i] == '\\') ++i;} while (str[i] != '\"');
            ////qDebug() << i;
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
    func_type.push_back(0); \
    info.push_back(y); \
    errlevel.push_back(z); \
    errtype.push_back(t); \
} while(0);

#define PUSHFUNCVULN(x, y, z, t) \
do { \
    pos.push_back(x); \
    func_type.push_back(now->leaf_num_); \
    info.push_back(y); \
    errlevel.push_back(z); \
    errtype.push_back(t); \
} while(0);

/*** 格式化字符串参数解析 ***/
/***
 *
 * %[flags][width][.precision][length]specifier
 * flags : - or + or BLANK or # or 0
 * width : number or *
 * precision : number or *
 * length : h or L or l
 *
 ***/

void GetPrintFormatArg(string const &str, vector<int> &types) {
    types.clear();
    auto str_size = str.size();
    for (size_t i = 0; i < str_size; ) {
        // int flag = 0;  不处理flag
        // int width = 0; 不处理宽度
        // int precision = 0; 不处理精度
        if (str[i] != '%') {i++; continue;}
        if (str[i+1] == '%') {i+=2; continue;}
        while (str[i] < 'a'|| str[i] > 'z') {
            if (str[i] == '*') types.push_back(FMT_x);
            ++i;
        }
        auto now = format_type_trie.root_;
        while (format_type_trie.Jump(now, str[i])) ++i;
        if (now->is_leaf_) types.push_back(now->leaf_num_);
    }
}

#define CHECK_PRINT_FMT(MAGIC) \
    if (types[MAGIC] == KSTR) { \
        vector<int> fmt_types; \
        GetPrintFormatArg(args[MAGIC], fmt_types); \
        if (args.size()-1-MAGIC != fmt_types.size()) { \
            PUSHFUNCVULN(start, "格式化字符串参数个数不匹配", HIGH, FORMATSTR); \
        } else { \
            for (size_t i = 0; i < fmt_types.size(); ++i) { \
                if (types[i+1+MAGIC] != VALUE) {PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, FORMATSTR); continue;} \
                auto vinfo = value2info[args[i+1+MAGIC]].top(); \
                /* FMT_p : is_pointer; FMT_s : char && is_pointer */ \
                if (vinfo->is_pointer_ || vinfo->is_array_) { \
                    if (fmt_types[i] == FMT_p) continue; \
                    if (fmt_types[i] == FMT_s && vinfo->type_ == CHAR) continue; \
                    PUSHFUNCVULN(start, "格式化字符串参数类型不匹配", HIGH, FORMATSTR); \
                } \
                int key = CAT8(fmt_types[i], CAT4(vinfo->unsigned_, vinfo->type_)); \
                if (format_type_set.find(key) == format_type_set.end()){ \
                    PUSHFUNCVULN(start, "格式化字符串参数类型不匹配", HIGH, FORMATSTR); \
                } \
            } \
        } \
    } else { \
        PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, FORMATSTR); \
    } \

/***
 * FIXME:
 * 1. 不同的格式化字符串格式
 * 2. 作简化处理的参数匹配
 ***/
#define CHECK_SCANF_FMT(MAGIC) \
    if (types[MAGIC] == KSTR) { \
        vector<int> fmt_types; \
        GetPrintFormatArg(args[MAGIC], fmt_types); \
        if (args.size()-1-MAGIC != fmt_types.size()) { \
            PUSHFUNCVULN(start, "格式化字符串参数个数不匹配", HIGH, FORMATSTR); \
        } else { \
            for (size_t i = 0; i < fmt_types.size(); ++i) { \
                string vi_str = args[i+1+MAGIC].substr(1); \
                auto it = value2info.find(vi_str); \
                if (it == value2info.end()) {PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, FORMATSTR); continue;} \
                auto vinfo = it->second.top(); \
                /* FMT_p : is_pointer; FMT_s : char && is_pointer */ \
                if (vinfo->is_pointer_ || vinfo->is_array_) { \
                    if (fmt_types[i] == FMT_p) continue; \
                    if (fmt_types[i] == FMT_s && vinfo->type_ == CHAR) continue; \
                    PUSHFUNCVULN(start, "格式化字符串参数类型不匹配", HIGH, FORMATSTR); \
                } \
                int key = CAT8(fmt_types[i], CAT4(vinfo->unsigned_, vinfo->type_)); \
                if (format_type_set.find(key) == format_type_set.end()){ \
                    PUSHFUNCVULN(start, "格式化字符串参数类型不匹配", HIGH, FORMATSTR); \
                } \
            } \
        } \
    } else { \
        PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, FORMATSTR); \
    } \

#define CHECK_DANG_VALUE(index) \
{ \
    string str_tmp = args[index]; \
    int j = 0; \
    while (true) { \
        while (j < str_tmp.size() && !ISIDBEGIN(str_tmp[j])) ++j; \
        if (j == str_tmp.size()) break; \
        size_t start_tmp = j; \
        while (j < str_tmp.size() && ISIDCHAR(str_tmp[j])) ++j; \
        auto vit = value2info.find(str_tmp.substr(start_tmp, j-start_tmp)); \
        if (vit != value2info.end()) { \
            auto vinfo = vit->second.top(); \
            if (vinfo->unsigned_ != true) \
                PUSHFUNCVULN(start, "敏感函数参数的隐式转换可能导致符号溢出漏洞:"+vinfo->name_, MIDDLE, SIGNOF); \
            if (dang_value.count(&*vinfo) != 0) \
                PUSHFUNCVULN(start, "敏感函数调用可能发生整数溢出的变量:"+vinfo->name_, MIDDLE, BUFOF); \
        } \
    } \
}

void BufVulnScan(vector<int> &pos, vector<int> &func_type, vector<string> &info, vector<int> &errlevel,
                 vector<int> &errtype, string &str, size_t func_start, size_t func_end, ValueInfos &value_infos) {
  pos.clear();
  func_type.clear();
  info.clear();
  errlevel.clear();
  errtype.clear();

  unordered_map<string, stack<ValueInfo*>> value2info;
  unordered_set<ValueInfo*> dang_value;
  auto vinfo_it = value_infos.begin();

  int deep = 0;
  for (size_t i = func_start; i < func_end;) {

   //qDebug() << "[INFO0]" << i << str[i];
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
            if (str[i-1] == '/') {
                do {++i;} while (str[i] != '/' || str[i-1] != '*'); // 注释
            }

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
        } else if (str[i] == '=') { // 宽度溢出：赋值符号右边有宽度大于左边的变量
            size_t j = i;
            while (!ISIDCHAR(str[j])) --j;
            size_t start = j;
            while (ISIDCHAR(str[start])) --start;
            auto lvit = value2info.find(str.substr(start+1, j-start));
            if (lvit == value2info.end()) {++i; continue;}
            auto lvinfo = lvit->second.top(); // FIXME

            if (str[i-1] != '>' && str[i-1] != '<' && str[i+1] != '=') {
                while (true) {
                    while (!ISIDBEGIN(str[i]) && str[i] != ',' && str[i] != ';') {
                        if (str[i] == '*' && str[i+1] == ' ') {
                            PUSHVULN(i, "可能存在运算溢出（乘法）漏洞", LOW, WIDTHOF);
                            dang_value.insert(&*lvinfo);
                        }
                        ++i;
                    }
                    if (str[i] == ',' || str[i] == ';') break;
                    size_t start = i;
                    while (ISIDCHAR(str[i])) ++i;
                    if (str[i] == '(') { //FIXME
                        do {++i;} while (str[i] != ')');
                        continue;
                    }
                    auto vit = value2info.find(str.substr(start, i-start));
                    if (vit != value2info.end()) {
                        auto vinfo = vit->second.top();
                        if ((size_t)vinfo->width_ > type2size[lvinfo->type_]) {
                            PUSHVULN(start, "可能存在宽度溢出漏洞", LOW, WIDTHOF);
                            dang_value.insert(&*lvinfo);
                        }
                        if (vinfo->unsigned_ != lvinfo->unsigned_) {
                            PUSHVULN(start, "可能存在符号溢出漏洞", LOW, SIGNOF);
                            dang_value.insert(&*lvinfo);
                        }
                    }
                }
            }

        }
        ++i;
    }

   //qDebug() << "[INFO1]"  << i << str[i];
    size_t start = i;
    Trie::TrieNode* now = buf_vuln_trie.root_;
    while (buf_vuln_trie.Jump(now, str[i])) ++i;
    //qDebug() << "[INFO2]"  << i << str.substr(i,10).c_str() << now << now->is_leaf_;

    if (now->is_leaf_ && !ISIDCHAR(str[i])) {
        vector<string> args;
        vector<int> types;
        vector<size_t> nums;
        GetArg(str, i, args, types, nums);
        //qDebug() << str.substr(start, i-start).c_str() << now->leaf_num_;

        switch (now->leaf_num_) {
        case STRCPY:
        case WCSCPY: {
            //auto vinfo = value2info[args[0]].top();
            auto it = value2info.find(args[0]);
            ValueInfo *vinfo = NULL;
            if (it != value2info.end() && !it->second.empty()) vinfo = it->second.top();
            if (vinfo && vinfo->len_ && types[0] == VALUE && types[1] == KSTR) {
                if (vinfo->len_ * vinfo->size_ >= nums[1]) {
                    PUSHFUNCVULN(start, "", LOW, vinfo->pos_);
                } else {
                    PUSHFUNCVULN(start, "拷贝的常量字符串长度大于可用长度", HIGH, vinfo->pos_);
                }
            } else {
                PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
            }

        } break;
        case STRNCPY:
        case MEMCPY:
        case MEMSET: {
            CHECK_DANG_VALUE(2);
            //auto vinfo = value2info[args[0]].top();
            auto it = value2info.find(args[0]);
            ValueInfo *vinfo = NULL;
            if (it != value2info.end() && !it->second.empty()) vinfo = it->second.top();
            if (vinfo && vinfo->len_ && types[0] == VALUE && types[2] == KNUM) {
              //qDebug() << vinfo->name_.c_str() << vinfo->lelvinfo =USn_ * vinfo->size_ << nums[2];
              if (vinfo->len_ * vinfo->size_ >= nums[2]) {
                  PUSHFUNCVULN(start, "", LOW, vinfo->pos_);
              } else {
                  PUSHFUNCVULN(start, "指定的拷贝长度大于可用长度", HIGH, vinfo->pos_);
              }
            } else {
              PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
            }
        } break;
        case WCSNCPY: {
            CHECK_DANG_VALUE(2);
            //auto vinfo = value2info[args[0]].top();
            auto it = value2info.find(args[0]);
            ValueInfo *vinfo = NULL;
            if (it != value2info.end() && !it->second.empty()) vinfo = it->second.top();
            if (vinfo && vinfo->len_ && types[0] == VALUE && types[2] == KNUM) {
              if (vinfo->len_ * vinfo->size_ >= nums[2]*2) {
                  PUSHFUNCVULN(start, "", LOW, vinfo->pos_);
              } else {
                  PUSHFUNCVULN(start, "指定的拷贝长度大于可用长度", HIGH, vinfo->pos_);
              }
            } else {
              PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
            }

        } break;
        case STRNCAT:
        case WCSNCAT:
            CHECK_DANG_VALUE(2);
        case STRCAT:
        case WCSCAT:
        case GETS: {
            //qDebug() << now->leaf_num_;
            PUSHFUNCVULN(start, "可能存在漏洞", MIDDLE, BUFOF);
            //qDebug() << func_type[func_type.size() - 1];

        } break;
        case FREAD: {
            CHECK_DANG_VALUE(1);
            CHECK_DANG_VALUE(2);
            //auto vinfo = value2info[args[0]].top();
            auto it = value2info.find(args[0]);
            ValueInfo *vinfo = NULL;
            if (it != value2info.end() && !it->second.empty()) vinfo = it->second.top();
            if (vinfo && vinfo->len_ && types[0] == VALUE && types[1] == KNUM && types[2] == KNUM) {
                if (vinfo->len_ * vinfo->size_ >= nums[1] * nums[2]) {
                    PUSHFUNCVULN(start, "", LOW, vinfo->pos_);
                } else {
                    PUSHFUNCVULN(start, "指定的读入长度大于可用长度", HIGH, vinfo->pos_);
                }
            } else {
                PUSHFUNCVULN(start, "可能存在漏洞", UNKNOWNLEVEL, BUFOF);
            }
        } break;
        case MALLOC: { // FIXME
            CHECK_DANG_VALUE(0);
            size_t end = i;
            while (str[end] != '=') --end;
            do {--end;} while (ISBLANK(str[end]));
            size_t start = end;
            do {--start;} while (!ISIDCHAR(str[end]));
            auto vinfo = value2info[str.substr(start+1, end-start)].top(); // FIXME
            vinfo->pos_ = HEAP;
            if (types[0] == KNUM) vinfo->len_ = nums[0] / vinfo->size_;

        } break;
        case CALLOC: { // FIXME
            CHECK_DANG_VALUE(0);
            size_t end = i;
            while (str[end] != '=') --end;
            do {--end;} while (ISBLANK(str[end]));
            size_t start = end;
            do {--start;} while (!ISIDCHAR(str[end]));
            auto vinfo = value2info[str.substr(start+1, end-start)].top(); // FIXME
            vinfo->pos_ = HEAP;
            if (types[0] == KNUM && types[1] == KNUM)
                vinfo->len_ = nums[0] * nums[1] / vinfo->size_;

        } break;
        case REALLOC: { // FIXME
            CHECK_DANG_VALUE(0);
            auto vinfo = value2info[args[0]].top();
            vinfo->pos_ = HEAP;
            if (types[1] == KNUM) vinfo->len_ = nums[1] / vinfo->size_;

        } break;
        case FREE: { // FIXME
            auto vinfo = value2info[args[0]].top();
            vinfo->len_ = 0;

        } break;
        case PRINTF: {
            CHECK_PRINT_FMT(0);

        } break;
        case FPRINTF:
        case SPRINTF: {
            CHECK_PRINT_FMT(1);

        } break;
        case SCANF: {
            CHECK_SCANF_FMT(0);

        } break;
        case FSCANF:
        case SSCANF:{
            CHECK_SCANF_FMT(1);

        } break;
        default:
            ;
        }
    } else {
        while (ISIDCHAR(str[i])) ++i;
    }
   //qDebug() << "[INFO3]"  << i << str[i];

  }
}

} // namespace kernel
} // namespace codeana
