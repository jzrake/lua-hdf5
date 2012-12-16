
--------------------------------------------------------------------------------
--
--               High level Lua wrappers for HDF5 library
--
--------------------------------------------------------------------------------
local hdf5 = { } -- module table
--------------------------------------------------------------------------------

local H5 = require 'h5lua'
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


--------------------------------------------------------------------------------
-- Base classes for meta-table and methods wrapping hid_t objects
--------------------------------------------------------------------------------
local BaseMeta = { }
function BaseMeta:__index()
   if self._hid == 0 then -- object is closed
      return nil
   end
   --   print("item retrieval not supported") -- not a necessary warning!
end
function BaseMeta:__newindex()
   if self._hid == 0 then -- object is closed
      return nil
   end
   print("item assignment not supported")
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
   else
      error("object %s/%s has an unsupported type", self._name, key)
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
-- HDF5 DataSpace class methods
--------------------------------------------------------------------------------
local DataSpaceClass = inherit_from(BaseClass)
function DataSpaceClass:set_extent(extent)
   if self._hid == 0 then
      error("DataSpaceClass:set_extent cannot operate on closed data space")
   end
   local current_size = H5.new_hsize_t_arr(extent)
   local maximum_size = H5.new_hsize_t_arr(extent)
   H5.H5Sset_extent_simple(self._hid, #extent, current_size, maximum_size)
end
function DataSpaceClass:get_extent(type)
   if self._hid == 0 then
      error("DataSpaceClass:get_extent cannot operate on closed data space")
   end
   local rank = H5.H5Sget_simple_extent_ndims(self._hid)
   local csize = { }
   local msize = { }
   for i=1,rank do csize[i] = 0 end
   for i=1,rank do msize[i] = 0 end
   local current_size = H5.new_hsize_t_arr(csize)
   local maximum_size = H5.new_hsize_t_arr(msize)
   H5.H5Sget_simple_extent_dims(self._hid, current_size, maximum_size)
   for i=1,rank do csize[i] = current_size[i-1] end
   for i=1,rank do msize[i] = maximum_size[i-1] end
   if not type or type == 'current' then
      return csize
   elseif type == 'maximum' then
      return msize
   else
      error("DataSpaceClass:get_extent: type must be 'current' or 'maximum'")
   end
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
-- HDF5 DataSpace constructor
--------------------------------------------------------------------------------
function hdf5.DataSpace()
   local new = { _type='data space',
		 _hid=0,
		 _close=H5.H5Sclose }
   inherit_from(DataSpaceClass, new)
   new._hid = H5.H5Screate(H5.H5S_SIMPLE)
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
end

local function test4()
   local h5s = hdf5.DataSpace()
   h5s:set_extent{10,12,14}
   print(h5s)
   local size = h5s:get_extent('maximum')
   assert(size[1] == 10)
   assert(size[2] == 12)
   assert(size[3] == 14)
end

--test1()
--collectgarbage() -- to close files
--test2()
--collectgarbage()
--test3()
--collectgarbage()

test4()
collectgarbage()

return hdf5
