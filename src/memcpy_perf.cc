//
//  memcpy_perf.c
//  PROJECT memcpy_perf
//
//  Created by zhenliu on 20/07/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#include <memory_ops.h>
#include <profiler.h>
#include <stdio.h>
#include <cstdint>
#include <cstring>

int main(int argc, char *argv[]) {
  std::uint8_t src[128];
  memset(src, 'a', 128);
  std::uint8_t dst[128];
  memset(dst, 0, 128);

  PointPorfiler pf;
  pf.start();
  for (auto i = 0; i < 10000; i++) rte_mov16(dst, src+ i%64);
  pf.end();

  PointPorfiler pf2;
  pf2.start();
  for (auto i = 0; i < 10000; i++) memcpy(dst, src + i % 64, 16);
  pf2.end();
  for (auto i = 0; i < 128; i++) {
    std::cout << dst[i] << ", ";
  }
  std::cout << std::endl;
  std::cout << "rte move latency:" << pf.duration() << std::endl;
  std::cout << "memcpy latency:" << pf2.duration() << std::endl;
  return 0;
}
