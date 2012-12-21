
local hdf5 = require 'LuaHDF5'

local fname = arg[2]
local h5f = hdf5.File(fname, 'r')

for k,v in pairs(h5f) do
   print(k, v:value())
end
