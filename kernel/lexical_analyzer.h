#ifndef CODEANA_KERNEL_LEXICAL_ANALYZER_H_
#define CODEANA_KERNEL_LEXICAL_ANALYZER_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>

using std::unordered_map;
using std::unordered_set;
using std::string;
using std::vector;
using std::pair;

namespace codeana {
namespace kernel {

// FIXME : cannot great process "typedef" and "define"
class LexicalAnalyzer {
 private:
  struct Node {
    Node() : edges(unordered_map<char, Node*>(256)) {};
    unordered_map<char, Node*> edges;
  };

 public:
  //LexicalAnalyzer();
  static void Init();
  static void GetStringTokens(vector<string>& result, const string&);
  static void GetStringFuncTokens(unordered_map<string, string> &func2tokens, unordered_map<string, pair<size_t, size_t>> func_pos,
                                  unordered_map<string, vector<string>> &func2subfunc, const string&);
  static void PrintTokens(FILE*, const vector<string>& result);
  static void DecodeTokens(string &, const vector<string>& result);

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
  static Node* keep_char_;
};

} // namespace kernel
} // namespace codeana

#endif // CODEANA_KERNEL_LEXICAL_ANALYZER_H_
