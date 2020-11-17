#ifndef CODEANA_KERNEL_LEXICAL_ANALYZER_H_
#define CODEANA_KERNEL_LEXICAL_ANALYZER_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>
#include <cstring>

using std::unordered_map;
using std::unordered_set;
using std::string;
using std::vector;
using std::pair;

namespace codeana {
namespace kernel {

#define PRTERROR do {fprintf(stderr, "[File:%s][Line:%d]Lex Ana Error.\n", __FILE__, __LINE__);} while(0);

#define N_KEYWORDS 74
#define KEYWORDS \
  "asm", "auto", "bool", "break;", "case", "catch", "char", "class", "const", "const_cast", "continue", "default", "delete", \
  "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "FALSE", "float", "for", "friend", "goto", \
  "if", "inline", "int", "long", "mutable", "namespace", "new", "operator", "private", "protected", "public", "register", \
  "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_cast", "struct", "switch", "template", "this", "throw", \
  "TRUE", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", \
  "alignas", "alignof", "char16_t", "char32_t", "constexpr", "decltype", "noexpect", "nullptr", "static_assert", "thread_local", \
  "include", \
  "define"

#define N_KEEPCHARS 26
#define KEEPCHARS \
  "+", "-", "*", "%", "!", "~", "&", "|", "^", "=", ".", "?", ":", ";", ",", "(", ")", "{", "}", "<", ">", "[", "]", "#", "/", "\\"

#define N_EXTENSION 5
#define EXTENSION \
  "KNUM_", "KCHAR_", "KSTR_", "ID_", "COMMENT_"


class LexicalAnalyzer {

 private:
  struct Node {
    Node() {memset(edges, 0, sizeof(Node*)*256);};
    Node* edges[256];
  };

 public:
  //LexicalAnalyzer();

  static void Init();
  static void GetStringTokens(vector<string>& result, const string&);
  static void GetStringFuncTokens(unordered_map<string, string> &func2tokens,
                                  unordered_map<string, pair<size_t, size_t>> &func_pos,
                                  unordered_map<string, vector<string>> &func2subfunc,
                                  string&);
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
