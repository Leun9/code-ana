/***

keyword :
"asm", "auto", "bool", "break;", "case", "catch", "char", "class", "const", "const_cast", "continue", "default", "delete",
"do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "FALSE", "float", "for", "friend", "goto",
"if", "inline", "int", "long", "mutable", "namespace", "new", "operator", "private", "protected", "public", "register",
"reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_cast", "struct", "switch", "template", "this", "throw",
"TRUE", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while"
"alignas", "alignof", "char16_t", "char32_t", "constexpr", "decltype", "noexpect", "nullptr", "static_assert", "thread_local"

const : 
  const num
  const char
  const string

identifier :
[a-z|A-Z][a-z|A-Z|0-9|_]*


operator : 
"+", "-", "*", "/", "%", "!", "~", "&", "|", "^", "=",
"++", "--", "&&", "||", "<=", "!=", "==", ">=",
"+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<", ">>", "<<=", ">>=",
",", ".", "->", "&", "*", "?", ":", "::"

Delimiter : 
"{", "}", ";", "(", ")", ",", "<", ">"

notes :
"//", "/*", "*[/]"

other special char:
" ", "\n"

***/

#include "lexical_analyzer.h"

#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

#define PRTERROR do {fprintf(stderr, "[File:%s][Line:%d]Lex Ana Error.\n", __FILE__, __LINE__);} while(0);

#define KEYWORDS \
  "asm", "auto", "bool", "break;", "case", "catch", "char", "class", "const", "const_cast", "continue", "default", "delete", \
  "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "FALSE", "float", "for", "friend", "goto", \
  "if", "inline", "int", "long", "mutable", "namespace", "new", "operator", "private", "protected", "public", "register", \
  "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_cast", "struct", "switch", "template", "this", "throw", \
  "TRUE", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", \
  "alignas", "alignof", "char16_t", "char32_t", "constexpr", "decltype", "noexpect", "nullptr", "static_assert", "thread_local", \
  "include"

#define KEEPCHARS \
  "+", "-", "*", "%", "!", "~", "&", "|", "^", "=", ".", "?", ":", ";", ",", "(", ")", "{", "}", "<", ">", "[", "]", "#"

#define EXTENSION \
  "CONST_NUM", "CONST_CHAR", "CONST_STR", "ID", "COMMENT"

using std::initializer_list;
using std::unordered_set;
using std::unordered_map;
using std::string;
using std::vector;
using std::cout;

/* FIXME : 
  do not consider tab
  const_num_ cannot correctly identify prefix '-'
  const_num_ cannot correctly identify "314159E-5L" (middle '-')
  const_char_ cannot correctly identify if char is '\''
  const_str_ cannot correctly identify is string contains '\"'
  opretor should be separated
  continuation character '\'
*/

namespace codeana {
namespace kernel {

unordered_set<string> LexicalAnalyzer::keywords_({KEYWORDS});
vector<string> LexicalAnalyzer::keep_chars_({KEEPCHARS});
vector<string> LexicalAnalyzer::decode_({KEYWORDS, KEEPCHARS, EXTENSION});
unordered_map<string, int> LexicalAnalyzer::encode_(256);

LexicalAnalyzer::Node* LexicalAnalyzer::start_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::space_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::newline_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::id_or_keyword_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::const_num_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::const_char_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::const_char2_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::const_str_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::const_str2_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::slash_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::line_comment_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::line_comment2_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::block_comment_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::block_comment_newline_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::block_comment2_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::block_comment3_ = new Node();
LexicalAnalyzer::Node* LexicalAnalyzer::keep_char_  = new Node();

LexicalAnalyzer::LexicalAnalyzer() {}

// Init DFA
void LexicalAnalyzer::Init() {
  
  for (int i = 0; i < decode_.size(); ++i) {
    encode_[decode_[i]] = i;
  }

  start_->edges['\n'] = newline_;
  start_->edges[' '] = space_;
  for (char i = 'a'; i <= 'z'; ++i) start_->edges[i] = id_or_keyword_;
  for (char i = 'A'; i <= 'Z'; ++i) start_->edges[i] = id_or_keyword_;
  start_->edges['_'] = id_or_keyword_;
  for (char i = '0'; i <= '9'; ++i) start_->edges[i] = const_num_;
  start_->edges['\''] = const_char_;
  start_->edges['\"'] = const_str_;
  start_->edges['/'] = slash_;
  for (string i : keep_chars_) start_->edges[i[0]] = keep_char_;

  for (char i = 'a'; i <= 'z'; ++i) id_or_keyword_->edges[i] = id_or_keyword_;
  for (char i = 'A'; i <= 'Z'; ++i) id_or_keyword_->edges[i] = id_or_keyword_;
  for (char i = '0'; i <= '9'; ++i) id_or_keyword_->edges[i] = id_or_keyword_;
  id_or_keyword_->edges['_'] = id_or_keyword_;

  for (char i = 'a'; i <= 'z'; ++i) const_num_->edges[i] = const_num_;
  for (char i = 'A'; i <= 'Z'; ++i) const_num_->edges[i] = const_num_;
  for (char i = '0'; i <= '9'; ++i) const_num_->edges[i] = const_num_;

  for (char i = 0x20; i <= 0x7E; ++i) const_char_->edges[i] = const_char_;
  const_char_->edges['\''] = const_char2_;

  for (char i = 0x20; i <= 0x7E; ++i) const_str_->edges[i] = const_str_;
  const_str_->edges['\n'] = const_str_;
  const_str_->edges['\"'] = const_str2_;

  slash_->edges['/'] = line_comment_;
  slash_->edges['*'] = block_comment_;

  for (char i = 0x20; i <= 0x7E; ++i) line_comment_->edges[i] = line_comment_;
  line_comment_->edges['\n'] = line_comment2_;

  for (char i = 0x20; i <= 0x7E; ++i) block_comment_->edges[i] = block_comment_;
  block_comment_->edges['\n'] = block_comment_newline_;
  block_comment_->edges['*'] = block_comment2_; 
  for (char i = 0x20; i <= 0x7E; ++i) block_comment_newline_->edges[i] = block_comment_;
  block_comment_newline_->edges['\n'] = block_comment_newline_;
  block_comment_newline_->edges['*'] = block_comment2_; 
  for (char i = 0x20; i <= 0x7E; ++i) block_comment2_->edges[i] = block_comment_;
  block_comment2_->edges['\n'] = block_comment_newline_;
  block_comment2_->edges['*'] = block_comment2_; 
  block_comment2_->edges['/'] = block_comment3_;
}

vector<string> LexicalAnalyzer::GetFileTokens(FILE* fp) {
  Node* now = start_;
  vector<string> result;
  string line_token;
  string token_buf;
  char ch;
  do {
    ch = fgetc(fp);
    if (now->edges.count(ch) != 0) {
      now = now->edges[ch];
      if (now == block_comment_newline_) {
        //line_token.push_back(encode_["COMMENT"]); // filter comment
        result.push_back(line_token); 
        line_token.clear();
        token_buf.clear();
      }
      token_buf.push_back(ch);
      continue;
    }
    // now->edges.count(ch) == 0
    // FIXME : token analysis
    if (now == start_) {
      PRTERROR;
    } else if (now == space_) {
      ; // space : do nothing
    } else if (now == newline_) {
      result.push_back(line_token);
      line_token.clear();
      token_buf.clear();
    } else if (now == id_or_keyword_) {
      //cout << "[" << token_buf << "]";
      if (keywords_.count(token_buf)) {
        line_token.push_back(encode_[token_buf]);
      } else {
        line_token.push_back(encode_["ID"]);
      }
    } else if (now == const_num_) {
      line_token.push_back(encode_["CONST_NUM"]);
    } else if (now == const_char_) {
      PRTERROR;
    } else if (now == const_char2_) {
      line_token.push_back(encode_["CONST_CHAR"]);
    } else if (now == const_str_) {
      PRTERROR;
    } else if (now == const_str2_) {
      line_token.push_back(encode_["CONST_STR"]);
    } else if (now == slash_) {
      line_token.push_back(encode_["/"]);
    } else if (now == line_comment_) {
      PRTERROR;
    } else if (now == line_comment2_) {
      //line_token.push_back(encode_["COMMENT"]); // filter comment
      result.push_back(line_token);
      line_token.clear();
      token_buf.clear();
    } else if (now == block_comment_) {
      PRTERROR;
    } else if (now == block_comment2_) {
      PRTERROR;
    } else if (now == block_comment3_) {
      ;// filter comment
    } else if (now == keep_char_) {
      line_token.push_back(encode_[token_buf]);
    }
    now = start_->edges[ch];
    token_buf = ch;
  } while (ch != EOF);
  return result;
}

void LexicalAnalyzer::PrintToken(FILE* fp, vector<string> result) {
  for (auto &line : result) {
    for (char token : line) {
      for (char ch : decode_[token]) {
        fputc(ch, fp);
      }
    }
    fputc('\n', fp);
  }
}

} // namespace kernel
} // namespace codeana