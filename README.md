
# Overview

These wrappers provide a high and low-level Lua interface to the HDF5
library. The code provided here should be embedded in your application to give
it flexible interaction with HDF5.

# Example usage

## High-level interface
    local hdf5 = require 'hdf5'
    local buffer = require 'buffer'

    local buf = buffer.new_buffer(4*4*8 * buffer.sizeof('double'))
    local my_data = buffer.view(buf, 'double', {0,0,0}, {4,4,8})
    local h5f = hdf5.File("outfile.h5", "w")
    h5f["dataset"] = my_data

    local group1 = h5f:require_group("group1")
    group1["message"] = "here is the message"

    h5f:close()


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


Make sure you have the [HDF5
sources](http://www.hdfgroup.org/HDF5/release/obtain5.html) installed.


Create a file called Makefile.in which contains macros like these:

    LUA_HOME = /path/to/lua-5.2.1
    HDF_HOME = /path/to/hdf5-1.8.10

Additional compile flags are optional:

    CC = gcc
    CFLAGS = -Wall -O2
    LVER = lua-5.2.1 # can be lua-5.1 or other


Optionally, you may install local Lua sources by typing `make lua`.


Run `python parse.py` in order to generate wrapper code for your own HDF5
library version.


Run `make`.


# Running tests

Type `./main alltests.lua`.


# Compatibility and conventions

H5Lua contains a Python script to generate low-level wrapper code which has been
tested for HDF5 library versions between 1.8.3 and 1.8.10. The generated Lua
code is tested for Lua 5.2.1, although small adjustments may be made to support
Lua 5.0 and 5.1.

An example program is provided which embeds the Lua HDF5 interface. It loads the
H5Lua module into the `package.loaded.h5lua` table, which can then be accessed
within Lua by typing `local h5lua = require 'h5lua'`. The module can be given
global visibility to Lua scripts by passing calling `luaL_requiref(L, "h5lua",
luaopen_h5lua, 1);` with 1 instead of 0 as the last argument.


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

