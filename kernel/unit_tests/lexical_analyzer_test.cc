#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "../lexical_analyzer.h"
#include "../lexical_analyzer.cc"

using namespace std;
using namespace codeana::kernel;

int main () {
  LexicalAnalyzer::Init();
  //LexicalAnalyzer::DecodeTokens(str, result);
  //cout << str;
  vector<string> result;
  string str("{if (x == 0) continue;}");
  string temp;
  LexicalAnalyzer::GetStringTokens(result, str);
  LexicalAnalyzer::DecodeTokens(temp, result);
  cout << temp;
  return 0;
}