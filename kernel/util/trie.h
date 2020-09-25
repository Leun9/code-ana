#ifndef CODEANA_KERNEL_UTIL_TRIE_H_
#define CODEANA_KERNEL_UTIL_TRIE_H_

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
    int leaf_num_;

    TrieNode() : is_leaf_(0) {};
    TrieNode(bool is_leaf) : is_leaf_(is_leaf) {}
  };

  Trie(const std::initializer_list<string> &lst);
  ~Trie() {}; // TODO

  bool Jump(TrieNode* &now, char ch);
  void Insert(string str);

  TrieNode* root_;
  int leaf_cnt_;

};

} // util
} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_UTIL_TRIE_H_
