#ifndef CODEANA_KERNEL_FUNC_SCANNER_H_
#define CODEANA_KERNEL_FUNC_SCANNER_H_

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <unordered_set>

using std::string;
using std::vector;
using std::pair;
using std::list;
using std::unordered_map;
using std::unordered_set;

namespace codeana {
namespace kernel {

enum {UNKNOWNPOS, STACK, HEAP};

static unordered_map<string, int> type2size({{"void", 0}, {"_Bool", 1}, {"char", 1}, {"wchar_t", 2}, {"short", 2}, {"int", 4}, {"long", 4}, {"float", 4}, {"long long", 8}, {"double", 4}}) ;
static unordered_set<string> integer({"short", "int", "long", "long long"});

struct ValueInfo {
    string name_;
    // 防止同名变量，变量名加作用域和深度可以唯一确定变量
    size_t start_;
    size_t end_;
    int deep_;

    //int pos_; // 0->global, 1->stack, 2->heap
    bool unsigned_;
    string type_;
    int size_;

    // for pointer or array
    bool is_pointer_;
    bool is_array_;
    size_t len_;
    int pos_;

    ValueInfo(string name, size_t start, size_t end, int deep, bool unsign, string type, int size,
        bool is_pointer, bool is_array, int len, int pos) :
        name_(name), start_(start), end_(end), deep_(deep), unsigned_(unsign), type_(type), size_(size),
        is_pointer_(is_pointer), is_array_(is_array), len_(len), pos_(pos) {};

};

typedef unordered_map<pair<string, int>, ValueInfo> Value2Info; // 扫描过程中维护的变量信息，扫描过程中深度和名称可以唯一确定变量
typedef list<ValueInfo> ValueInfos;

struct FuncInfo {
    string name_;
    size_t start_;
    size_t end_;
    ValueInfos value_infos_;
    FuncInfo() {};
    //FuncInfo(string name, size_t start, size_t end) : name_(name), start_(start), end_(end) {}
};

typedef list<FuncInfo> FuncInfos;

void GetFuncInfos(FuncInfos &func_infos, ValueInfos &global_value, string &str);

//void FuncScanner(string &func, size_t start, size_t end, ValueInfos&);


} // namespace kernel
} // namespace codeana


#endif // CODEANA_KERNEL_FUNC_SCANNER_H
