#include <cstdio>
#include <string>
#include <iostream>
#include <stack>
#include <cstring>
#include <cmath>

using namespace std;

inline bool better(char a, char b) {
  if (a == '(') return false;
  if (a == '^') return true;
  if (a == '*') return (b != '^');
  return (b == '+' || b == '-');
}

long long calc(long long x, long long y, char c) {
  if (c == '^') return pow(x, y);
  if (c == '*') return x*y;
  if (c == '-') return x-y;
  if (c == '+') return x+y;
  return 0;
}

long long eval(string nowexp, long long x) {
  if (nowexp[0] == '-') nowexp.insert(0, 1, '0');
  int i = nowexp.size() - 1;
  while (i--) {
    if (nowexp[i] != '(') continue;
    if (nowexp[i + 1] != '-') continue;
    nowexp.insert(i, 1, '0');
  }
  int nowexp_size = nowexp.size();
  stack<char> sc;
  stack<long long> si;
  i = 0;
  long long num;
  for (; i < nowexp_size; ) {
    if (nowexp[i] == '(') {
      sc.push('(');
      i++;
    }
    else if (nowexp[i] == ')') {
      num = si.top();
      si.pop();
      while (sc.top() != '(') {
        num = calc(si.top(), num, sc.top());
        si.pop();
        sc.pop();
      }
      si.push(num);
      sc.pop();
      i++;
    }
    else if (nowexp[i] == 'x'){
      si.push(x);
      i++;
    }
    else if (isdigit(nowexp[i])) {
      num = nowexp[i] - '0';
      i++;
      while (isdigit(nowexp[i])) {
        num *= 10;
        num += nowexp[i] - '0';
        i++;
      }
      si.push(num);
    }
    else {
      char c = nowexp[i];
      while (!sc.empty() && better(sc.top(), c)) {
        long long y = si.top();
        si.pop();
        long long x = si.top();
        si.pop();
        si.push(calc(x, y, sc.top()));
        sc.pop();
      }
      sc.push(c);
      i++;
    }
  }
  num = si.top();
  si.pop();
  while (!sc.empty()) {
    num = calc(si.top(), num, sc.top());
    si.pop();
    sc.pop();
  }
  return num;
}

int n;
string stdexp;
string nowexp;
long long res0;
long long res[10];
long long neg_res[10];

int main() {
  ios::sync_with_stdio(false);
  std::cin.tie(0);

  cin >> stdexp;
  res0 = eval(stdexp, 0);
  for (long long i = 1; i < 3; i++) {
    res[i] = eval(stdexp, i);
    neg_res[i] = eval(stdexp, -i);
    //cout << i << " " << res[i] << " " << neg_res[i] << endl;
  }
  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> nowexp;
    //cout << nowexp << endl;
    if (res0 != eval(nowexp, 0)) continue;
    int flag = 1;
    for (long long i = 1; i < 2; i++) {
      if (eval(nowexp, i) != res[i]) {
        flag = 0;
        //cout << nowexp << " " << i << " " << eval(nowexp, i) << " " << res[i] << endl;
        break;
      };
      if (eval(nowexp, -i) != neg_res[i]) {
        flag = 0;
        //cout << nowexp << " " << i << " " << eval(nowexp, -i) << " " << neg_res[i] << endl;
        break;
      };
    }
    if (flag == 1) cout << i << " ";
  }
  return 0;
}