
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

