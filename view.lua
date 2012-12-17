
local buffer = require 'buffer'

function buffer.view(buf, dtype, start, size, stride, block)
   local rank = #start
   local stride = stride or { }
   local block = block or { }
   for i=1,rank do
      stride[i] = stride[i] or 1
      block[i] = block[i] or 1
   end
   local new = { _buf=buf,
		 _dtype=buffer[dtype],
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
   local totsize = 1
   for i=1,rank do
      totsize = totsize * size[i] * block[i] / stride[i]
   end
   if totsize > #buf / buffer.sizeof(new._dtype) then
      error("buffer is too small for the requested view")
   end
   local skip = {[rank]=1}
   for i=rank-1,1,-1 do skip[i] = skip[i+1] * size[i+1] end

   new._totsize = totsize
   new._skip = skip

   local mt = { }
   function mt:__index(ind)
      if type(ind) == 'number' then
	 error("index must be a table")
      end
      if #ind ~= self._rank then
	 error("wrong number of indices")
      end
      local n = 0

      for i=1,self._rank do
	 if ind[i] < 0 or ind[i] >= self._size[i] then
	    error("index out of bounds")
	 end
	 n = n + (ind[i] + self._start[i]) * self._stride[i] * skip[i]
      end
      return buffer.get_typed(self._buf, self._dtype, n)
   end

   function mt:__newindex(ind, value)
      return buffer.set_typed(self._buf, self._dtype, ind, value)
   end
   function mt:__len(ind) return self._totsize end
   setmetatable(new, mt)
   return new
end
