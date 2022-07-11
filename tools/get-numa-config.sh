#!/usr/bin/env bash

# SPDX-FileCopyrightText: Copyright (c) 2019-2021 Virginia Tech
# SPDX-License-Identifier: Apache-2.0
echo "#pragma once" > ./include/numa_config.h
python3 tools/cpu-topology.py >> ./include/numa_config.h
