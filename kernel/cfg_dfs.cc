
#include "cfg_dfs.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <QDebug>

using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::find;

namespace codeana {
namespace kernel {


void Dfs(vector<string> &result, string now, unordered_map<string, vector<string>> &func2subfunc) {
    if (result.size() == func2subfunc.size()) return ;
    // qDebug() << QString::fromStdString(now);
    for (auto &i : func2subfunc[now] ) {
        if (find(result.begin(), result.end(), i) == result.end()) {
            result.push_back(i);
            Dfs(result, i, func2subfunc);
        }
    }
}

int DfsCfg(vector<string> &result, unordered_map<string, vector<string>> &func2subfunc) {
    if (func2subfunc.count("main") == 0) { // FIXME
        return 1;
    }
    result.push_back("main");
    Dfs(result, "main", func2subfunc);
    return 0;
}

}
}

