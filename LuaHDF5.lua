
--------------------------------------------------------------------------------
--
--               High level Lua wrappers for HDF5 library
--
--------------------------------------------------------------------------------
local hdf5 = { } -- module table
--------------------------------------------------------------------------------

local H5 = require 'HDF5'
local buffer = require 'buffer'
local array = require 'array'
local hp0 = H5.H5P_DEFAULT


--------------------------------------------------------------------------------
-- Utility functions
--------------------------------------------------------------------------------
local function printf(...)
   print(string.format(...))
end

local function inherit_from(base, derived)
   -- Basically just deep-copy the base class table --
   local new = derived or { }
   for k,v in pairs(base) do new[k] = v end
   return new
end
local function class(object)
   if type(object) == 'table' then
      return object._type
   else
      return nil
   end
end


--------------------------------------------------------------------------------
-- Base classes for meta-table and methods wrapping hid_t objects
--------------------------------------------------------------------------------
local BaseMeta = { }
function BaseMeta:__index()
   if self._hid == 0 then -- object is closed
      return nil
   end
end
function BaseMeta:__newindex()
   if self._hid == 0 then -- object is closed
      error("cannot assign to closed object")
   end
end
function BaseMeta:__gc()
   self:close()
end

local BaseClass = { }
function BaseClass:close()
   if self._open_objects then
      for k,v in pairs(self._open_objects) do
	 v:close()
	 self._open_objects[k] = nil
      end
   end
   if self._parent then
      self._parent._open_objects[self._name] = nil
   end
   if self._hid ~= 0 then
      self._close(self._hid)
      self._hid = 0
   else
      -- object is already closed
   end
end


--------------------------------------------------------------------------------
-- IndexableClass methods
--------------------------------------------------------------------------------
local IndexableClass = inherit_from(BaseClass)
function IndexableClass:path(key)
   if not self._parent then
      return '/' .. self._name
   else
      return self._parent:path() .. '/' .. self._name
   end
end
function IndexableClass:keys()
   local link_names = { }
   local function f(name)
      table.insert(link_names, name)
   end
   local idx = H5.new_hsize_t_arr{0}
   H5.H5Literate(self._hid, H5.H5_INDEX_NAME, H5.H5_ITER_NATIVE, idx, f)
   return link_names
end
function IndexableClass:require_group(name)
   return hdf5.Group(self, name)
end


--------------------------------------------------------------------------------
-- IndexableMeta methods
--------------------------------------------------------------------------------
local IndexableMeta = inherit_from(BaseMeta)
function IndexableMeta:__index(key)
   if self._hid == 0 then -- object is closed
      return nil
   end
   if self._open_objects[key] then
      return self._open_objects[key]
   end
   if not H5.H5Lexists(self._hid, key, hp0) then
      return nil
   end
   local info = H5.new_H5O_info_t()
   H5.H5Oget_info_by_name(self._hid, key, info, hp0)
   if info.type == H5.H5O_TYPE_GROUP then
      return hdf5.Group(self, key)
   elseif info.type == H5.H5O_TYPE_DATASET then
      return hdf5.DataSet(self, key)
   else
      error("object %s/%s has an unsupported type", self._name, key)
   end
end
function IndexableMeta:__newindex(key, value)
   if self._hid == 0 then -- object is closed
      error("cannot assign to closed object")
   end
   if type(value) == 'string' then
      local targ = self._hid
      local data = buffer.new_buffer(value)

      local fspc = H5.H5Screate(H5.H5S_SCALAR)
      local strn = H5.H5Tcopy(H5.H5T_C_S1)
      H5.H5Tset_size(strn, #data)
      local dset = H5.H5Dcreate2(targ, key, strn, fspc, hp0, hp0, hp0)

      H5.H5Dwrite(dset, strn, fspc, fspc, hp0, data)
      H5.H5Dclose(dset)
      H5.H5Tclose(strn)
      H5.H5Sclose(fspc)
   elseif value.buffer and value.dtype and value.selection then
      --------------------------------------------------------------------------
      -- If the buffer, dtype, and selection methods are given, assume their
      -- behavior is like that of array.view, and we can write to a buffer
      -- automatically.
      --------------------------------------------------------------------------
      local start, stride, count, block = value:selection()
      local mspc = hdf5.DataSpace()
      mspc:set_extent(count)
      mspc:select_hyperslab(start, stride, count, block)
      local dtyp = hdf5.DataType(value:dtype())
      local dset = hdf5.DataSet(self, key, mspc, dtyp)
      dset:write(value:buffer())
   else
      error("unrecognized type for writing")
   end
end
function IndexableMeta:__pairs()
   local p = { }
   for k,v in pairs(self:keys()) do
      p[v] = self[v]
   end
   return pairs(p)
end


--------------------------------------------------------------------------------
-- HDF5 File class methods
--------------------------------------------------------------------------------
local FileClass = inherit_from(IndexableClass)

local FileMeta = inherit_from(IndexableMeta)
function FileMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 file: \"%s\" (mode %s)>", self._name,
			   self._mode)
   else
      return "<Closed HDF5 file>"
   end
end


--------------------------------------------------------------------------------
-- HDF5 Group class methods
--------------------------------------------------------------------------------
local GroupClass = inherit_from(IndexableClass)

local GroupMeta = inherit_from(IndexableMeta)
function GroupMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 group: \"%s\">", self._name)
   else
      return "<Closed HDF5 group>"
   end
end


--------------------------------------------------------------------------------
-- HDF5 DataSet class methods
--------------------------------------------------------------------------------
local DataSetClass = inherit_from(BaseClass)

function DataSetClass:write(buf)
   local spc = H5.H5Dget_space(self._hid)
   local typ = H5.H5Dget_type(self._hid)
   local siz = H5.H5Tget_size(typ)
   if H5.H5Sget_select_npoints(spc) * siz > #buf then
      error("data space selection is too large for buffer")
   end
   H5.H5Dwrite(self._hid, typ, spc, spc, hp0, buf)
   H5.H5Sclose(spc)
   H5.H5Tclose(typ)
end

function DataSetClass:read()
   -----------------------------------------------------------------------------
   -- Read all internal data into an un-typed data buffer. Always works.
   -----------------------------------------------------------------------------
   local spc = H5.H5Dget_space(self._hid)
   local typ = H5.H5Dget_type(self._hid)
   local siz = H5.H5Tget_size(typ)
   local bytes = H5.H5Sget_select_npoints(spc) * siz
   local buf = buffer.new_buffer(bytes)
   H5.H5Dread(self._hid, typ, spc, spc, hp0, buf)
   H5.H5Sclose(spc)
   H5.H5Tclose(typ)
   return buf
end

function DataSetClass:value()
   -----------------------------------------------------------------------------
   -- Read internal data into an object whose Lua type is inferred from the HDF5
   -- type. Might throw an error if it can't find an appropriate Lua type.
   -----------------------------------------------------------------------------
   local space = self:get_space()
   local start = { }
   local size = space:get_extent()
   local tid1 = H5.H5Dget_type(self._hid)
   local tcls = H5.H5Tget_class(tid1)

   for i,v in ipairs(size) do start[i] = 0 end
   local ret
   if false then
      ret = nil -- just to align text
   elseif tcls == H5.H5T_STRING then
      ret = tostring(self:read())
   elseif H5.H5Tequal(tid1, H5.H5T_NATIVE_CHAR) then
      ret = array.view(self:read(), 'char', start, size)
   elseif H5.H5Tequal(tid1, H5.H5T_NATIVE_INT) then
      ret = array.view(self:read(), 'int', start, size)
   elseif H5.H5Tequal(tid1, H5.H5T_NATIVE_FLOAT) then
      ret = array.view(self:read(), 'float', start, size)
   elseif H5.H5Tequal(tid1, H5.H5T_NATIVE_DOUBLE) then
      ret = array.view(self:read(), 'double', start, size)
   else
      print("could not infer a Lua type from the data set")
   end

   H5.H5Tclose(tid1)
   return ret
end
function DataSetClass:get_space()
   return hdf5.DataSpace(self)
end

function DataSetClass:get_type()
   -----------------------------------------------------------------------------
   -- Return a DataType class representing the DataSet's HDF5 type.
   -----------------------------------------------------------------------------
   local typ = H5.H5Dget_type(self._hid)
   local ret = hdf5.DataType(typ) -- a copy of typ is made
   H5.H5Tclose(typ)
   return ret
end

local DataSetMeta = inherit_from(BaseMeta)
function DataSetMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 data set: \"%s\">", self._name)
   else
      return "<Closed HDF5 data set>"
   end
end


--------------------------------------------------------------------------------
-- HDF5 DataType class methods
--------------------------------------------------------------------------------
local DataTypeClass = inherit_from(BaseClass)
function DataTypeClass:get_size()
   return H5.H5Tget_size(self._hid)
end
function DataTypeClass:set_size(size)
   return H5.H5Tset_size(self._hid, size)
end

local DataTypeMeta = inherit_from(BaseMeta)
function DataTypeMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 data type: \"%s\">", self._name)
   else
      return "<Closed HDF5 data type>"
   end
end


--------------------------------------------------------------------------------
-- HDF5 DataSpace class methods
--------------------------------------------------------------------------------
local DataSpaceClass = inherit_from(BaseClass)
function DataSpaceClass:get_extent(what)
   if self._hid == 0 then
      error("DataSpace:get_extent cannot operate on closed data space")
   end
   local rank = H5.H5Sget_simple_extent_ndims(self._hid)
   local csize = { }
   local msize = { }
   for i=1,rank do
      csize[i] = 0
      msize[i] = 0
   end
   local current_size = H5.new_hsize_t_arr(csize)
   local maximum_size = H5.new_hsize_t_arr(msize)
   H5.H5Sget_simple_extent_dims(self._hid, current_size, maximum_size)
   for i=1,rank do
      csize[i] = current_size[i-1]
      msize[i] = maximum_size[i-1]
   end
   return ({current=csize, maximum=msize})[what] or csize
end
function DataSpaceClass:get_select_npoints()
   return H5.H5Sget_select_npoints(self._hid)
end
function DataSpaceClass:set_extent(extent)
   if self._hid == 0 then
      error("DataSpace:set_extent cannot operate on closed data space")
   end
   local current_size = H5.new_hsize_t_arr(extent)
   local maximum_size = H5.new_hsize_t_arr(extent)
   local err = H5.H5Sset_extent_simple(self._hid, #extent, current_size,
				       maximum_size)
   if #err < 0 then error("DataSpace:set_extent") end
end
function DataSpaceClass:select_hyperslab(start, stride, count, block)
   if self._hid == 0 then
      error("DataSpace:select_hyperslab cannot operate on closed data space")
   end
   local hstart = H5.new_hsize_t_arr(start)
   local hstride = H5.new_hsize_t_arr(stride)
   local hcount = H5.new_hsize_t_arr(count)
   local hblock = H5.new_hsize_t_arr(block)
   local err = H5.H5Sselect_hyperslab(self._hid, H5.H5S_SELECT_SET, hstart, 
				      hstride, hcount, hblock)
   if #err < 0 then error("DataSpace:select_hyperslab") end
end
local DataSpaceMeta = inherit_from(BaseMeta)
function DataSpaceMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 data space: [%s]>",
			   table.concat(self:get_extent(), " "))
   else
      return "<Closed HDF5 data space>"
   end
end


--------------------------------------------------------------------------------
-- HDF5 File constructor
--------------------------------------------------------------------------------
function hdf5.File(name, mode)
   local new = { _type='file',
		 _name=name,
		 _mode=mode,
		 _hid=0,
		 _close=H5.H5Fclose,
		 _open_objects={ } }
   inherit_from(FileClass, new)
   if mode == "w" then
      new._hid = H5.H5Fcreate(name, H5.H5F_ACC_TRUNC, hp0, hp0)
   elseif mode == "r" then
      new._hid = H5.H5Fopen(name, H5.H5F_ACC_RDONLY, hp0)
   elseif mode == "r+" then
      new._hid = H5.H5Fopen(name, H5.H5F_ACC_RDWR, hp0)
   else
      error("mode must be one of [w, r, r+]")
   end
   setmetatable(new, FileMeta)
   return new
end


--------------------------------------------------------------------------------
-- HDF5 Group constructor
--------------------------------------------------------------------------------
function hdf5.Group(parent, name)
   local new = { _type='group',
		 _parent=parent,
		 _name=name,
		 _hid=0,
		 _close=H5.H5Gclose,
		 _open_objects={ } }
   inherit_from(GroupClass, new)
   if not H5.H5Lexists(parent._hid, name, hp0) then
      new._hid = H5.H5Gcreate2(parent._hid, name, hp0, hp0, hp0)
   else
      new._hid = H5.H5Gopen2(parent._hid, name, hp0)
   end
   parent._open_objects[name] = new
   setmetatable(new, GroupMeta)
   return new
end


--------------------------------------------------------------------------------
-- HDF5 DataSet constructor
-- 
-- DataSet's may be either created or opened by this constructor. If a DataSet
-- with `name` exists in the object `parent` then it is opened and returned, and
-- the `dataspace` and `datatype` arguments are ignored. Otherwise, a new
-- DataSet, with corresponding space and type, is created and returned.
--------------------------------------------------------------------------------
function hdf5.DataSet(parent, name, dataspace, datatype)
   local new = { _type='data set',
		 _parent=parent,
		 _name=name,
		 _hid=0,
		 _close=H5.H5Dclose }
   inherit_from(DataSetClass, new)
   if not H5.H5Lexists(parent._hid, name, hp0) then
      new._hid = H5.H5Dcreate2(parent._hid, name, datatype._hid, dataspace._hid,
			       hp0, hp0, hp0)
   else
      new._hid = H5.H5Dopen2(parent._hid, name, hp0)
   end
   parent._open_objects[name] = new
   setmetatable(new, DataSetMeta)
   return new
end


--------------------------------------------------------------------------------
-- HDF5 DataType constructor
--
-- If `arg` is an hid_t representing a type then a copy of that type is made via
-- H5Tcopy. If `arg` is a string, then it must be a valid key into the variable
-- typedict. Opening existing types from data sets is done by the DataSet class.
--------------------------------------------------------------------------------
function hdf5.DataType(typeid)
   local new = { _type='data type',
		 _hid=0,
		 _close=H5.H5Tclose }
   inherit_from(DataTypeClass, new)
   local typedict = {char=H5.H5T_NATIVE_CHAR,
		     int=H5.H5T_NATIVE_INT,
		     float=H5.H5T_NATIVE_FLOAT,
		     double=H5.H5T_NATIVE_DOUBLE}
   if type(typeid) == 'string' then
      new._hid = H5.H5Tcopy(typedict[typeid])
   else
      new._hid = H5.H5Tcopy(typeid)
   end
   setmetatable(new, DataTypeMeta)
   return new
end


--------------------------------------------------------------------------------
-- HDF5 DataSpace constructor
--
-- If `arg` is a string, then it must be either 'simple' or 'scalar' and a new
-- data space is created and returned. If `arg` is a data set then its data
-- space is extracted and returned.
--------------------------------------------------------------------------------
function hdf5.DataSpace(arg)
   local new = { _type='data space',
		 _hid=0,
		 _close=H5.H5Sclose }
   inherit_from(DataSpaceClass, new)
   local arg = arg or 'simple'
   if type(arg) == 'string' then
      local t = { simple=H5.H5S_SIMPLE, scalar=H5.H5S_SCALAR }
      new._hid = H5.H5Screate(t[arg])
   elseif class(arg) == 'data set' then
      new._hid = H5.H5Dget_space(arg._hid)
   else
      error("DataSpace constructor argument not recognized")
   end
   setmetatable(new, DataSpaceMeta)
   return new
end


--------------------------------------------------------------------------------
-- Unit test cases
--------------------------------------------------------------------------------
local function test1()
   local h5f = hdf5.File("outfile.h5", "w")
   local h5g = hdf5.Group(h5f, "thegroup")
   local h5h = hdf5.Group(h5g, "thesubgroup")
   assert(h5f["thegroup"]["thesubgroup"] == h5h)
   h5f:close()
end

local function test2()
   local h5f = hdf5.File("outfile.h5", "w")
   local h5g = hdf5.Group(h5f, "thegroup")
   local h5h = hdf5.Group(h5g, "thesubgroup")
   h5g:close()
   assert(h5f["thegroup"]["thesubgroup"]:path() == h5h:path())
   assert(h5f["thething"] == nil)
   h5f:close()
end

local function test3()
   local h5f = hdf5.File("outfile.h5", "w")
   local hg1 = hdf5.Group(h5f, "thegroup1")
   local hg2 = hdf5.Group(h5f, "thegroup2")
   local hg3 = hdf5.Group(h5f, "thegroup3")
   for k,v in pairs(h5f) do
      assert(v == h5f[k])
   end
   hg3["message"] = "the message content"
   h5f:close()
end

local function test4()
   local h5s = hdf5.DataSpace()
   h5s:set_extent{10,12,14}
   local size = h5s:get_extent('maximum')
   assert(size[1] == 10)
   assert(size[2] == 12)
   assert(size[3] == 14)

   local dtype = hdf5.DataType('double')
   dtype:set_size(128)
   assert(dtype:get_size() == 128)
end

local function test5() -- depends on test3 being run first
   local file = hdf5.File("outfile.h5", "r")
   local dset = hdf5.DataSet(file["thegroup3"], "message")
   local fspc = hdf5.DataSpace(dset)
   local buf = dset:read()
   assert(tostring(buf) == "the message content")
   assert(type(file["thegroup3"]["message"]:value()) == 'string')
   dset:close()
   file:close()
end

local function test6()
   local file = hdf5.File("outfile.h5", "w")
   local fspc = hdf5.DataSpace('simple')
   local type = hdf5.DataType('double')
   fspc:set_extent{4,4,8}

   local dset = hdf5.DataSet(file, "data1d", fspc, type)
   local buf = buffer.new_buffer(4*4*8*8)
   dset:write(buf)

   local array = array.view(buf, 'double', {0,0,0}, {2,2,2}, {2,2,2})
   file["data3d"] = array
   file:close()

   local file = hdf5.File("outfile.h5", "r")
   assert(#file["data3d"]:read() == 64)
   assert(file["data3d"]:value():dtype() == 'double')
   local start, stride, count, block = file["data3d"]:value():selection()
   assert(count[1] == 2)
   assert(count[2] == 2)
   assert(count[3] == 2)
   file:close()
end

local function test7()
   local buf = buffer.new_buffer(4*4*8 * array.sizeof('double'))
   local my_data = array.view(buf, 'double', {0,0,0}, {4,4,8})
   local h5f = hdf5.File("outfile.h5", "w")
   h5f["dataset"] = my_data
   local group1 = h5f:require_group("group1")
   group1["message"] = "here is the message"
   h5f:close()
end



if ... then -- if __name__ == "__main__"
   return hdf5
else
   local function runtests()
      test1()
      test2()
      test3()
      test4()
      test5()
      test6()
      test7()
   end
   local success, msg = xpcall(runtests, debug.traceback)
   if not success then
      print(msg)
   else
      print(debug.getinfo(1).source, ": All tests passed")
   end
end
