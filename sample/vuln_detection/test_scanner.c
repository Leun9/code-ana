#include <stdio.h>
#include <string.h>
#include <stdlib.h>

_Bool v0; // bool 不为C的关键字
int v1;
char v2[] = "hustis\n";
wchar_t v3[4];
long v4;
long long v5;
double v6;
float v7;
unsigned long long v8;
char *v9[100];

void f1() { // 测试部分函数
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

int f2() { // 测试部分函数
  strcat(v2, v2);
  wcscat(v3, v3);
  strncat(v2, v2, 5);
  wcsncat(v3, v3, 5);
  gets(v2);
}

void _printf() { // 测试部分函数
  fread(v2, 8, 1, stdin);
  fread(v2, v1, 1, stdin);
  fread(v2, 8, 2, stdin);
};

unsigned long long f3() { // 测试部分函数
  printf("");
  //vprintf("");
  _printf();
  return 0;
}

void f4() { // 测试重名变量
  char arr[2];
  {
    memcpy(arr, v2, 2); // 安全
    char arr[1];
    memcpy(arr, v2, 2); // 不安全
    {
      memcpy(arr, v2, 2); // 不安全
      char arr[3]; 
      memcpy(arr, v2, 3); // 安全
    }
    memcpy(arr, v2, 3); // 不安全
    {
      memcpy(arr, v2, 1); // 安全
      char arr[2];
      memcpy(arr, v2, 2); // 安全
    }
  }
}

void f5() { // 测试堆溢出/栈溢出
  char str[10];
  char *p;
  memcpy(str, v2, 12);
  p = str;
  memcpy(p, v2, 12);
  p = (char*) malloc(10);
  memcpy(p, v2, 12);
  memcpy(p, v2, 10);

}

/*** test block comment ***/
int main(int argc, char *argv[]) {
  //gets();
  f1();
  char *i[10000] = {};
  {int i; {long i; {short i;}}}
  return 0;
}