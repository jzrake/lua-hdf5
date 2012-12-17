
# ------------------------------------------------------------------------------
# H5Lua build instructions
# ------------------------------------------------------------------------------
#
# 1. Optionally, you may install local Lua sources by typing `make lua`
#
#
# 2. Create a file called Makefile.in which contains macros like these:
#
#    LUA_I = -I/path/to/lua-5.2.1/include
#    LUA_A = -L/path/to/lua-5.2.1/lib -llua
#
#    HDF5_I = -I/path/to/hdf5/include
#    HDF5_L = -L/path/to/hdf5/lib -lz -lhdf5
#
#    # Additional compile flags are optional:
#
#    CC = gcc
#    CFLAGS = -Wall -O2
#    LVER = lua-5.2.1 # can be lua-5.1 or other
#
#
# 3. Run `python parse.py` in order to generate wrapper code for your own HDF5
#    library version.
#
#
# 4. Run `make`.
# ------------------------------------------------------------------------------

include Makefile.in

CFLAGS ?= -Wall
CURL ?= curl
UNTAR ?= tar -xvf
CD ?= cd
RM ?= rm
LVER ?= lua-5.2.1


default : main

lua : $(LVER)

$(LVER) :
	$(CURL) http://www.lua.org/ftp/$(LVER).tar.gz -o $(LVER).tar.gz
	$(UNTAR) $(LVER).tar.gz
	$(CD) $(LVER); $(MAKE) generic CC=$(CC); \
		$(MAKE) install INSTALL_TOP=$(PWD)/$(LVER)
	$(RM) $(LVER).tar.gz

h5lua.o : h5lua.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_I) $(HDF5_I)

buffer.o : buffer.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_I)

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_I)

main : main.o h5lua.o buffer.o
	$(CC) $(CFLAGS) -o $@ $^ $(LUA_I) $(LUA_A) $(HDF5_L)

clean :
	$(RM) -f *.o main

# Also remove local Lua sources
realclean :
	$(RM) -rf $(LVER)
