#include "longest_common_subsequence.h"

#include <string>
#include <vector>
#include <QDebug>

using std::string;
using std::vector;

namespace codeana {
namespace kernel {

void LongestCommonSubsequence(string &src, string &dst, size_t& len) {
    size_t src_size = src.size();
    size_t dst_size = dst.size();
    len = 0;
    vector<vector<int>> dp(2, vector<int>(src_size+1));
    for (size_t i = 0; i < src_size; ++i) dp[0][i+1] = (src[i] == dst[0]);
    dp[0][src_size] = 0;
    for (size_t i = 1; i < dst_size; ++i) {
        for (size_t j = 0; j < src_size; ++j) {
            if(dst[i] == src[j]) {
                dp[i&1][j+1] = dp[!(i&1)][j] + 1;
            } else if (dp[i&1][j] > dp[!(i&1)][j+1]) {
                dp[i&1][j+1] = dp[i&1][j];
            } else {
                dp[i&1][j+1] = dp[!(i&1)][j+1];
            }
        }
    }
    len = dp[!(dst_size & 1)][src_size];
    return ;
}

}
}
