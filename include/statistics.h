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
  ZIPF_WRITE = 0,
  ZIPF_READ,
  SEQ_WRITE,
  SEQ_READ,
  UNIF_WRITE,
  UNIF_READ,
  TICKER_ENUM_MAX
};

const std::vector<std::pair<PmemTickers, std::string>> PmemTickersNameMap = {
  {ZIPF_WRITE,  "microbench.zipfian.write"},
  {ZIPF_READ,   "microbench.zipfian.read"},
  {SEQ_WRITE,   "microbench.seq.write"},
  {SEQ_READ,    "microbench.seq.read"},
  {UNIF_WRITE,   "microbench.uniform.write"},
  {UNIF_READ,    "microbench.uniform.read"}
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
