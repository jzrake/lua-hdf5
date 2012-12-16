
# ------------------------------------------------------------------------------
# H5Lua build instructions
# ------------------------------------------------------------------------------
#
# Create a file called Makefile.in which contains macros like these:
#
#    LUA_I = -I/path/to/lua-5.2.1/include
#    LUA_A = /path/to/lua-5.2.1/lib/liblua.a
#
#    HDF5_I = -I/Library/Science/hdf5/include
#    HDF5_L = -L/Library/Science/hdf5/lib -lz -lhdf5
#
# Then type `make`.
# ------------------------------------------------------------------------------

include Makefile.in

default : main

h5lua.o : h5lua.c
	$(CC) -Wall -c -o $@ $< $(LUA_I) $(HDF5_I)

buffer.o : buffer.c
	$(CC) -Wall -c -o $@ $< $(LUA_I)

main : main.o h5lua.o buffer.o
	$(CC) -Wall -o $@ $^ $(LUA_I) $(LUA_A) $(HDF5_L)

clean :
	rm -f *.o main h5lua_wrap.c
