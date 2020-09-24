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

namespace codeana {
namespace kernel {

static Trie buf_vuln_trie({"strcpy", "strncpy", "memcpy", "strcat", "strncat", "gets", "read", "fread"
                           "scanf", "sscanf", "fscanf", "vscanf", "vsscanf", "vfscanf",
                           "printf", "sprintf", "fprintf", "vprintf", "vsprintf", "vfprintf"});

void BufVulnScan(vector<int> &pos, vector<string> &info, string &func, ValueInfos &value_infos) {
  pos.clear();
  info.clear();
  size_t func_size = func.size();
  for (size_t i = 0; i < func_size;) {
    while (!ISIDCHAR(func[i]) && i < func_size) ++i;
    size_t start = i;
    Trie::TrieNode* now = buf_vuln_trie.root_;
    while (buf_vuln_trie.Jump(now, func[i])) ++i;
    if (now->is_leaf_ && !ISIDCHAR(func[i])) {
      qDebug() << func.substr(start, i-start).c_str();
      pos.push_back(start);
      info.push_back(func.substr(start, i-start));
    } else {
      while (ISIDCHAR(func[i])) ++i;
    }
  }
}

} // namespace kernel
} // namespace codeana
