#ifndef CODEANA_KERNEL_BUF_VULN_SCAN_H_
#define CODEANA_KERNEL_BUF_VULN_SCAN_H_

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "util/trie.h"
#include "func_scanner.h"
using codeana::kernel::util::Trie;

using std::string;
using std::vector;
using std::pair;
using std::list;
using std::unordered_map;
using std::unordered_set;
namespace codeana {
namespace kernel {

/*** 敏感函数 ***/
#define VULN_FUNC_LIST \
{"", "strcpy", "wcscpy", "strncpy", "wcsncpy", "memcpy", "memset", "strcat", "strncat", "wcscat", "wcsncat", \
"gets", "fread", \
"scanf", "sscanf", "fscanf", "vscanf", "vsscanf", "vfscanf", \
"printf", "sprintf", "fprintf", "vprintf", "vsprintf", "vfprintf", \
"malloc", "calloc", "realloc", "free"}

enum {NONE, STRCPY, WCSCPY, STRNCPY, WCSNCPY, MEMCPY, MEMSET, STRCAT, STRNCAT, WCSCAT, WCSNCAT,
      GETS, FREAD,
      SCANF, SSCANF, FSCANF, VSCANF, VSSCANF, VFSCANF,
      PRINTF, SPRINTF, FPRINTF, VPRINTF, VSPRINTF, VFPRINTF,
      MALLOC, CALLOC, REALLOC, FREE};

static Trie buf_vuln_trie(VULN_FUNC_LIST);

/*** 参数类型 ***/
enum {UNKNOWNTYPE, KSTR, KNUM, VALUE, EXP};

/*** 危险级别 ***/
enum {UNKNOWNLEVEL, LOW, MIDDLE, HIGH};

/*** 漏洞类型 ***/
enum {BUFOF, STACKOF, HEAPOF, FORMATSTR, WIDTHOF, SIGNOF};

/*** 格式化字符串类型 ***/
enum {FMT_d, FMT_o, FMT_x, FMT_X, FMT_u, FMT_f, FMT_e, FMT_E, FMT_g, FMT_G, FMT_c, FMT_s, FMT_p,
      FMT_hd, FMT_ho, FMT_hu, FMT_hx, FMT_hX,
      FMT_ld, FMT_lo, FMT_lu, FMT_lx, FMT_lX, FMT_lc, FMT_ls,
      FMT_lld, FMT_llo, FMT_llu, FMT_llx, FMT_llX,
      FMT_le, FMT_lE, FMT_lf, FMT_lg, FMT_lG,
      FMT_lle, FMT_llE, FMT_llf, FMT_llg, FMT_llG};

static Trie format_type_trie({"d", "o", "x", "X", "u", "f", "e", "E", "g", "G", "c", "s", "p",
                              "hd", "ho", "hu", "hx", "hX",
                              "ld", "lo", "lu", "lx", "lX", "lc", "ls",
                              "lld", "llo", "llu", "llx", "llX",
                              "le", "lE", "lf", "lg", "lG",
                              "lle", "llE", "llf", "llg", "llG"});
/*** 类型匹配 ***/
#define CAT8(a,b) ((a<<8)|(b))
#define CAT4(a,b) ((a<<4)|(b))
#define UNSIGN(x)  ((1<<4)|(x))
static unordered_set<int> format_type_set({
                                              CAT8(FMT_d, INT),
                                              CAT8(FMT_o, INT), CAT8(FMT_o, UNSIGN(INT)),
                                              CAT8(FMT_x, INT), CAT8(FMT_x, UNSIGN(INT)),
                                              CAT8(FMT_X, INT), CAT8(FMT_X, UNSIGN(INT)),
                                              CAT8(FMT_u, UNSIGN(INT)),
                                              CAT8(FMT_f, FLOAT), CAT8(FMT_e, FLOAT), CAT8(FMT_g, FLOAT), CAT8(FMT_G, FLOAT),
                                              CAT8(FMT_c, CHAR), CAT8(FMT_c, UNSIGN(CHAR)),
                                              CAT8(FMT_llu, UNSIGN(LONG_LONG)),
                                              CAT8(FMT_hd, SHORT),
                                              CAT8(FMT_ho, SHORT), CAT8(FMT_ho, UNSIGN(SHORT)),
                                              CAT8(FMT_hx, SHORT), CAT8(FMT_hx, UNSIGN(SHORT)),
                                              CAT8(FMT_hX, SHORT), CAT8(FMT_hX, UNSIGN(SHORT)),
                                              CAT8(FMT_ld, LONG),
                                              CAT8(FMT_lo, LONG), CAT8(FMT_lo, UNSIGN(LONG)),
                                              CAT8(FMT_lx, LONG), CAT8(FMT_lx, UNSIGN(LONG)),
                                              CAT8(FMT_lX, LONG), CAT8(FMT_lX, UNSIGN(LONG)),
                                              CAT8(FMT_lc, WCHAR_T), CAT8(FMT_lc, UNSIGN(WCHAR_T)),
                                              CAT8(FMT_lld, LONG_LONG),
                                              CAT8(FMT_llo, LONG_LONG), CAT8(FMT_llo, UNSIGN(LONG_LONG)),
                                              CAT8(FMT_llx, LONG_LONG), CAT8(FMT_llx, UNSIGN(LONG_LONG)),
                                              CAT8(FMT_llX, LONG_LONG), CAT8(FMT_llX, UNSIGN(LONG_LONG)),
                                              CAT8(FMT_le, FLOAT), CAT8(FMT_lE, FLOAT), CAT8(FMT_lf, FLOAT),
                                              CAT8(FMT_lg, FLOAT), CAT8(FMT_lG, FLOAT),
                                              CAT8(FMT_le, DOUBLE), CAT8(FMT_lE, DOUBLE), CAT8(FMT_lf, DOUBLE),
                                              CAT8(FMT_lg, DOUBLE), CAT8(FMT_lG, DOUBLE),
                                          });

void BufVulnScan(vector<int> &pos, vector<int> &func_type, vector<string> &info, vector<int> &errlevel,
                 vector<int> &errtype, string &str, size_t func_start, size_t func_end, ValueInfos &value_infos);

} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_BUF_VULN_SCAN_H_
