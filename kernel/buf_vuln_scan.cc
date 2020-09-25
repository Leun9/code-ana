#include "buf_vuln_scan.h"

#include <string>
#include <vector>
#include <utility>
#include <list>
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
using std::unordered_map;
using codeana::kernel::util::Trie;
using codeana::kernel::util::ISIDCHAR;
using codeana::kernel::util::ISBLANK;

namespace codeana {
namespace kernel {

#define VULN_FUNC_LIST \
{"strcpy", "wcscpy", "strncpy", "wcsncpy", "memcpy", "memset", "strcat", "strncat", "wcscat", "wcsncat", \
"gets", "fread", \
"scanf", "sscanf", "fscanf", "vscanf", "vsscanf", "vfscanf", \
"printf", "sprintf", "fprintf", "vprintf", "vsprintf", "vfprintf"}

enum {STRCPY, WCSCPY, STRNCPY, WCSNCPY, MEMCPY, MEMSET, STRCAT, STRNCAT, WCSCAT, WCSNCAT,
      GETS, FREAD,
      SCANF, SSCANF, FSCANF, VSCANF, VSSCANF, VFSCANF,
      PRINTF, SPRINTF, FPRINTF, VPRINTF, VSPRINTF, VFPRINTF};

enum {UNKNOWNTYPE, KSTR, KNUM, VALUE, EXP};
enum {UNKNOWNLEVEL, LOW, MIDDLE, HIGH};

static Trie buf_vuln_trie(VULN_FUNC_LIST);

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
            args.push_back(str.substr(start, i+1-start));
            nums.push_back(len);
            types.push_back(KSTR);
            while (str[i] != ',' && str[i] != ')') ++i;
            continue;
        } else if (str[i] == 'L' && str[i+1] == '"') {
            ++i;
            size_t len = 0;
            do {++i; ++len; if (str[i] == '\\') ++i;} while (str[i] != '\"');
            // qDebug() << i;
            args.push_back(str.substr(start, i+1-start));
            nums.push_back(len * 2);
            types.push_back(KSTR);
            while (str[i] != ',' && str[i] != ')') ++i;
            continue;
        } else if (str[i] >= '0' && str[i] <= '9'){
            while (str[i] >= '0' && str[i] <= '9') ++i;
            end = i;
            while (ISBLANK(i)) ++i;
            if (str[i] == ',' || str[i] == ')') {
                args.push_back(str.substr(start, end-start));
                size_t x = 0;
                for (size_t i = start; i < end; ++i) {x *= 10; x += str[i] - '0';}
                nums.push_back(x);
                types.push_back(KNUM);
                continue;
            }
        } else if (ISIDCHAR(str[i])) { // FIXME 类型强制
            while (ISIDCHAR(str[i])) ++i;
            end = i;
            while (ISBLANK(i)) ++i;
            if (str[i] == ',' || str[i] == ')') {
                args.push_back(str.substr(start, end-start));
                nums.push_back(0); // FIXME
                types.push_back(VALUE);
                continue;
            }
        }
        while (str[i] != ',' && str[i] != ')') ++i;
        if (start == i) continue;
        args.push_back(str.substr(start, i-start));
        nums.push_back(0); //
        types.push_back(EXP);
    }
}

#define PUSH(x, y, z) do {pos.push_back(x); func_type.push_back(now->leaf_num_); info.push_back(y); errlevel.push_back(z);} while(0);
void BufVulnScan(vector<int> &pos, vector<int> &func_type, vector<string> &info, vector<int> &errlevel, string &func, ValueInfos &value_infos) {
  pos.clear();
  info.clear();
  errlevel.clear();
  unordered_map<string, ValueInfo*> value2info;
  for (auto &vinfo : value_infos) {
    value2info[vinfo.name_] = &vinfo;
  }
  size_t func_size = func.size();
  // qDebug() << func_size;
  for (size_t i = 0; i < func_size;) {
    while (!ISIDCHAR(func[i]) && i < func_size) ++i;
    size_t start = i;
    Trie::TrieNode* now = buf_vuln_trie.root_;
    while (buf_vuln_trie.Jump(now, func[i])) ++i;
    if (now->is_leaf_ && !ISIDCHAR(func[i])) {
      vector<string> args;
      vector<int> types;
      vector<size_t> nums;
      GetArg(func, i, args, types, nums);

      if (now->leaf_num_  == STRCPY || now->leaf_num_ == WCSCPY) {
            if (types[0] == VALUE && types[1] == KSTR) {
                auto vinfo = value2info[args[0]];
                if (vinfo->len_ * vinfo->size_ >= nums[1]) {
                    PUSH(start, "", LOW);
                } else {
                    PUSH(start, "写入长度大于数组长度", HIGH);
                }
            } else {
                PUSH(start, "可能存在漏洞", MIDDLE);
            }

    } else if (now->leaf_num_ == STRNCPY || now->leaf_num_ == MEMCPY || now->leaf_num_ == MEMSET) {
          if (types[0] == VALUE && types[2] == KNUM) {
              auto vinfo = value2info[args[0]];
              if (vinfo->len_ * vinfo->size_ >= nums[2]) {
                  PUSH(start, "", LOW);
              } else {
                  PUSH(start, "指定的写入长度大于数组长度", HIGH);
              }
          } else {
              PUSH(start, "可能存在漏洞", MIDDLE);
          }

    }  else if (now->leaf_num_ == WCSNCPY) {
          if (types[0] == VALUE && types[2] == KNUM) {
              auto vinfo = value2info[args[0]];\
              if (vinfo->len_ * vinfo->size_ >= nums[2]*2) {
                  PUSH(start, "", LOW);
              } else {
                  PUSH(start, "指定的写入长度大于数组长度", HIGH);
              }
          } else {
              PUSH(start, "可能存在漏洞", MIDDLE);
          }

    } else if (now->leaf_num_ >= STRCAT && now->leaf_num_ <= GETS) {
            PUSH(start, "可能存在漏洞", HIGH);

      } else if (now->leaf_num_ == FREAD) {
            if (types[0] == VALUE && types[1] == KNUM && types[2] == KNUM) {
                if (value2info[args[0]]->len_ >= nums[1] * nums[2]) {
                    PUSH(start, "", LOW);
                } else {
                    PUSH(start, "读入长度大于数组长度", HIGH);
                }
            } else {
                PUSH(start, "可能存在漏洞", MIDDLE);
            }
      }

    } else {
      while (ISIDCHAR(func[i])) ++i;
    }

  }
}

} // namespace kernel
} // namespace codeana
