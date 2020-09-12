#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "../lexical_analyzer.h"
#include "../lexical_analyzer.cc"
#include "../calc_similarity.h"
#include "../calc_similarity.cc"

using namespace std;
using namespace codeana::kernel;

vector<string> src;
vector<string> dst;
vector<size_t> src_pos;
vector<size_t> dst_pos;
vector<size_t> len;
size_t len_sum;

int min_size = 6;

int main () {
  FILE* fp0 = fopen("data/spn.cpp", "r");
  FILE* fp1 = fopen("data/spn_change_id.cpp", "r");
  LexicalAnalyzer lex;
  lex.Init();
  src = lex.GetFileTokens(fp0);
  dst = lex.GetFileTokens(fp1);
  cout << src.size() << "\n";
  cout << dst.size() << "\n";
  CalcSimlarity(src, dst, min_size, src_pos, dst_pos, len, len_sum);
  for (int i = 0; i < src_pos.size(); ++i) cout << src_pos[i] << " " << dst_pos[i] << " " << len[i] << "\n";
  cout << len_sum << "\n";
  return 0;
}