#include "func_scanner.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <QDebug>
#include "util/type.h"

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using codeana::kernel::util::ISBLANK;
using codeana::kernel::util::ISIDCHAR;


namespace codeana {
namespace kernel {

#define PRTERROR do {fprintf(stderr, "[File:%s][Line:%d]Func Scan Error.\n", __FILE__, __LINE__);} while(0);

unordered_map<string, int> type2size({{"void", 0}, {"bool", 1}, {"char", 1}, {"wchar_t", 2}, {"short", 2}, {"int", 4}, {"long", 4}, {"float", 4}, {"long long", 8}, {"double", 4}}) ;
unordered_set<string> integer({"short", "int", "long", "long long"});

void GetFuncInfos(FuncInfos &func_infos, ValueInfos &global_values, string &str) {
    func_infos.clear();
    global_values.clear();

    auto code_size = str.size();
    int deep = 0;
    bool in_func = 0;
    ValueInfos func_values;
    FuncInfo func_info;

    for (size_t i = 0; i < code_size; ++i) {

        if (str[i] == '/') {
            if (str[i-1] == '/')
                while (str[i] != '\n') ++i;

        } else if (str[i] == '*') {
            if (str[i-1] == '/')
                do {++i;} while (str[i] != '/' || str[i-1] != '*');

        } else if (str[i] == '\"') {
            do {++i;} while (str[i] != '\"' && str[i-1] != '\\');

        } else if (str[i] == '{') { // FIXME : 未考虑注释的特殊情况
            ++deep;

        } else if (str[i] == '}') { // 压入函数信息
            --deep;
            //qDebug() << "deep" << deep;
            // 处理变量的end
            for (auto it = func_info.value_infos_.rbegin(); it != func_info.value_infos_.rend(); ++it) {
                //qDebug() << it->name_.c_str() << it->deep_;
                if (it->deep_ == deep) break;
                if (it->deep_ == deep + 1) it->end_ = i;
            }

            if (deep == 0 && in_func) {
                in_func = 0;
                func_info.end_ = i;
                func_infos.push_back(func_info);
                // 弹出局部变量
                while (func_info.value_infos_.size() && func_info.value_infos_.back().deep_ > 0)
                    func_info.value_infos_.pop_back();
            }

        } else {
            // 找变量 : "类型 标识符;" 或 "类型 标识符 = ;"
            if (!ISIDCHAR(str[i])) continue;
            size_t j = i;
            while (ISIDCHAR(str[i])) ++i;
            string token(str.substr(j, i - j));
            if (type2size.find(token) == type2size.end()) continue;
            if (token == "long") { // 判long long
                size_t j = i;
                while (ISBLANK(str[j])) ++j;
                if (str[j] == 'l' && str[++j] == 'o' && str[++j] == 'n' && str[++j] == 'g') {
                    ++j;
                    if (!ISIDCHAR(str[j])) {
                        token += " long";
                        i = j;
                    }
                }
            }
            string type(token);
            int size = type2size[type];
            bool unsign = 0;
            if (integer.find(token) != integer.end()) {
                do {--j;} while (ISBLANK(str[j]));
                if (str.substr(j-7, 8) == "unsigned") {
                    type = "unsigned " + type;
                    unsign = 1;
                }
            }
            while (true) {
                bool isp = 0;
                bool isa = 0;
                while (ISBLANK(str[i])) ++i;
                if (str[i] == '*') {isp = 1; do {++i;} while (ISBLANK(str[i]));}
                size_t start = i;
                while (ISIDCHAR(str[i])) ++i;
                string name(str.substr(start, i - start));
                while (ISBLANK(str[i])) ++i;
                if (str[i] == '(') {
/*** 函数声明 or 实现 ***/
if (deep == 0)  {
    func_info.name_ = name;
    func_info.start_ = start; // FIXME : start
    do {++i;} while (ISBLANK(str[i]));
    if (str[i] != ')') {
        while (true) {
            while (!ISIDCHAR(str[i])) ++i;
            size_t type_start = i;
            do {++i;} while (str[i] != ',' && str[i] != ')');
            size_t v_end = i;
            while (!ISIDCHAR(str[v_end])) {v_end--; if (str[v_end] == ']') isa = 1;}
            size_t v_start = v_end;
            while (ISIDCHAR(str[v_start])) v_start--;
            size_t start = v_start + 1;
            name = str.substr(start, v_end - v_start);
            while (ISBLANK(str[v_start])) v_start--;
            if (str[v_start] == '*') {isp = 1; do {--v_start;} while (ISBLANK(str[v_start]));}
            type = str.substr(type_start, v_start - type_start + 1);

            // std::cout << "[" << type << "] " << name << "\n";
            func_info.value_infos_.push_back(ValueInfo(name, start, code_size-1, 1, unsign, type, size, isp, isa, 0));
            if (str[i] == ')') break;
        }
    }
};
do {++i;} while (ISBLANK(str[i]));
if (str[i] == '{') { // 实现
    ++deep;
    in_func = 1;
} else { // 否则是声明
    while (func_info.value_infos_.back().deep_ == 1) func_info.value_infos_.pop_back();
}
/*** end 处理函数声明 or 实现 ***/
                    break;
                }
                size_t len = 0;
                if (str[i] == '[') { // 得到长度
                    isa = 1;
                    do {++i;} while (ISBLANK(str[i]));
                    if (str[i] == ']') { // []
                        while (str[i] != '=') ++i;
                        do {++i;} while (str[i] != '"' && str[i] != '{');
                        if (str[i] == '{') {
                            while (str[i] != '}') {if (str[i] == ',') ++len; ++i;}
                            ++len;
                        } else { // str[i] == " , 字符串常量
                            do {++i; if (str[i] == '\\') ++i; ++len;} while (str[i] != '"');
                            ++i;
                        }
                    } else { // [len]
                        while (str[i] >= '0' && str[i] <= '9') {len *= 10; len += str[i] - '0'; ++i;};
                        while (ISBLANK(str[i])) ++i;
                    }
                }

                // std::cout << "[" << type << "] " << name << "\n";
                // std::cout << i << str[i] << "\n";

                size_t end = code_size - 1; // 全局变量：end = str_size - 1
                func_info.value_infos_.push_back(ValueInfo(name, start, end, deep, unsign, type, size, isp, isa, len));
                while (str[i] != ',' && str[i] != ';' && str[i] != '\"' && str[i] != '{') ++i;
                if (str[i] == '{') {
                    do {++i;} while (str[i] != '}');
                    while (str[i] != ',' && str[i] != ';') ++i;
                } else if (str[i] == '\"') {
                    do {++i;} while (str[i] != '\"');
                    while (str[i] != ',' && str[i] != ';') ++i;
                }
                if (str[i] == ';') break;
                ++i; // str[i] = ','
            }
        }
    }
    global_values = func_info.value_infos_;
}

} // namespace kernel
} // namespace codeana

// int main() {
//     using namespace codeana::kernel;
//     using namespace std;
//     FuncInfos func_infos;
//     ValueInfos global_values;
//     string str;
//     //auto fp = fopen("../sample/homology_detection/spn/spn.cpp", "r");
//     //auto fp = fopen("../sample/vuln_detection/test_scanner.cc", "r");
//     // std::cout << "fopen : " << fp << "\n";
//     while (true) {
//         char ch = fgetc(fp);
//         if (ch == EOF) break;
//         str.push_back(ch);
//     }
//     //cout << str;
//     GetFuncInfos(func_infos, global_values, str);

//     return 0;

//     // cout << "\nINFO:\n";
//     // auto values = func_infos[0].value_infos_ ;
//     // cout << "VSize : " << values.size() << "\n";
//     // for (auto v : values) {
//     //     cout << v.name_ << " : " << v.type_ << "\n";
//     // }
//     return 0;
// }

