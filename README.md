
# Overview

These wrappers provide a high and low-level Lua interface to the HDF5
library. The code provided here should be embedded in your application to give
it flexible interaction with HDF5.

# Example usage

## High-level interface
    local hdf5 = require 'hdf5'
    local h5f = hdf5.File("outfile.h5", "w")
    local h5g = hdf5.Group(h5f, "thegroup")
    local h5h = hdf5.Group(h5g, "subgroup")
    local h5s = hdf5.DataSpace()
    h5s:set_extent{128,128,256}
    ...


## Low-level bindings
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
    ...


# Build instructions

Create a file called Makefile.in which contains macros like these:

    LUA_I = -I/path/to/lua-5.2.1/include
    LUA_A = /path/to/lua-5.2.1/lib/liblua.a

    HDF5_I = -I/Library/Science/hdf5/include
    HDF5_L = -L/Library/Science/hdf5/lib -lz -lhdf5

Then type `make`.


# License

This code is made freely available for anybody's use. I only ask that if you
find it useful, please send me an email about how it works, and in what project
you are using it.


The Lua HDF5 wrappers are licensed under the same terms as Lua itself.

Copyright (c) 2012, Jonathan Zrake

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

