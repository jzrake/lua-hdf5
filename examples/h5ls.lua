
--------------------------------------------------------------------------------
-- Prints all the data sets and groups at the top level of a file
--------------------------------------------------------------------------------

local hdf5 = require 'LuaHDF5'

local function iterate_group(grp, indent)
   local indent = indent or ''
   for k,v in pairs(grp) do
      if v:class() == 'data set' then
	 if v:get_type():type_class() == 'string' then
	    print(indent..'Dataset: '..indent..k, v)
	 elseif v:get_type():type_class() == 'float' then
	    print(indent..'Dataset: '..indent..k, v)
	 end
      elseif v:class() == 'group' then
	 print(indent..'Group: '..v:name())
	 iterate_group(v, indent..'\t')
      end
   end
end

local fname = arg[2]

if not fname then
   print("usage: h5ls.lua infile.h5")
else
   local h5f = hdf5.File(fname, 'r')
   iterate_group(h5f)
   h5f:close()
end
