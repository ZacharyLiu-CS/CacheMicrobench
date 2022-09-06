//
//  pmem_ops.h
//  PROJECT pmem_ops
//
//  Created by zhenliu on 21/06/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#pragma once

#include <gperftools/profiler.h>
#include <libpmem.h>

#define CACHE_LINE_SIZE 64

static inline void mfence() { asm volatile("mfence" ::: "memory"); }
__attribute__((unused))

static inline void
clflush(char *data, int len) {
  volatile char *ptr = (char *)((unsigned long)data & ~(CACHE_LINE_SIZE - 1));
  mfence();
  for (; ptr < data + len; ptr += CACHE_LINE_SIZE) {
    asm volatile(".byte 0x66; clflush %0" : "+m"(*(volatile char *)ptr));
  }
  mfence();
}
__attribute__((unused))
