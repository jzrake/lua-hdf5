
local buffer = require 'buffer'
local view = { }

function view.view(buf, type, start, size, stride, block)
   local rank = #start
   local stride = stride or { }
   local block = block or { }
   for i=1,rank do
      stride[i] = stride[i] or 1
      block[i] = block[i] or 1
   end
   local new = { _buf=buf,
		 _type=buffer[type],
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
   if totsize > #buf / buffer.sizeof(new._type) then
      error("buffer is too small for the requested view")
   end
   new._totsize = totsize

   local mt = { }
   function mt:__index(ind)
      return buffer.get_typed(self._buf, self._type, ind)
   end
   function mt:__newindex(ind, value)
      return buffer.set_typed(self._buf, self._type, ind, value)
   end
   function mt:__len(ind) return self._totsize end
   setmetatable(new, mt)
   return new
end

return view
