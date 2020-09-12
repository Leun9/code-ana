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
  FILE* fp_out = fopen("data/lex_ana_out.txt", "w");
  LexicalAnalyzer lex;
  lex.Init();
  auto result = lex.GetFileTokens(fp);
  lex.PrintToken(fp_out, result);
  return 0;
}