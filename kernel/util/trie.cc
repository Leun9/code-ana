#include "trie.h"

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;
using codeana::kernel::util::Trie;

Trie::Trie(const std::initializer_list<string> &lst) {
  leaf_cnt_ = 0;
  root_ = new TrieNode();
  for (auto &str : lst) {
    this->Insert(str);
  }
}

bool Trie::Jump(Trie::TrieNode* &now, char ch) {
  auto it = now->next_.find(ch);
  if (it == now->next_.end()) {
      return false;
  } else {
    now = it->second;
    return true;
  }
}

void Trie::Insert(string str) {
  if (str.size() == 0) {++leaf_cnt_; return;}
  TrieNode* now = root_;
  for (size_t i = 0; i < str.size(); ++i) {
    if (now->next_.find(str[i]) == now->next_.end()) {
      now->next_[str[i]] = new TrieNode();
    }
    now = now->next_[str[i]]; // FIXME 重复计算hash
    //putchar(str[i]);
  }
  now->is_leaf_ = 1;
  now->leaf_num_ = leaf_cnt_; // FIXME 不考虑重复插入相同节点
  ++leaf_cnt_;
  //putchar('\n');
}

// int main() {
//   Trie trie({"husthust", "hustcse"});

//   auto now = trie.root_;
//   string str("hustck");

//   now = trie.root_;
//   for (auto ch : str) {
//     //for (auto vk : now->next_) {putchar(vk.first);} putchar('\n');
//     if (!trie.Jump(now, ch)) {printf("Jump failed : %c\n", ch); break;}
//   }

//   return 0;
// }
