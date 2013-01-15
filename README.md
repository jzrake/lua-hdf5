
# Overview

These wrappers provide a high and low-level Lua interface to the HDF5
library. The code provided here should be embedded in your application to give
it flexible interaction with HDF5.

# Example usage

## High-level interface

This demonstrates the simplest usage of the high-level HDF5 interface. It
creates a file with a (numeric array-valued) data set named 'the_dataset' and
group named 'the_group' containing a (string-valued) data set called
'the_message'.

    local hdf5 = require 'LuaHDF5'
    local array = require 'array'

    local the_array = array.array({16,32,64}, 'double')
    local the_h5file = hdf5.File("outfile.h5", "w")
    local the_group = the_h5file:require_group("the_group")
    the_group["the_message"] = "here is the message"
    the_h5file["the_dataset"] = the_array
    the_h5file:close()


## Low-level bindings

Access to the low-level HDF5 library follows exactly the HDF5 API specification,
in other words there is one-to-one mapping between (most) HDF5 functions and
their low-level Lua wrappers. Some HDF5 functions are not wrapped, although they
can be recognized by the wrapper generator `parse.py` with easy modifications.

    local H5 = require 'HDF5'
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
    USE_MPIO = True # wrap HDF5 collectve IO routines


Optionally, you may install local Lua sources by typing `make lua`.


Run `make`.


# Running tests

Type `./main alltests.lua`.


# Compatibility and conventions

Lua HDF5 contains a Python script to generate low-level wrapper code which has
been tested for HDF5 library versions between 1.8.3 and 1.8.10. The generated
Lua code is tested for Lua 5.2.1, although small adjustments may be made to
support Lua 5.0 and 5.1.

An example program is provided which embeds the Lua HDF5 interface. It loads the
`HDF5` module into the `package.loaded.HDF5` table, which can then be accessed
within Lua by typing `local H5 = require 'HDF5'`. The module can be given global
visibility to Lua scripts by passing calling `luaL_requiref(L, "HDF5",
luaopen_hdf5, 1);` with 1 instead of 0 as the last argument.


# License

This code is made freely available for anybody's use. I only ask that if you
find it useful, please send me an email about how it works, and in what project
you are using it.


The Lua HDF5 wrappers are licensed under the same terms as Lua itself.

Copyright (c) 2012, Jonathan Zrake <jonathan.zrake@gmail.com>

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

