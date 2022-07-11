//
//  generator.h
//  PROJECT generator
//
//  Created by zhenliu on 07/07/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#pragma once

template <typename Value>
class Generator {
  public:
  virtual Value Next() = 0;
  virtual Value Last() = 0;
  virtual ~Generator() {}
};
