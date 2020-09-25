#ifndef CODEANA_KERNEL_BUF_VULN_SCAN_H_
#define CODEANA_KERNEL_BUF_VULN_SCAN_H_

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>

#include "util/trie.h"
#include "func_scanner.h"

using std::string;
using std::vector;
using std::pair;
using std::list;
using std::unordered_map;

namespace codeana {
namespace kernel {

#define VULN_FUNC_LIST \
{"strcpy", "wcscpy", "strncpy", "wcsncpy", "memcpy", "memset", "strcat", "strncat", "wcscat", "wcsncat", \
"gets", "fread", \
"scanf", "sscanf", "fscanf", "vscanf", "vsscanf", "vfscanf", \
"printf", "sprintf", "fprintf", "vprintf", "vsprintf", "vfprintf", \
"malloc"}

enum {STRCPY, WCSCPY, STRNCPY, WCSNCPY, MEMCPY, MEMSET, STRCAT, STRNCAT, WCSCAT, WCSNCAT,
      GETS, FREAD,
      SCANF, SSCANF, FSCANF, VSCANF, VSSCANF, VFSCANF,
      PRINTF, SPRINTF, FPRINTF, VPRINTF, VSPRINTF, VFPRINTF,
      MALLOC};

enum {UNKNOWNTYPE, KSTR, KNUM, VALUE, EXP};
enum {UNKNOWNLEVEL, LOW, MIDDLE, HIGH};
enum {BUFOF, STACKOF, HEAPOF};

void BufVulnScan(vector<int> &pos, vector<int> &func_type, vector<string> &info, vector<int> &errlevel,
                 vector<int> &errtype, string &str, size_t func_start, size_t func_end, ValueInfos &value_infos);

} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_BUF_VULN_SCAN_H_
