#!/bin/lua
require "opc"

local binary_path = "../build/basicop_perf"
-- workload type : 0 means zipfian
--                 1 means sequential
--                 2 means uniform
local exp_config = {
  workload_type = "2" ,
  operation_type = "read",
  io_size = "16",
  op_per_thread = 100000,
  debug = "true",
}

local threads_variables = {1, 2, 4, 8, 12, 16, 20, 24, 32, 36, 40 };

--------------------------------------------------------------------


for _, val in ipairs(threads_variables) do
  local commands = binary_path .. " "
  exp_config["thread_num"] = val
  for k, v in pairs(exp_config)do
    commands = commands .. "-" .. k .. "=" .. v .." "
  end

  local dir_name = opc.GetFileName()
  os.execute("mkdir -p " .. dir_name)
  local log_file_name = opc.GetOutputName(exp_config, dir_name)
  -- print(log_file_name)
  -- print(commands)
  opc.Capture(commands .. " > " .. log_file_name, true)
end


