//
//  ../include/dont_optimize_away.h
//  PROJECT ../include/dont_optimize_away
//
//  Created by zhenliu on 03/10/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#pragma once

void fake_reference(char *ptr);
template <typename T>
inline void dont_optimize_away(T &&datum) {
  fake_reference(reinterpret_cast<char *>(&datum));
}
