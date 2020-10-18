#ifndef CODEANA_KERNEL_FUZZER_H_
#define CODEANA_KERNEL_FUZZER_H_

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <unordered_set>

using std::string;
using std::vector;
using std::pair;
using std::list;
using std::unordered_map;
using std::unordered_set;

namespace codeana {
namespace kernel {

void fuzzer(string &res, string &str, bool value, bool func, bool newline, bool order);


} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_FUZZER_H_
