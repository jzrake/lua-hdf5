
local buffer = require 'buffer'
local array = require 'array'

local b = buffer.new_buffer("the buffer data")

assert(#b == 15)
assert("the buffer data" == string.format("%s", b))

b[0] = 127
b[1] = 128

assert(b[0] == 127)
assert(b[1] == -128)

assert(not pcall(function() b[100] = 0 end))
local arr = buffer.new_buffer(array.sizeof('double')) -- just one double

assert(array.sizeof('char') == 1)
assert(array.sizeof('double') == 8)

array.set_typed(arr, 'double', 0, 1.5)
assert(array.get_typed(arr, 'double', 0) == 1.5)

local buf = buffer.new_buffer(100 * 8)
local bv = array.view(buf, 'double', {10,5,2})

assert(#bv == 100)
assert(bv._skip[1] == 10)
assert(bv._skip[2] == 2)
assert(bv._skip[3] == 1)

for i=0,9 do
   for j=0,4 do
      for k=0,1 do
	 bv[{i,j,k}] = i + j + k
      end
   end
end
for i=0,9 do
   for j=0,4 do
      for k=0,1 do
	 assert(bv[{i,j,k}] == i + j + k)
      end
   end
end

print(debug.getinfo(1).source, ": All tests passed")
