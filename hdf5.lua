
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
   print("item retrieval not supported")
end
function BaseMeta:__newindex()
   error("item assignment not supported")
end
function BaseMeta:__gc()
   self:close()
end

local BaseClass = { }
function BaseClass:close()
   if self._open_objects then
      for k,v in pairs(self._open_objects) do
	 v:close()
      end
      self._open_objects = { }
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


--------------------------------------------------------------------------------
-- IndexableMeta methods
--------------------------------------------------------------------------------
local IndexableMeta = inherit_from(BaseMeta)
function IndexableMeta:__index(key)
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


--------------------------------------------------------------------------------
-- HDF5 File class methods
--------------------------------------------------------------------------------
local FileClass = inherit_from(IndexableClass)


--------------------------------------------------------------------------------
-- HDF5 Group class methods
--------------------------------------------------------------------------------
local GroupClass = inherit_from(IndexableClass)


local FileMeta = inherit_from(IndexableMeta)
function FileMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 file: \"%s\" (mode %s)>", self._name,
			   self._mode)
   else
      return "<Closed HDF5 file>"
   end
end

local GroupMeta = inherit_from(IndexableMeta)
function GroupMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 group: \"%s\">", self._name)
   else
      return "<Closed HDF5 group>"
   end
end

function hdf5.File(name, mode)
   -----------------------------------------------------------------------------
   -- HDF5 File constructor
   -----------------------------------------------------------------------------
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


function hdf5.Group(parent, name)
   -----------------------------------------------------------------------------
   -- HDF5 Group constructor
   -----------------------------------------------------------------------------
   local new = { _type='group',
		 _parent=parent,
		 _name=name,
		 _hid=0,
		 _close=H5.H5Gclose,
		 _open_objects={ } }
   inherit_from(GroupClass, new)
   local exist = H5.H5Lexists(parent._hid, name, hp0)
   if not exist then
      new._hid = H5.H5Gcreate2(parent._hid, name, hp0, hp0, hp0)
   else
      new._hid = H5.H5Gopen2(parent._hid, name, hp0)
   end
   parent._open_objects[name] = new
   setmetatable(new, GroupMeta)
   return new
end


local function test1()
   local h5f = hdf5.File("outfile1.h5", "w")
   local h5g = hdf5.Group(h5f, "thegroup")
   local h5h = hdf5.Group(h5g, "thesubgroup")
   assert(h5f["thegroup"]["thesubgroup"] == h5h)
end


local function test2()
   local h5f = hdf5.File("outfile2.h5", "w")
   local h5g = hdf5.Group(h5f, "thegroup")
   local h5h = hdf5.Group(h5g, "thesubgroup")
   assert(h5h:path() == "/outfile2.h5/thegroup/thesubgroup")
   h5g:close()
   assert(h5f["thegroup"]["thesubgroup"]._name == h5h._name)
   assert(h5f["thething"] == nil)
end

test1()
test2()

return hdf5
