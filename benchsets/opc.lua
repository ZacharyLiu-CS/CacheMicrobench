opc = {}

function opc.GetFileName()
  local str = debug.getinfo(2, "S").source:sub(2)
  return str:match("^.*/(.*).lua$") or str
end

function opc.GetOutputName(exp_config, dir_name)
  local output_file_name = "./" .. dir_name .. "/" .. dir_name .. "@thread_num@" .. exp_config["thread_num"] ..".log"
  return output_file_name
end


function opc.Capture(cmd, raw)
  local f = assert(io.popen(cmd, 'r'))
  local s = assert(f:read('*a'))
  f:close()
  if raw then return s end
  s = string.gsub(s, '^%s+', '')
  s = string.gsub(s, '%s+$', '')
  s = string.gsub(s, '[\n\r]+', ' ')
  return s
end


