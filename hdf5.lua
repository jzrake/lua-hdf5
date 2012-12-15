
local H5 = require 'h5lua'
local hp0 = H5.H5P_DEFAULT

local function printf(...)
   print(string.format(...))
end

local function inherit_from(base, derived)
   -- Basically just deep-copy the base class table --
   local new = derived or { }
   for k,v in pairs(base) do new[k] = v end
   return new
end

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
-- HDF5 File class methods
--------------------------------------------------------------------------------
local FileClass = inherit_from(BaseClass)


--------------------------------------------------------------------------------
-- HDF5 Group class methods
--------------------------------------------------------------------------------
local GroupClass = inherit_from(BaseClass)



local FileMeta = inherit_from(BaseMeta)
function FileMeta:__index(key)
   return self._open_objects[key]
end
function FileMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 file: \"%s\" (mode %s)>", self._filename,
			   self._mode)
   else
      return "<Closed HDF5 file>"
   end
end

local GroupMeta = inherit_from(BaseMeta)
function GroupMeta:__tostring()
   if self._hid ~= 0 then
      return string.format("<HDF5 group: \"%s\">", self._name)
   else
      return "<Closed HDF5 group>"
   end
end
function GroupMeta:__index(key)
   return self._open_objects[key]
end

local function File(filename, mode)
   -----------------------------------------------------------------------------
   -- HDF5 File constructor
   -----------------------------------------------------------------------------
   local new = { _type='file',
		 _filename=filename,
		 _mode=mode,
		 _hid=0,
		 _close=H5.H5Fclose,
		 _open_objects={ } }
   inherit_from(FileClass, new)
   if mode == "w" then
      new._hid = H5.H5Fcreate(filename, H5.H5F_ACC_TRUNC, hp0, hp0)
   elseif mode == "r" then
      new._hid = H5.H5Fopen(filename, H5.H5F_ACC_RDONLY, hp0)
   elseif mode == "r+" then
      new._hid = H5.H5Fopen(filename, H5.H5F_ACC_RDWR, hp0)
   else
      error("mode must be one of [w, r, r+]")
   end
   setmetatable(new, FileMeta)
   return new
end


local function Group(parent, name)
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
   new._hid = H5.H5Gcreate2(parent._hid, name, hp0, hp0, hp0)
   parent._open_objects[name] = new
   setmetatable(new, GroupMeta)
   return new
end


local function test1()
   local h5f = File("outfile.h5", "w")
   local h5g = Group(h5f, "thegroup")
   local h5h = Group(h5g, "thesubgroup")
   assert(h5f["thegroup"]["thesubgroup"] == h5h)
end


local function test2()
   local h5f = File("outfile.h5", "w")
   local h5g = Group(h5f, "thegroup")
   local h5h = Group(h5g, "thesubgroup")
   h5h:close()
   print(h5f["thegroup"]["thesubgroup"])
end

test1()
--test2()
