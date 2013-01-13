
local buffer = require 'buffer'
local array = { }
local vector = { }
local view = { }

function array.sizeof(T)
   return buffer.sizeof(buffer[T])
end
function array.get_typed(buf, T, n)
   return buffer.get_typed(buf, buffer[T], n)
end
function array.set_typed(buf, T, n, v)
   buffer.set_typed(buf, buffer[T], n, v)
end

function vector:__index(i,x)
   if type(i) == 'string' then
      error('vector has no method '..i)
   end
   while i < 0 do i = i + #self end
   return buffer.get_typed(self._buf, buffer[self._dtype], i)
end
function vector:__newindex(i,x)
   while i < 0 do i = i + #self end
   buffer.set_typed(self._buf, buffer[self._dtype], i, x)
end
function vector:__len(i,x)
   return self._len
end
function vector:__tostring(i,x)
   local tab = { }
   if #self <= self._printn then
      for i=1,#self do
	 tab[i] = self[i-1]
      end
   else
      local N=self._printn
      for i=1,N-1 do
	 tab[i] = self[i-1]
	 tab[2*N-i] = self[-i]
      end
      tab[N] = '...'
   end
   return '['..table.concat(tab, ', ')..']'
end

function array.vector(arg, dtype)
   local dtype = dtype or 'double'
   local new = {_dtype=dtype, _printn=5}
   local val = false

   if buffer.isbuffer(arg) then
      new._len = #arg / array.sizeof(dtype)
      new._buf = arg
   else
      new._len = type(arg) == 'table' and #arg or arg
      new._buf = buffer.new_buffer(new._len * array.sizeof(dtype))
      val = type(arg) == 'table' and arg or { }
   end

   function new:buffer() return self._buf end
   function new:pointer() return buffer.light(self._buf) end
   function new:dtype() return self._dtype end
   function new:selection() return self:view():selection() end
   function new:extent() return self:view():extent() end
   function new:view(extent, start, count, stride)
      return array.view(self._buf, self._dtype, extent or {#self},
			start, count, stride)
   end
   function new:copy(extent, start, count, stride)
      return self:view():copy():vector()
   end
   function new:set_printn(n) self._printn = n end
   setmetatable(new, vector)

   if val then
      for i=1,#new do
	 new[i-1] = val[i] or 0
      end
   end
   return new
end

function view:__index(descr)
   local descr = descr or { }
   if type(descr) == 'string' then
      error('view has no method '..descr)
   elseif type(descr) == 'number' then
      error('index must be a table')
   end
   local start = { }
   local count = { }
   local strid = { }
   local shape = self:shape()
   for i=1,self._rank do
      descr[i] = descr[i] or { }
      descr[i][2] = descr[i][2] or shape[i]
      while descr[i][2] < 0 do
	 descr[i][2] = descr[i][2] + shape[i]
      end
      start[i] = descr[i][1] or 0
      strid[i] = descr[i][3] or 1
      count[i] =(descr[i][2] - start[i]) / strid[i]
      if count[i] <= 0 then
	 error('count must be >= 0')
      end
   end
   for i=1,self._rank do
      start[i] = start[i] + self._start[i]
      strid[i] = strid[i] * self._stride[i]
      if start[i] + count[i] > shape[i] then
	 error('start + count not within extent')
      end
   end
   return array.view(self._buf, self._dtype, self._extent, start, count, strid)
end

function view:__newindex(descr, value)
   local descr = descr or { }
   if type(descr) == 'string' then
      error('view has no method '..descr)
   elseif type(descr) == 'number' then
      error('index must be a table')
   end
   local exten = self._extent
   local start = { }
   local count = { }
   local strid = { }
   local shape = self:shape()
   for i=1,self._rank do
      descr[i] = descr[i] or { }
      descr[i][2] = descr[i][2] or shape[i]
      while descr[i][2] < 0 do
	 descr[i][2] = descr[i][2] + shape[i]
      end
      start[i] = descr[i][1] or 0
      strid[i] = descr[i][3] or 1
      count[i] =(descr[i][2] - start[i]) / strid[i]
      if count[i] <= 0 then
	 error('count must be >= 0')
      end
   end
   for i=1,self._rank do
      start[i] = start[i] + self._start[i]
      strid[i] = strid[i] * self._stride[i]
      if start[i] + count[i] > shape[i] then
	 error('start + count not within extent')
      end
   end

   local exten0 = array.vector(exten, 'int'):buffer()
   local start0 = array.vector(start, 'int'):buffer()
   local strid0 = array.vector(strid, 'int'):buffer()
   local count0 = array.vector(count, 'int'):buffer()
   local exten1 = array.vector(value._extent, 'int'):buffer()
   local start1 = array.vector(value._start, 'int'):buffer()
   local strid1 = array.vector(value._stride, 'int'):buffer()
   local count1 = array.vector(value._count, 'int'):buffer()

   local buf0 = self._buf
   local buf1 = value._buf

   buffer.copy(buf0, exten0, start0, strid0, count0,
	       buf1, exten1, start1, strid1, count1,
	       array.sizeof(self._dtype))
end

function view:__len()
   return self._vsize
end

function array.view(buf, dtype, extent, start, count, stride)
   local sz =  array.sizeof(dtype)
   local start = start or { }
   local count = count or { }
   local stride = stride or { }
   local block = { }
   local rank = #extent
   for i=1,rank do
      start[i] = start[i] or 0
      count[i] = count[i] or extent[i]
      stride[i] = stride[i] or 1
      block[i] = 1 -- non-trivial block not supported
   end
   local new = {_buf=buf,
		_dtype=dtype,
		_rank=rank,
		_extent=extent,
		_start=start,
		_count=count,
		_stride=stride,
		_block=block,
		_bsize=0,
		_vsize=0}
   function new:buffer() return self._buf end
   function new:pointer() return buffer.light(self._buf) end
   function new:dtype() return self._dtype end
   function new:start() return self._start end
   function new:shape() return self._count end -- shape of the selection
   function new:stride() return self._stride end
   function new:extent() return self._extent end -- global buffer extent
   function new:selection() -- useful for HDF5 compatibility
      return self._start, self._stride, self._count, self._block
   end
   function new:contiguous()
      for i=1,self._rank do
	 if self._extent[i] ~= self._count[i] then return false end
      end
      return true
   end
   function new:copy()
      local buf = buffer.extract(self._buf, self._rank,
				 array.sizeof(self._dtype),
				 array.vector(self._extent, 'int')._buf,
				 array.vector(self._start, 'int')._buf,
				 array.vector(self._stride, 'int')._buf,
				 array.vector(self._count, 'int')._buf)
      return array.view(buf, self._dtype, self._count)
   end
   function new:vector()
      local arr = self:contiguous() and self or self:copy()
      return array.vector(arr._buf, arr._dtype)
   end
   setmetatable(new, view)

   local bsize = 1 -- buffer size spanned
   local vsize = 1 -- elements in view
   for i=1,rank do
      if start[i] + count[i] > extent[i] then
	 error('start + count not within extent')
      end
      bsize = bsize * extent[i]
      vsize = vsize * count[i]
   end
   if bsize * sz ~= #buf then
      error("buffer has the wrong size for the requested view")
   end
   new._bsize = bsize
   new._vsize = vsize
   return new
end

function array.array(extent, dtype)
   local dtype = dtype or 'double'
   local N = 1
   for i,n in ipairs(extent) do N = N * n end
   local buf = buffer.new_buffer(N * buffer.sizeof(buffer[dtype]))
   return array.view(buf, dtype, extent)
end

--------------------------------------------------------------------------------
-- Module test suite
--------------------------------------------------------------------------------
local function test1()
   local vec = array.vector{0.0, 1.0, 2.0}
   assert(#vec == 3)
   assert(vec[ 0] == 0)
   assert(vec[-1] == #vec-1)
   assert(tostring(vec) == '[0, 1, 2]')
   assert(vec:dtype() == 'double')
end
local function test2()
   local vec = array.vector(10, 'int')
   vec[-1] = 10
   assert(#vec == 10)
   assert(vec[ 0] == 0)
   assert(vec[-1] == 10)
   assert(tostring(vec) == '[0, 0, 0, 0, ..., 0, 0, 0, 10]')
   assert(not pcall(function() vec[11] = 0 end))
   assert(vec:dtype() == 'int')
end
local function test3()
   local vec = array.vector(1000)
   for i=0,#vec-1 do vec[i] = i end
   local view0 = vec:view{10,10,10}
   local view1 = view0[{{0,10},{0,10},{0,10,2}}]
   local view2 = view1[{{0,10},{0,5},{0,4,2}}]
   local shp1 = view1:shape()
   local shp2 = view2:shape()
   assert(#view1 == 500)
   assert(#view2 == 100)
   assert(shp1[1] == 10)
   assert(shp1[2] == 10)
   assert(shp1[3] == 5)
   assert(shp2[1] == 10)
   assert(shp2[2] == 5)
   assert(shp2[3] == 2)
   assert(view0:contiguous())
   assert(not view1:contiguous())
   assert(not view2:contiguous())
end
local function test4()
   local A0 = array.array{10,10,10}
   local A1 = array.array{10,10,10}
   local V0 = A0:vector()
   local V1 = A1:vector()
   A0[{{2,8},{2,8},{2,8}}] = A1[{{2,8},{2,8},{2,8}}]
   for i=0,#V0-1 do
      if V0[i] ~= 0 then
	 assert(V0[i] == V1[i])
      end
   end
end


--------------------------------------------------------------------------------
-- Run test or export module
--------------------------------------------------------------------------------
if ... then -- if __name__ == "__main__"
   return array
else
   test1()
   test2()
   test3()
   test4()
   print(debug.getinfo(1).source, ": All tests passed")
end
