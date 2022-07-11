//
//  sequential_generator.h
//  PROJECT sequential_generator
//
//  Created by zhenliu on 07/07/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#pragma once

#include "generator.h"
#include <cstdint>

class SequentialGenerator : public Generator<std::uint64_t> {
  public:
  SequentialGenerator(std::uint64_t min, std::uint64_t max)
      : min_(min)
      , max_(max)
      , last_int_(min)
  {
    Next();
  }

  std::uint64_t Next();
  std::uint64_t Last();

  private:
  std::uint64_t min_;
  std::uint64_t max_;
  std::uint64_t last_int_;
};

inline std::uint64_t SequentialGenerator::Next()
{
  // if the last_int_ is larger than the max, it will go back to min
  last_int_++;
  if (last_int_ > max_)
    last_int_ = min_;
  return last_int_;
}

inline std::uint64_t SequentialGenerator::Last()
{
  return last_int_;
}
