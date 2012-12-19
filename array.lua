
local buffer = require 'buffer'
local array = { }

--------------------------------------------------------------------------------
-- It's better to use string names to identify C data types in Lua. This code
-- wraps the C functions and converts the string to corresponding enum.
--------------------------------------------------------------------------------

function array.sizeof(T)
   return buffer.sizeof(buffer[T])
end
function array.get_typed(buf, T, n)
   return buffer.get_typed(buf, buffer[T], n)
end
function array.set_typed(buf, T, n, v)
   buffer.set_typed(buf, buffer[T], n, v)
end


function array.array(size, dtype)
   local size = type(size) == 'table' and size or {size}
   local start = { }
   local rank = #size
   local dtype = dtype or 'double'
   local nelem = 1
   for i=1,rank do
      nelem = nelem * size[i]
      start[i] = 0
   end
   local buf = buffer.new_buffer(nelem * array.sizeof(dtype))
   return array.view(buf, dtype, start, size)
end


function array.view(buf, dtype, start, size, stride)
   local sz =  array.sizeof(dtype)
   local start = start or { 0 }
   local size = size or { #buf/sz }
   local stride = stride or { }
   local block = { }
   local rank = #start
   for i=1,rank do
      stride[i] = stride[i] or 1
      block[i] = 1 -- non-trivial block not supported
   end
   local new = { _buf=buf,
		 _dtype=buffer[dtype],
		 _dtype_string=dtype,
		 _rank=rank,
		 _start=start,
		 _size=size,
		 _stride=stride,
		 _block=block }

   if rank ~= #size or
      rank ~= #stride or
      rank ~= #block then
      error("inconsistent sizes of extent description")
   end
   local bsize = 1 -- buffer size spanned
   local vsize = 1 -- elements in view
   for i=1,rank do
      vsize = vsize * size[i]
      bsize = bsize * size[i] * stride[i]
   end
   if bsize * sz > #buf then
      error("buffer is too small for the requested view")
   end

   -- skip is the conventional C-ordering distance between elements along the
   -- i-th axis. Skip sizes are in units of the data element size.
   local skip = {[rank]=1}
   for i=rank-1,1,-1 do skip[i] = skip[i+1] * size[i+1] end

   new._elem = vsize
   new._skip = skip

   function new:buffer()
      return self._buf
   end
   function new:dtype()
      return self._dtype_string
   end
   function new:selection()
      return self._start, self._size, self._stride, self._block
   end

   local mt = { }
   function mt:__index(ind)
      if type(ind) == 'string' then
	 error(string.format("buffer has no attribute %s", ind))
      end
      if type(ind) ~= 'table' then error("index must be a table") end
      if #ind ~= self._rank then error("wrong number of indices") end
      local n = 0
      for i=1,self._rank do
	 if ind[i] < 0 or ind[i] >= self._size[i] then
	    error("index out of bounds")
	 end
	 n = n + (ind[i] + self._start[i]) * self._stride[i] * self._skip[i]
      end
      return array.get_typed(self._buf, self._dtype_string, n)
   end

   function mt:__newindex(ind, value)
      if type(ind) ~= 'table' then error("index must be a table") end
      if #ind ~= self._rank then error("wrong number of indices") end
      local n = 0
      for i=1,self._rank do
	 if ind[i] < 0 or ind[i] >= self._size[i] then
	    error("index out of bounds")
	 end
	 n = n + (ind[i] + self._start[i]) * self._stride[i] * self._skip[i]
      end
      return array.set_typed(self._buf, self._dtype_string, n, value)
   end
   function mt:__tostring()
      return string.format("<array: %s>", self._dtype_string)
   end
   function mt:__len(ind) return self._elem end
   setmetatable(new, mt)
   return new
end

return array
