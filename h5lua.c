
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


// -----------------------------------------------------------------------------
// hid_t
// -----------------------------------------------------------------------------
static void lh5_push_hid_t(lua_State *L, hid_t id)
{
  *((hid_t*) lua_newuserdata(L, sizeof(hid_t))) = id;
  luaL_setmetatable(L, "HDF5::hid_t");
}
static int h5lua_new_hid_t(lua_State *L)
{
  lh5_push_hid_t(L, 0);
  return 1;
}

// -----------------------------------------------------------------------------
// herr_t
// -----------------------------------------------------------------------------
static void lh5_push_herr_t(lua_State *L, herr_t id)
{
  *((herr_t*) lua_newuserdata(L, sizeof(herr_t))) = id;
  luaL_setmetatable(L, "HDF5::herr_t");
}
static int h5lua_new_herr_t(lua_State *L)
{
  lh5_push_herr_t(L, 0);
  return 1;
}

// -----------------------------------------------------------------------------
// hsize_t_arr
// -----------------------------------------------------------------------------
static void lh5_push_hsize_t_arr(lua_State *L, hsize_t *hs, unsigned int N)
{
  memcpy(lua_newuserdata(L, sizeof(hsize_t) * N), hs, sizeof(hsize_t) * N);
  luaL_setmetatable(L, "HDF5::hsize_t_arr");
}
static int h5lua_new_hsize_t_arr(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  int n = 0, N = lua_rawlen(L, 1);
  hsize_t *hs = (hsize_t*) malloc(sizeof(hsize_t) * N);

  while (n < N) {
    lua_rawgeti(L, 1, n+1);
    hs[n] = lua_tounsigned(L, -1);
    lua_pop(L, 1);
    ++n;
  }

  lh5_push_hsize_t_arr(L, hs, N);
  free(hs);
  return 1;
}
static int h5lua_hsize_t_arr__index(lua_State *L)
{
  hsize_t *lhs = (hsize_t*) luaL_checkudata(L, 1, "HDF5::hsize_t_arr");
  unsigned int n = luaL_checkunsigned(L, 2);
  unsigned int N = lua_rawlen(L, 1) / sizeof(hsize_t);
  if (n < N) {
    lua_pushnumber(L, lhs[n]);
  }
  else {
    lua_pushnil(L);
  }
  return 1;
}
static int h5lua_hsize_t_arr__newindex(lua_State *L)
{
  hsize_t *lhs = (hsize_t*) luaL_checkudata(L, 1, "HDF5::hsize_t_arr");
  unsigned int n = luaL_checkunsigned(L, 2);
  hsize_t val = luaL_checkunsigned(L, 3);
  unsigned int N = lua_rawlen(L, 1) / sizeof(hsize_t);
  if (n < N) {
    lhs[n] = val;
  }
  else {
    luaL_error(L, "index %d out of range on array of length %d", n, N);
  }
  return 1;
}


/* hid_t H5Fopen( const char *name, unsigned flags, hid_t fapl_id ) */
static int h5lua_H5Fopen(lua_State *L)
{
  const char *name = luaL_checkstring(L, 1);
  unsigned flags = luaL_checkunsigned(L, 2);
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Fopen(name, flags, fapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}

/* hid_t H5Fcreate( const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id ) */
static int h5lua_H5Fcreate(lua_State *L)
{
  const char *name = luaL_checkstring(L, 1);
  unsigned flags = luaL_checkunsigned(L, 2);
  hid_t fcpl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t res = H5Fcreate(name, flags, fcpl_id, fapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}

/* herr_t H5Fclose( hid_t file_id ) */
static int h5lua_H5Fclose(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Fclose(file_id);
  lh5_push_herr_t(L, res);
  return 1;
}

/* hid_t H5Dcreate:
   
   hid_t loc_id,
   const char *name,
   hid_t dtype_id,
   hid_t space_id,
   hid_t lcpl_id,
   hid_t dcpl_id,
   hid_t dapl_id
 */
static int h5lua_H5Dcreate(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t dtype_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t dcpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 7, "HDF5::hid_t"));
  hid_t res = H5Dcreate(loc_id, name, dtype_id, space_id, lcpl_id, dcpl_id, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}

/* herr_t H5Dclose( hid_t dataset_id ) */
static int h5lua_H5Dclose(lua_State *L)
{
  hid_t dataset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Dclose(dataset_id);
  lh5_push_herr_t(L, res);
  return 1;
}

/* hid_t H5Screate( H5S_class_t type ) */
static int h5lua_H5Screate(lua_State *L)
{
  H5S_class_t type = luaL_checkinteger(L, 1);
  herr_t res = H5Screate(type);
  lh5_push_hid_t(L, res);
  return 1;
}

/* herr_t H5Sclose( hid_t space_id ) */
static int h5lua_H5Sclose(lua_State *L)
{
  hid_t dataset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sclose(dataset_id);
  lh5_push_herr_t(L, res);
  return 1;
}




int luaopen_h5lua(lua_State *L)
{
  luaL_Reg h5lua_funcs[] = {{"new_hid_t", h5lua_new_hid_t},
			    {"new_herr_t", h5lua_new_herr_t},
			    {"new_hsize_t_arr", h5lua_new_hsize_t_arr},
                            {"H5Fopen", h5lua_H5Fopen},
                            {"H5Fcreate", h5lua_H5Fcreate},
                            {"H5Fclose", h5lua_H5Fclose},
			    {"H5Dcreate", h5lua_H5Dcreate},
			    {"H5Dclose", h5lua_H5Dclose},
			    {"H5Screate", h5lua_H5Screate},
			    {"H5Sclose", h5lua_H5Sclose},
                            {NULL, NULL}};

  luaL_Reg hsize_t_arr_meta[] = {{"__index", h5lua_hsize_t_arr__index},
				 {"__newindex", h5lua_hsize_t_arr__newindex},
				 {NULL, NULL}};

  luaL_newmetatable(L, "HDF5::hid_t");
  lua_pop(L, 1);

  luaL_newmetatable(L, "HDF5::herr_t");
  lua_pop(L, 1);

  luaL_newmetatable(L, "HDF5::hsize_t_arr");
  luaL_setfuncs(L, hsize_t_arr_meta, 0);
  lua_pop(L, 1);

  lua_getglobal(L, "package");
  lua_getfield(L, -1, "loaded");
  luaL_newlib(L, h5lua_funcs);

  lua_pushnumber(L, H5F_ACC_TRUNC); lua_setfield(L, -2, "H5F_ACC_TRUNC");
  lua_pushnumber(L, H5F_ACC_RDONLY); lua_setfield(L, -2, "H5F_ACC_RDONLY");
  lua_pushnumber(L, H5S_SCALAR); lua_setfield(L, -2, "H5S_SCALAR");
  lh5_push_hid_t(L, H5P_DEFAULT); lua_setfield(L, -2, "H5P_DEFAULT");
  lh5_push_hid_t(L, H5T_NATIVE_DOUBLE); lua_setfield(L, -2, "H5T_NATIVE_DOUBLE");

  lua_setfield(L, -2, "h5lua");
  lua_pop(L, 2);

  return 0;
}
