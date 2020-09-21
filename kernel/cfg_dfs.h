#ifndef CODEANA_KERNEL_CFG_DFS_H_
#define CODEANA_KERNEL_CFG_DFS_H_

#include <vector>
#include <string>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;

namespace codeana {
namespace kernel {

    int DfsCfg(vector<string> &result, unordered_map<string, vector<string>> &func2subfunc);

}
}

#endif // CODEANA_KERNEL_CFG_DFS_H_
