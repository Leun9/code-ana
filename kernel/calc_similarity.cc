#include "calc_similarity.h"

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

namespace codeana {
namespace kernel {

void CalcSimlarity(vector<string> &src, vector<string> &dst, size_t min_size,  // min_size >= 2
  vector<size_t> &src_pos, vector<size_t> &dst_pos, vector<size_t> &len, size_t &len_sum, size_t &len_total) {
  size_t src_size = src.size();
  size_t dst_size = dst.size();
  len_sum = 0;
  len_total = 0;
  //vector<CommonSubStrInfo> result;
  vector<size_t> dp(src_size);
  for (size_t i = 0; i < dst_size; ++i) {
    len_total++;
    if (dp[src_size - 1] >= min_size) {
      //result.push_back(CommonSubStrInfo(src_size - dp[src_size-1], i - dp[src_size-1], dp[src_size-1]));
      src_pos.push_back(src_size - dp[src_size-1]);
      dst_pos.push_back(i - dp[src_size-1]);
      len.push_back(dp[src_size-1]);
      len_sum += dp[src_size-1];
    }
    for (size_t j = src_size - 1; j > 0; --j) {
      if (dst[i] == src[j]) {
        dp[j] = dp[j - 1] + 1;
      } else {
        dp[j] = 0;
        if (dp[j - 1] >= min_size) {
          //result.push_back(CommonSubStrInfo(j - dp[j-1], i - dp[j-1], dp[j-1]));
          src_pos.push_back(j - dp[j-1]); 
          dst_pos.push_back(i - dp[j-1]);
          len.push_back(dp[j-1]);
          len_sum += dp[j-1];
        }
      }
    }
    if (dst[i] == src[0]) {
      dp[0] = 1;
    } else {
      dp[0] = 0;
    }
  }
  for (size_t i = src_size - 1; i > 0; --i) {
    if (dp[i] >= min_size)  {
      //result.push_back(CommonSubStrInfo(i - dp[i] + 1, dst_size - dp[i], dp[i]));
      src_pos.push_back(i - dp[i] + 1);
      dst_pos.push_back(dst_size - dp[i]);
      len.push_back(dp[i]);
      len_sum += dp[i];
    }
  }
  //return result;
  return ;
}

} // namespace kernel
} // namespace codeana
