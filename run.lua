
--local buffer = require 'buffer'
local h5lua = require 'h5lua'

local def = h5lua.H5P_DEFAULT
local trunc = h5lua.H5F_ACC_TRUNC
local double = h5lua.H5T_NATIVE_DOUBLE
local fid = h5lua.H5Fcreate("outfile.h5", trunc, def, def)
local sid = h5lua.H5Screate(h5lua.H5S_SCALAR)
local did = h5lua.H5Dcreate(fid, "dataset", double, sid, def, def, def)

h5lua.H5Fclose(fid)
h5lua.H5Sclose(sid)
h5lua.H5Dclose(did)

local hs = h5lua.new_hsize_t_arr{12,16,24}
assert(hs[0] == 12)
assert(hs[1] == 16)
assert(hs[2] == 24)
hs[0] = 8
assert(hs[0] == 8)
assert(not pcall(function() hs[-1] = 8 end))
assert(not pcall(function() hs[3] = 8 end))
