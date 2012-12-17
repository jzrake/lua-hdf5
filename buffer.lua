
local buffer = require 'buffer'

local b = buffer.new_buffer("the buffer data")

assert(#b == 15)
assert("the buffer data" == string.format("%s", b))

b[0] = 127
b[1] = 128

assert(b[0] == 127)
assert(b[1] == -128)

assert(not pcall(function() b[100] = 0 end))
local arr = buffer.new_buffer(buffer.sizeof('double')) -- just one double

assert(buffer.sizeof('char') == 1)
assert(buffer.sizeof('double') == 8)

buffer.set_typed(arr, 'double', 0, 1.5)
assert(buffer.get_typed(arr, 'double', 0) == 1.5)

local buf = buffer.new_buffer(100 * 8)
local start = {0,0,0}
local size = {10,5,2}

local bv = buffer.view(buf, 'double', start, size)

assert(#bv == 100)
assert(bv._skip[1] == 10)
assert(bv._skip[2] == 2)
assert(bv._skip[3] == 1)

for i=0,99 do bv[i] = i end
assert(bv[{9,4,1}] == 99)

print(debug.getinfo(1).source, ": All tests passed")

