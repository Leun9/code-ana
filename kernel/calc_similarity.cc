#include "calc_similarity.h"

#include <string>
#include <vector>
#include <iostream>
#include <QDebug>
#include <utility>

using std::string;
using std::vector;
using std::pair;

namespace codeana {
namespace kernel {

// 找到长度大于min_size的公共子串，其中min_size大于等于2
void CalcTokensSimlarity(vector<string> &src, vector<string> &dst, size_t min_size,  // min_size >= 2
                         vector<size_t> &src_pos, vector<size_t> &dst_pos, vector<size_t> &len) {
  size_t src_size = src.size();
  size_t dst_size = dst.size();
  vector<size_t> dp(src_size);
  for (size_t i = 0; i < dst_size; ++i) {
    //if (dst[i] == "") continue; // FIXME : 处理空行
    if (dp[src_size - 1] >= min_size) {
      //result.push_back(CommonSubStrInfo(src_size - dp[src_size-1], i - dp[src_size-1], dp[src_size-1]));
      src_pos.push_back(src_size - dp[src_size-1]);
      dst_pos.push_back(i - dp[src_size-1]);
      len.push_back(dp[src_size-1]);
    }
    for (size_t j = src_size - 1; j != 0; --j) {
      //if (src[j] == "") {dp[j] = dp[j-1]; continue;}// FIXME : 处理空行
      if (dst[i] == src[j]) {
        dp[j] = dp[j - 1] + 1;
      } else {
        dp[j] = 0;
        if (dp[j - 1] >= min_size) {
          //qDebug() << j;
          //result.push_back(CommonSubStrInfo(j - dp[j-1], i - dp[j-1], dp[j-1]));
          src_pos.push_back(j - dp[j-1]);
          dst_pos.push_back(i - dp[j-1]);
          len.push_back(dp[j-1]);
        }
      }
    }
    if (dst[i] == src[0]) {
      dp[0] = 1;
    } else {
      dp[0] = 0;
    }
  }
  for (size_t i = src_size - 1; i > 0; --i) {
    if (dp[i] >= min_size)  {
      //result.push_back(CommonSubStrInfo(i - dp[i] + 1, dst_size - dp[i], dp[i]));
      src_pos.push_back(i - dp[i] + 1);
      dst_pos.push_back(dst_size - dp[i]);
      len.push_back(dp[i]);
    }
  }


  // 后处理结果，排序并使相似代码块不重叠。
  vector<pair<pair<size_t, size_t>, size_t>> temp;
  for (size_t i = 0; i < dst_pos.size(); ++i) {
      int start = dst_pos[i];
      int end = start + len[i] - 1;
      int llen = len[i];
      while (end > start && dst[end--].empty()) llen--;
      while (start < end && dst[start++].empty()) llen--;
      if (llen < min_size) continue;
      temp.push_back(pair< pair<size_t, size_t>, size_t>(pair<size_t, size_t>(dst_pos[i], -len[i]), i));

    }
  sort(temp.begin(), temp.end());
  auto src_pos_temp = src_pos;
  auto dst_pos_temp = dst_pos;
  auto len_temp = len;
  src_pos.clear();
  dst_pos.clear();
  len.clear();
  size_t last_end = 0;
  for (auto &tempi : temp) {
      //qDebug() << tempi.first << tempi.second;
      size_t i = tempi.second;
      size_t end = dst_pos_temp[i] + len_temp[i];
      if (last_end > dst_pos_temp[i]) {
          if (last_end >= end) continue;
          src_pos.push_back(last_end - dst_pos_temp[i]);
          dst_pos.push_back(last_end);
          len.push_back(end - dst_pos_temp[i]);
      }
      src_pos.push_back(src_pos_temp[i]);
      dst_pos.push_back(dst_pos_temp[i]);
      len.push_back(len_temp[i]);
      last_end = end;
  }
  //qDebug() << len.size();
  //qDebug() << len[0];
  return ;
}

void CalcStringSimlarity(string &src, string &dst, size_t min_size,  // min_size >= 2
                         vector<size_t> &src_pos, vector<size_t> &dst_pos, vector<size_t> &len) {
    size_t src_size = src.size();
    size_t dst_size = dst.size();
    vector<size_t> dp(src_size);
    for (size_t i = 0; i < dst_size; ++i) {
      //if (dst[i] == "") continue; // FIXME : 处理空行
      if (dp[src_size - 1] >= min_size) {
        //result.push_back(CommonSubStrInfo(src_size - dp[src_size-1], i - dp[src_size-1], dp[src_size-1]));
        src_pos.push_back(src_size - dp[src_size-1]);
        dst_pos.push_back(i - dp[src_size-1]);
        len.push_back(dp[src_size-1]);
      }
      for (size_t j = src_size - 1; j != 0; --j) {
        //if (src[j] == "") { // FIXME : 处理空行
        if (dst[i] == src[j]) {
          dp[j] = dp[j - 1] + 1;
        } else {
          dp[j] = 0;
          if (dp[j - 1] >= min_size) {
            //qDebug() << j;
            //result.push_back(CommonSubStrInfo(j - dp[j-1], i - dp[j-1], dp[j-1]));
            src_pos.push_back(j - dp[j-1]);
            dst_pos.push_back(i - dp[j-1]);
            len.push_back(dp[j-1]);
          }
        }
      }
      if (dst[i] == src[0]) {
        dp[0] = 1;
      } else {
        dp[0] = 0;
      }
    }
    for (size_t i = src_size - 1; i > 0; --i) {
      if (dp[i] >= min_size)  {
        //result.push_back(CommonSubStrInfo(i - dp[i] + 1, dst_size - dp[i], dp[i]));
        src_pos.push_back(i - dp[i] + 1);
        dst_pos.push_back(dst_size - dp[i]);
        len.push_back(dp[i]);
      }
    }


    // 后处理结果，排序并使相似代码块不重叠。
    vector<pair<pair<size_t, size_t>, size_t>> temp;
    for (size_t i = 0; i < dst_pos.size(); ++i)
        temp.push_back(pair< pair<size_t, size_t>, size_t>(pair<size_t, size_t>(dst_pos[i], -len[i]), i));
    sort(temp.begin(), temp.end());
    auto src_pos_temp = src_pos;
    auto dst_pos_temp = dst_pos;
    auto len_temp = len;
    src_pos.clear();
    dst_pos.clear();
    len.clear();
    size_t last_end = 0;
    for (auto &tempi : temp) {
        //qDebug() << tempi.first << tempi.second;
        size_t i = tempi.second;
        size_t end = dst_pos_temp[i] + len_temp[i];
        if (last_end > dst_pos_temp[i]) {
            if (last_end >= end) continue;
            src_pos.push_back(last_end - dst_pos_temp[i]);
            dst_pos.push_back(last_end);
            len.push_back(end - dst_pos_temp[i]);
        }
        src_pos.push_back(src_pos_temp[i]);
        dst_pos.push_back(dst_pos_temp[i]);
        len.push_back(len_temp[i]);
        last_end = end;
    }
    //qDebug() << len.size();
    //qDebug() << len[0];
    return ;
}

} // namespace kernel
} // namespace codeana
