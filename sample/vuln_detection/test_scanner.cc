#include <cstdio>

bool v0;
int v1;
char v2[] = "hello\n";
short v3;
long v4;
long long v5;
double v6;
float v7;
unsigned long long v8;
char v9[100];

void f1() {
}

int f2() {

}

void _printf() {};

unsigned long long f3() {
  printf("");
  vprintf("");
  _printf();
}


int main(int argc, char *argv[]) {
  {int i; {long i; {short i;}}}
  return 0;
}