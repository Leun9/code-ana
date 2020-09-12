#include <cstdio>
#include <iostream>
#include <string>

#include "../calc_similarity.h"
#include "../calc_similarity.cc"

using namespace std;
using namespace codeana::kernel;

string string1("a");
string string2("b");
string string3("c");
string string4("d");

vector<string> src({string1, string2, string3});
vector<string> dst({string1, string2, string3, string1, string2, string3, string1, string2, string2, string3});
vector<size_t> src_pos;
vector<size_t> dst_pos;
vector<size_t> len;
size_t len_sum;

int main () {
  int min_size = 2;
  CalcSimlarity(src, dst, min_size, src_pos, dst_pos, len, len_sum);
  for (int i = 0; i < src_pos.size(); ++i) cout << src_pos[i] << " " << dst_pos[i] << " " << len[i] << "\n";
  cout << len_sum << "\n";
  return 0;
}