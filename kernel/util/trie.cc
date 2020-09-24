#include "trie.h"

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;
using codeana::kernel::util::Trie;

bool Trie::Jump(Trie::TrieNode* &now, char ch) {
  if (now->next_.find(ch) == now->next_.end()) {
    return false;
  } else {
    now = now->next_[ch]; // FIXME 
    return true;
  }
}

void Trie::Insert(string str) {
  TrieNode* now = root_;
  for (int i = 0; i < str.size(); ++i) {
    if (now->next_.find(str[i]) == now->next_.end()) {
      now->next_[str[i]] = new TrieNode();
    } 
    now = now->next_[str[i]]; // FIXME
    //putchar(str[i]);
  }
  now->is_leaf_ = 1;
  //putchar('\n');
}

// int main() {
//   Trie trie;
//   trie.Insert("husthust");
//   trie.Insert("csecse");
//   trie.Insert("hustcse");

//   auto now = trie.root_;
//   string str("hustck");

//   now = trie.root_;
//   for (auto ch : str) {
//     //for (auto vk : now->next_) {putchar(vk.first);} putchar('\n');
//     if (!trie.Jump(now, ch)) {printf("Jump failed : %c\n", ch); break;}
//   }

//   return 0;
// }