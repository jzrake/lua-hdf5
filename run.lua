
local H5 = require 'h5lua'
local buffer = require 'buffer'

local def = H5.H5P_DEFAULT
local trunc = H5.H5F_ACC_TRUNC
local double = H5.H5T_NATIVE_DOUBLE

local current_size = H5.new_hsize_t_arr{10,10,10}
local maximum_size = H5.new_hsize_t_arr{10,10,10}

local fid = H5.H5Fcreate("outfile.h5", trunc, def, def)
local sid = H5.H5Screate(H5.H5S_SIMPLE)
H5.H5Sset_extent_simple(sid, 3, current_size, maximum_size)

local did = H5.H5Dcreate2(fid, "dataset", double, sid, def, def, def)
local dif = H5.H5Dcreate2(fid, "fataset", double, sid, def, def, def)

H5.H5Dwrite(did, double, sid, sid, def, buffer.new_buffer(1000 * 8))

local idx = H5.new_hsize_t_arr{0}
local link_names = { }
local function iterfunc(name)
   table.insert(link_names, name)
end

H5.H5Literate(fid, H5.H5_INDEX_NAME, H5.H5_ITER_NATIVE, idx, iterfunc)
assert(#link_names == 2)
assert(link_names[1] == "dataset")
assert(link_names[2] == "fataset")
H5.H5Dclose(dif)

local info = H5.new_H5O_info_t()
H5.H5Oget_info(fid, info)
assert(info.fileno == 2)
assert(info.addr == 96)
assert(info.type == H5.H5O_TYPE_GROUP)
assert(info.rc == 1)
assert(info.ctime == 0)
assert(info.num_attrs == 0)

H5.H5Fclose(fid)
H5.H5Sclose(sid)
H5.H5Dclose(did)

local hs = H5.new_hsize_t_arr{12,16,24}
assert(hs[0] == 12)
assert(hs[1] == 16)
assert(hs[2] == 24)
hs[0] = 8
assert(hs[0] == 8)
assert(not pcall(function() hs[-1] = 8 end))
assert(not pcall(function() hs[3] = 8 end))

print "All tests passed"
