
local buffer = require 'buffer'
local b = buffer.new_buffer("the buffer data")

assert(#b == 15)
assert("the buffer data" == string.format("%s", b))

b[0] = 127
b[1] = 128

assert(b[0] == 127)
assert(b[1] == -128)

print "All tests passed"

assert(not pcall(function() b[100] = 0 end))


local arr = buffer.new_buffer(buffer.sizeof(buffer.double)) -- a single double

assert(buffer.sizeof(buffer.char) == 1)
assert(buffer.sizeof(buffer.double) == 8)

buffer.set_typed(arr, buffer.double, 0, 1.5)
assert(buffer.get_typed(arr, buffer.double, 0) == 1.5)
