//
//  logging.h
//  PROJECT logging
//
//  Created by zhenliu on 21/06/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#pragma once
#include <iostream>
#include <gflags/gflags.h>


DEFINE_bool(debug, false, "Set if output log message");

#define LOG(...) if (FLAGS_debug)LogWrapper(__FILE__, "line:", __LINE__, "\n", __VA_ARGS__)
inline void LogWrapper() { std::cout << std::endl; }

template <typename First, typename... Rest>
inline void LogWrapper(First&& first, Rest&&... rest)
{
  std::cout << std::forward<First>(first) << " ";
  LogWrapper(std::forward<Rest>(rest)...);
}
