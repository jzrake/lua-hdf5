
# ------------------------------------------------------------------------------
# H5Lua build instructions
# ------------------------------------------------------------------------------
#
# 1. Make sure you have the HDF5 sources installed.
#
#
# 2. Create a file called Makefile.in which contains macros like these:
#
#    LUA_HOME = /path/to/lua-5.2.1
#    HDF_HOME = /path/to/hdf5-1.8.10
#
#    # Additional compile flags are optional:
#
#    CC = gcc
#    CFLAGS = -Wall -O2
#    LVER = lua-5.2.1 # can be lua-5.1 or other
#
#
# 3. Optionally, you may install local Lua sources by typing `make lua`.
#
#
# 4. Run `python parse.py` in order to generate wrapper code for your own HDF5
#    library version.
#
#
# 5. Run `make`.
#
# ------------------------------------------------------------------------------

MAKEFILE_IN = Makefile.in
include $(MAKEFILE_IN)

CFLAGS ?= -Wall
CURL ?= curl
UNTAR ?= tar -xvf
CD ?= cd
RM ?= rm
LVER ?= lua-5.2.1

LUA_I ?= -I$(LUA_HOME)/include
LUA_A ?= -L$(LUA_HOME)/lib -llua
HDF_I ?= -I$(HDF_HOME)/include
HDF_L ?= -L$(HDF_HOME)/lib -lz -lhdf5


default : main

lua : $(LVER)

$(LVER) :
	$(CURL) http://www.lua.org/ftp/$(LVER).tar.gz -o $(LVER).tar.gz
	$(UNTAR) $(LVER).tar.gz
	$(CD) $(LVER); $(MAKE) generic CC=$(CC); \
		$(MAKE) install INSTALL_TOP=$(PWD)/$(LVER)
	$(RM) $(LVER).tar.gz

h5lua.o : h5lua.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_I) $(HDF_I)

buffer.o : buffer.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_I)

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_I)

main : main.o h5lua.o buffer.o
	$(CC) $(CFLAGS) -o $@ $^ $(LUA_I) $(LUA_A) $(HDF_L)

clean :
	$(RM) *.o main

# Also remove local Lua sources
realclean : clean
	$(RM) -r $(LVER)
