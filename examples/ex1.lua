
--------------------------------------------------------------------------------
-- Demonstrate the simplest use of the high-level HDF5 Lua bindings. Create an
-- HDF5 file with the following layout:
-- 
-- outfile.h5/
--
--   the_dataset (double array [16,32,64])
--
--   the_group/
--
--     the_message (string: "here is the message")
--
--------------------------------------------------------------------------------

local hdf5 = require 'LuaHDF5'
local array = require 'array'

local the_array = array.array({16,32,64}, 'double')
local the_h5file = hdf5.File("outfile.h5", "w")
local the_group = the_h5file:require_group("the_group")
the_group["the_message"] = "here is the message"
the_h5file["the_dataset"] = the_array
the_h5file:close()
