#ifndef CODEANA_KERNEL_CALC_SIMILARITY_H_
#define CODEANA_KERNEL_CALC_SIMILARITY_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace codeana {
namespace kernel {

void CalcTokensSimlarity(vector<string> &src, vector<string> &dst, size_t min_size,  // min_size >= 2
  vector<size_t> &src_pos, vector<size_t> &dst_pos, vector<size_t> &len);

void CalcStringSimlarity(string &src, string &dst, size_t min_size,  // min_size >= 2
  vector<size_t> &src_pos, vector<size_t> &dst_pos, vector<size_t> &len);


} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_CALC_SIMILARITY_H_
