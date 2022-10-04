//
//  ../src/dont_optimize_away.cc
//  PROJECT ../src/dont_optimize_away
//
//  Created by zhenliu on 03/10/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#include "include/dont_optimize_away.h"
#include <stdio.h>
// putchar
#include <unistd.h>
// getpid
static auto pid = getpid();
void fake_reference(char *ptr) {
  if (pid == 0) {
    putchar(*ptr);
  }
}
