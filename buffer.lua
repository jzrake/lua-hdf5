
local buffer = require 'buffer'
local view = require 'view'
local b = buffer.new_buffer("the buffer data")

assert(#b == 15)
assert("the buffer data" == string.format("%s", b))

b[0] = 127
b[1] = 128

assert(b[0] == 127)
assert(b[1] == -128)

assert(not pcall(function() b[100] = 0 end))
local arr = buffer.new_buffer(buffer.sizeof(buffer.double)) -- a single double

assert(buffer.sizeof(buffer.char) == 1)
assert(buffer.sizeof(buffer.double) == 8)

buffer.set_typed(arr, buffer.double, 0, 1.5)
assert(buffer.get_typed(arr, buffer.double, 0) == 1.5)

local buf = buffer.new_buffer(100 * 8)
local start = {0,0}
local size = {10,10}
local bv = view.view(buf, 'double', start, size)

assert(#bv == 100)
bv[99] = 1.5
assert(bv[99] == 1.5)

print(debug.getinfo(1).source, ": All tests passed")

