
local H5 = require 'h5lua'
local hp0 = H5.H5P_DEFAULT


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
   if self._hid ~= 0 then
      self._close(self._hid)
      self._hid = 0
   else
      error(string.format("attempted to close an inactive %s", self._type))
   end
end

local FileClass = inherit_from(BaseClass)
local GroupClass = inherit_from(BaseClass)
local AttributeClass = inherit_from(BaseClass)
local TypeClass = inherit_from(BaseClass)


local FileMeta = inherit_from(BaseMeta)
local GroupMeta = inherit_from(BaseMeta)

function FileMeta:__tostring()
   return string.format("<HDF5 File: %s [mode %s]>", self._filename, self._mode)
end
function GroupMeta:__tostring()
   return string.format("<HDF5 Group: %s>", self._name)
end


local function File(filename, mode)
   -----------------------------------------------------------------------------
   -- HDF5 File class
   -----------------------------------------------------------------------------
   local new = { _type='file',
		 _filename=filename,
		 _mode=mode,
		 _hid=0,
		 _close=H5.H5Fclose }
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
   -- HDF5 Group class
   -----------------------------------------------------------------------------
   local new = { _type='group',
		 _parent=parent,
		 _name=name,
		 _hid=0,
		 _close=H5.H5Gclose }
   inherit_from(GroupClass, new)
   new._hid = H5.H5Gcreate2(parent._hid, name, hp0, hp0, hp0)
   setmetatable(new, GroupMeta)
   return new
end


local function self_test()
   local h5f = File("outfile.h5", "w")
   local h5g = Group(h5f, "thegroup")

   print(h5f)
   print(h5g)

   h5f:close()
   h5g:close()
end

self_test()
