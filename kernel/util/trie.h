#ifndef CODEANA_KERNEL_UTIL_CALC_SIMILARITY_H_
#define CODEANA_KERNEL_UTIL_CALC_SIMILARITY_H_

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

namespace codeana {
namespace kernel {
namespace util {

struct Trie {

  struct TrieNode {
    unordered_map<char, TrieNode*> next_;
    bool is_leaf_;

    TrieNode() : is_leaf_(0) {};
    TrieNode(bool is_leaf) : is_leaf_(is_leaf) {}
  };
  
  Trie() {root_ = new TrieNode();};
  ~Trie() {}; // TODO
  
  bool Jump(TrieNode* &now, char ch);
  void Insert(string str);
  
  TrieNode* root_;
  
};

} // util
} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_UTIL_CALC_SIMILARITY_H_
