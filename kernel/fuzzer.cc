#include "fuzzer.h"

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "func_scanner.h"
#include "kernel/util/type.h"

using std::string;
using std::vector;
using std::pair;
using std::list;
using std::unordered_map;
using std::unordered_set;
using namespace codeana::kernel::util;

namespace codeana {
namespace kernel {

string num2bar(int num) {
    string tmp("llllllll");
    for (int i = 0; i < 8; ++i) {
        if (num & 1) tmp[i] = 'I';
        num >>= 1;
    }
    return tmp;
}

void fuzzer(string &res, string &str_raw, bool value, bool func, bool newline, bool order) {
    res.clear();
    FuncInfos func_infos;
    ValueInfos global_values;
    GetFuncInfos(func_infos, global_values, str_raw);

    string str;
    if (order) {
        int i = 0;
        for (auto &f : func_infos) {
            if (f.name_ == "main") continue;
            str.append(str_raw.substr(i, f.start_-i));
            i = f.start_;
            while (str_raw[i] != '{') {str.push_back(str_raw[i]); ++i;}
            str.push_back(';');
            i = f.end_ + 1;
        }
        str.append(str_raw.substr(i, str_raw.size()-i));
        func_infos.reverse();
        for (auto &f : func_infos) {
            if (f.name_ == "main") continue;
            str.push_back('\n');
            str.push_back('\n');
            int start = f.start_-1;
            while (!ISIDCHAR(str_raw[start])) start--;
            while (ISIDCHAR(str_raw[start])) start--;
            str.append(str_raw.substr(start+1, f.end_-start));
        }

    }
    else {str = str_raw;}

    int num = 7;
    unordered_map<string, int> value2num;
    for (auto &v : global_values)
        if (value2num.count(v.name_) == 0) value2num[v.name_] = num++;
    for (auto &f : func_infos)
        for (auto &v : f.value_infos_)
            if (value2num.count(v.name_) == 0) value2num[v.name_] = num++;

    num = 128;
    unordered_map<string, int> func2num;
    for (auto &f : func_infos)
        if (f.name_ != "main" && func2num.count(f.name_) == 0) func2num[f.name_] = num++;

    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '/') {
            if (str[i-1] == '/')
                while (str[i] != '\n') {res.push_back(str[i]); ++i;}

        } else if (str[i] == '*') {
            if (str[i-1] == '/') {
                do {res.push_back(str[i]); ++i;} while (str[i] != '/' || str[i-1] != '*'); // 注释
            }

        } else if (str[i] == '\"') {
            do {res.push_back(str[i]); ++i;} while (str[i] != '\"' && str[i-1] != '\\');

        } else if (str[i] == '\'') {
            res.push_back(str[i++]);
            if (str[i+1] == '\\') res.push_back(str[i++]);
            res.push_back(str[i++]);

        } else if (ISIDCHAR(str[i])) {
            size_t start = i;
            ++i;
            while (ISIDCHAR(str[i])) ++i;
            string id = str.substr(start, i-start);
            if (value && value2num.count(id)) {
                res.append(num2bar(value2num[id]));
            } else if (func && func2num.count(id)) {
                res.append(num2bar(func2num[id]));
            } else {
                res.append(id);
            }
        }
        res.push_back(str[i]);
        if (str[i] == '\n' && newline) res.push_back('\n');
    }

}


} // namespace kernel
} // namespace codeana
