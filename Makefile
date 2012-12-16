
LUA_A = ../lua-5.2.1/lib/liblua.a
LUA_I = -I../lua-5.2.1/include
HDF5_I = -I/Library/Science/hdf5/include
HDF5_L = -L/Library/Science/hdf5/lib -lz -lhdf5

default : main

h5lua.o : h5lua.c
	$(CC) -Wall -c -o $@ $< $(LUA_I) $(HDF5_I)

main : main.o h5lua.o
	$(CC) -Wall -o $@ $^ $(LUA_I) $(LUA_A) $(HDF5_L)

clean :
	rm -f *.o main h5lua_wrap.c
