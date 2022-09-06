//
//  statistics.h
//  PROJECT statistics
//
//  Created by zhenliu on 23/06/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "histogram.h"

enum PmemTickers : uint32_t {
  ZIPF = 0,
  SEQ,
  UNIF,
  TICKER_ENUM_MAX
};

const std::vector<std::pair<PmemTickers, std::string>> PmemTickersNameMap = {
  {ZIPF,   "microbench.zipfian"},
  {SEQ,    "microbench.seq"},
  {UNIF,    "microbench.uniform"}
};

enum SizeUnit : uint32_t{
  h_gigabytes = 1UL << 30,
  h_megabytes = 1UL << 20,
  h_kilobytes = 1UL << 10
};

inline double calculate_ops(int32_t threads_num, utils::Histogram &histogram) {
  auto latency_base = histogram.sum_ / threads_num;
  double ops = histogram.num_ / (latency_base / histogram.record_unit_);
  return ops;
}

inline double calculate_thpt(int32_t io_size, int32_t threads_num,
                                 utils::Histogram &histogram) {
  return io_size * calculate_ops(threads_num, histogram);
}
