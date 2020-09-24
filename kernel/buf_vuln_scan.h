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

void BufVulnScan(vector<int> &pos, vector<string> &info, string &func, ValueInfos &value_infos);

} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_BUF_VULN_SCAN_H_
