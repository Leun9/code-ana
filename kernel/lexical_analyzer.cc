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
"//", "[/]*", "*[/]"

other special char:
" ", "\n"

***/

#include "lexical_analyzer.h"

#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include <QDebug>

#define PRTERROR do {fprintf(stderr, "[File:%s][Line:%d]Lex Ana Error.\n", __FILE__, __LINE__);} while(0);
#define ISIDCHAR(x) ((x <= '9' && x >= '0') || (x <= 'z' && x >= 'a') || (x <= 'Z' && x >= 'A') || (x == '_'))

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

using std::initializer_list;
using std::unordered_set;
using std::unordered_map;
using std::string;
using std::vector;
using std::cout;
using std::pair;

/* FIXME :
  do not consider tab
  const_num_ cannot correctly identify prefix '-'
  const_num_ cannot correctly identify "314159E-5L" (middle '-')
  const_char_ cannot correctly identify if char is '\''
  const_str_ cannot correctly identify is string contains '\"'
  opretor should be separated
  continuation character '\'
  process define and typedef
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

//LexicalAnalyzer::LexicalAnalyzer() {}

// Init DFA
void LexicalAnalyzer::Init() {

  for (size_t i = 0; i < decode_.size(); ++i) {
    encode_[decode_[i]] = i;
  }

  // start node
  start_->edges['\n'] = newline_;
  start_->edges[' '] = space_;
  for (char i = 'a'; i <= 'z'; ++i) start_->edges[i] = id_or_keyword_;
  for (char i = 'A'; i <= 'Z'; ++i) start_->edges[i] = id_or_keyword_;
  start_->edges['_'] = id_or_keyword_;
  for (char i = '0'; i <= '9'; ++i) start_->edges[i] = const_num_;
  start_->edges['\''] = const_char_;
  start_->edges['\"'] = const_str_;
  for (string i : keep_chars_) start_->edges[i[0]] = keep_char_; // include slash
  start_->edges['/'] = slash_; // slash

  // id_or_keyword node
  for (char i = 'a'; i <= 'z'; ++i) id_or_keyword_->edges[i] = id_or_keyword_;
  for (char i = 'A'; i <= 'Z'; ++i) id_or_keyword_->edges[i] = id_or_keyword_;
  for (char i = '0'; i <= '9'; ++i) id_or_keyword_->edges[i] = id_or_keyword_;
  id_or_keyword_->edges['_'] = id_or_keyword_;

  // const_num node
  for (char i = 'a'; i <= 'z'; ++i) const_num_->edges[i] = const_num_;
  for (char i = 'A'; i <= 'Z'; ++i) const_num_->edges[i] = const_num_;
  for (char i = '0'; i <= '9'; ++i) const_num_->edges[i] = const_num_;

  // const_char node
  for (char i = 0x20; i <= 0x7E; ++i) const_char_->edges[i] = const_char_;
  const_char_->edges['\''] = const_char2_;

  // const_str node
  for (char i = 0x20; i <= 0x7E; ++i) const_str_->edges[i] = const_str_;
  const_str_->edges['\n'] = const_str_;
  const_str_->edges['\"'] = const_str2_;

  // slash node
  slash_->edges['/'] = line_comment_;
  slash_->edges['*'] = block_comment_;

  // line_comment node
  for (char i = 0x20; i <= 0x7E; ++i) line_comment_->edges[i] = line_comment_;
  line_comment_->edges['\n'] = line_comment2_;

  // block comment node
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

void LexicalAnalyzer::GetStringTokens(vector<string> &result, const string &str) {
  result.clear();
  Node* now = start_;
  unordered_map<string, string> define_map; // FIXME : process define
  string line_token;
  string token_buf;
  char ch;
  size_t str_size = str.size();
  for (size_t i = 0; i <= str_size; ++i) {
    if (i == str_size) ch = 0; else ch = str[i]; // FIXME
    if (now->edges.count(ch) != 0) {
      now = now->edges[ch];
      if (now == block_comment_newline_) {
        //line_token.push_back(encode_["COMMENT_"]); // filter comment
        result.push_back(line_token);
        line_token.clear();
        token_buf.clear();
      }
      token_buf.push_back(ch);
      continue;
    }
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
      if (token_buf == "define") { // FIXME : process define
        while (str[i] == ' ') ++i;
        size_t id_start = i;
        while (str[i] != ' ') ++i;
        size_t id_len = i - id_start;
        while (str[i] == ' ') ++i;
        size_t str_start = i;
        while (str[i] != '\n') ++i;
        size_t str_len = i - str_start;
        string define_str(str.substr(str_start, str_len));
        vector<string> result_temp;
        GetStringTokens(result_temp, define_str);
        define_map[str.substr(id_start, id_len)] = result_temp[0];
        line_token.push_back(encode_["define"]);
        result.push_back(line_token);
        line_token.clear();
        token_buf.clear();
      } else if (define_map.count(token_buf)) {
        line_token.append(define_map[token_buf]);
      } else if (keywords_.count(token_buf)) {
        line_token.push_back(encode_[token_buf]);
      } else {
        line_token.push_back(encode_["ID_"]);
      }
    } else if (now == const_num_) {
      line_token.push_back(encode_["KNUM_"]);
    } else if (now == const_char_) {
      PRTERROR;
    } else if (now == const_char2_) {
      line_token.push_back(encode_["KCHAR_"]);
    } else if (now == const_str_) {
      PRTERROR;
    } else if (now == const_str2_) {
      line_token.push_back(encode_["KSTR_"]);
    } else if (now == slash_) {
      line_token.push_back(encode_["/"]);
    } else if (now == line_comment_) {
      PRTERROR;
    } else if (now == line_comment2_) {
      //line_token.push_back(encode_["COMMENT_"]); // filter comment
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
  };
  result.push_back(line_token);
  //cout << "result size : " << result.size() << "\n";
  return ;
}

void LexicalAnalyzer::GetStringFuncTokens(unordered_map<string, string> &func2tokens, unordered_map<string, pair<size_t, size_t>> func_pos,
                                unordered_map<string, vector<string>> &func2subfunc, const string &str) {
    func2tokens.clear();
    func2subfunc.clear();
    Node* now = start_;
    unordered_map<string, string> define_map; // FIXME : process define
    char ch;
    size_t str_size = str.size();
    // first time : get func_start, func_end, func2tokens
    string func_token;
    string func_name;
    string token_buf;
    size_t start_temp;
    int dep_cnt = 0;
    bool in_func = false;
    for (size_t i = 0; i <= str_size; ++i) {
      if (i == str_size) ch = 0; else ch = str[i]; // FIXME
      if (now->edges.count(ch) != 0) {
        now = now->edges[ch];
        token_buf.push_back(ch);
        continue;
      }
      if (now == start_) {
        PRTERROR;
      } else if (now == space_) {
      } else if (now == newline_) {
      } else if (now == id_or_keyword_) {
        if (token_buf == "define") { // FIXME : process define
          while (str[i] == ' ') ++i;
          size_t id_start = i;
          while (str[i] != ' ') ++i;
          size_t id_len = i - id_start;
          while (str[i] == ' ') ++i;
          size_t str_start = i;
          while (str[i] != '\n') ++i;
          size_t str_len = i - str_start;
          string define_str(str.substr(str_start, str_len));
          vector<string> result_temp;
          GetStringTokens(result_temp, define_str);
          define_map[str.substr(id_start, id_len)] = result_temp[0];
          func_token.push_back(encode_["define"]);
        } else if (define_map.count(token_buf)) {
          func_token.append(define_map[token_buf]);
        } else if (keywords_.count(token_buf)) {
          func_token.push_back(encode_[token_buf]);
        }
      } else if (now == const_num_) {
        func_token.push_back(encode_["KNUM_"]);
      } else if (now == const_char_) {
        PRTERROR;
      } else if (now == const_char2_) {
        func_token.push_back(encode_["KCHAR_"]);
      } else if (now == const_str_) {
        PRTERROR;
      } else if (now == const_str2_) {
        func_token.push_back(encode_["KSTR_"]);
      } else if (now == slash_) {
        func_token.push_back(encode_["/"]);
      } else if (now == line_comment_) {
        PRTERROR;
      } else if (now == line_comment2_) {
      } else if (now == block_comment_) {
        PRTERROR;
      } else if (now == block_comment2_) {
        PRTERROR;
      } else if (now == block_comment3_) {
      } else if (now == keep_char_) {
        if (token_buf == "{") {
            ++dep_cnt;
            if (dep_cnt == 1) {
                size_t j = i - 2;
                while (str[j] == ' ' || str[j] == '\n') --j;
                if (str[j] == ')') {
                    func_token.clear();
                    start_temp = i - 1;
                    while (str[j] != '(') --j;
                    while (!ISIDCHAR(str[j])) --j;
                    size_t k = j;
                    while (ISIDCHAR(str[j])) --j;
                    func_name = str.substr(j + 1, k - j);
                    in_func = true;
                }
            }
        } else if (token_buf == "}") {
            --dep_cnt;
            if (dep_cnt == 0 && in_func) {
                in_func = false;
                func_token.push_back('}');
                func2tokens[func_name] = func_token;
                func_pos[func_name] = pair<size_t, size_t>(start_temp, i - 1);
                func_token.clear();
            }
        }
        func_token.push_back(encode_[token_buf]);
      }
      now = start_->edges[ch];
      token_buf = ch;
    };
    return ;
}

void LexicalAnalyzer::PrintTokens(FILE* fp, const vector<string> &result) {
  for (auto &line : result) {
    for (char token : line) {
      for (char &ch : decode_[token]) {
        fputc(ch, fp);
      }
    }
    fputc('\n', fp);
  }
}

void LexicalAnalyzer::DecodeTokens(string &str, const vector<string> &result) {
  // FIXME : str multiple expend
  //cout << "result size : " << result.size() << "\n";
  str.clear();
  for (auto &line : result) {
    for (char token : line) {
      str += decode_[token];
      if (token < N_KEYWORDS) str += " ";
    }
    str += "\n";
  }
}

} // namespace kernel
} // namespace codeana
