#ifndef CODEANA_KERNEL_LEXICAL_ANALYZER_H_
#define CODEANA_KERNEL_LEXICAL_ANALYZER_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using std::unordered_map;
using std::unordered_set;
using std::string;
using std::vector;

namespace codeana {
namespace kernel {

// TODO : "define" and "typedef"
// FIXME : cannot great process "typedef" 
class LexicalAnalyzer {
 private:
  struct Node {
    Node() : edges(unordered_map<char, Node*>(256)) {}; 
    unordered_map<char, Node*> edges;
  };

 public:
  LexicalAnalyzer();
  void Init();
  // FIXME : Line-level
  vector<string> GetFileTokens(FILE*);
  void PrintToken(FILE*, vector<string>);

 private:
  static unordered_set<string> keywords_;
  static vector<string> keep_chars_;

  static vector<string> decode_;
  static unordered_map<string, int> encode_;

  static Node* start_;
  static Node* space_;
  static Node* newline_;
  static Node* id_or_keyword_;
  static Node* const_num_;
  static Node* const_char_;
  static Node* const_char2_;
  static Node* const_str_;
  static Node* const_str2_;
  static Node* slash_;
  static Node* line_comment_;
  static Node* line_comment2_;
  static Node* block_comment_;
  static Node* block_comment_newline_;
  static Node* block_comment2_;
  static Node* block_comment3_;
  static Node* keep_char_ ;
};

} // namespace kernel
} // namespace codeana

#endif // CODEANA_KERNEL_LEXICAL_ANALYZER_H_