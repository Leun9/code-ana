#include <cstdio>
#include <cstring>

bool v0;
int v1;
char v2[] = "hustis\n";
wchar_t v3[4];
long v4;
long long v5;
double v6;
float v7;
unsigned long long v8;
char *v9[100];

void f1() {
  strcpy(v2, "hustis\n");
  strcpy(v2, "hustcse\n");
  strcpy(v2, v2);
  strncpy(v2, "hustcse\n", 10);
  strncpy(v2, "hustcse\n", 8);
  wcscpy(v3, L"abc");
  wcscpy(v3, L"abcd");
  wcscpy(v3, v3);
  wcsncpy(v3, L"abcd", 5);
  wcsncpy(v3, L"abcd", 4);
  //printf("[INFO] %ls\n", v3);
  memset(v3, 0, 9);
  memset(v3, 0, v1);
  memset(v3, 0, 8);
  memcpy(v3, L"abcd", 9);
  memcpy(v3, L"abcd", v1);
  memcpy(v3, L"abcd", 8);
  //printf("[INFO] %ls\n", v3);
}

int f2() {
  strcat(v2, v2, 5);
  wcscat(v3, v3, 5);
  strcat(v2, v2, 5);
  wcscat(v3, v3, 5);
  gets(v2);
}

void _printf() {
  fread(v2, 8, 1, stdin);
};

unsigned long long f3() {
  printf("");
  //vprintf("");
  _printf();
  return 0;
}


int main(int argc, char *argv[]) {
  //gets();
  f1();
  char *i[10000] = {};
  {int i; {long i; {short i;}}}
  return 0;
}