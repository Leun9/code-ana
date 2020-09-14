#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "../lexical_analyzer.h"
#include "../lexical_analyzer.cc"

using namespace std;
using namespace codeana::kernel;

int main () {
  FILE* fp = fopen("data/spn.cpp", "r");
  FILE* fp_out = fopen("data/spn_lex.txt", "w");
  LexicalAnalyzer lex;
  lex.Init();
  vector<string> result;
  LexicalAnalyzer::GetFileTokens(result, fp);
  LexicalAnalyzer::PrintTokens(fp_out, result);
  string str;
  //LexicalAnalyzer::DecodeTokens(str, result);
  //cout << str;
  string str_("{if (x == 0) continue;}");
  string temp;
  LexicalAnalyzer::GetStringTokens(result, str_);
  LexicalAnalyzer::DecodeTokens(temp, result);
  cout << temp;
  return 0;
}