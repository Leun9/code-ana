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
size_t len_total;

int min_size = 6;

void test(FILE* fp0, FILE* fp1) {
  vector<string> src, dst;
  LexicalAnalyzer::GetFileTokens(src, fp0); src_pos.clear();
  LexicalAnalyzer::GetFileTokens(dst, fp1); dst_pos.clear();
  cout << src.size() << "\n";
  cout << dst.size() << "\n";
  CalcSimlarity(src, dst, min_size, src_pos, dst_pos, len, len_sum, len_total);
  for (int i = 0; i < src_pos.size(); ++i) cout << "SAME : " << src_pos[i] << " " << dst_pos[i] << " " << len[i] << "\n";
  cout << "SUMMARY : " << len_sum << " " << len_total << "\n\n";
}

int main () {
  LexicalAnalyzer::Init();
  FILE* fp0;
  FILE* fp1 = fopen("data/spn_1.cpp", "r");
  FILE* fp2 = fopen("data/spn_2.cpp", "r");
  FILE* fp3 = fopen("data/spn_3.cpp", "r");
  FILE* fp4 = fopen("data/spn_4.cpp", "r");
  fp0 = fopen("data/spn.cpp", "r"); test(fp0, fp1);
  fp0 = fopen("data/spn.cpp", "r"); test(fp0, fp2);
  fp0 = fopen("data/spn.cpp", "r"); test(fp0, fp3);
  fp0 = fopen("data/spn.cpp", "r"); test(fp0, fp4);
  return 0;
}