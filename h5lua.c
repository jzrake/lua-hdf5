
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
// H5O_info_t
// -----------------------------------------------------------------------------
static void lh5_push_H5O_info_t(lua_State *L, H5O_info_t id)
{
  *((H5O_info_t*) lua_newuserdata(L, sizeof(H5O_info_t))) = id;
  luaL_setmetatable(L, "HDF5::H5O_info_t");
}
static int h5lua_new_H5O_info_t(lua_State *L)
{
  H5O_info_t id;
  lh5_push_H5O_info_t(L, id);
  return 1;
}
static int h5lua_H5O_info_t__index(lua_State *L)
{
  H5O_info_t *i = (H5O_info_t*) luaL_checkudata(L, 1, "HDF5::H5O_info_t");
  const char *key = luaL_checkstring(L, 2);
  if (0) { }
  else if (strcmp(key, "fileno") == 0) { lua_pushnumber(L, i->fileno); }
  else if (strcmp(key, "addr") == 0) { lua_pushnumber(L, i->addr); }
  else if (strcmp(key, "type") == 0) { lua_pushnumber(L, i->type); }
  else if (strcmp(key, "rc") == 0) { lua_pushnumber(L, i->rc); }
  else if (strcmp(key, "atime") == 0) { lua_pushnumber(L, i->atime); }
  else if (strcmp(key, "mtime") == 0) { lua_pushnumber(L, i->mtime); }
  else if (strcmp(key, "ctime") == 0) { lua_pushnumber(L, i->ctime); }
  else if (strcmp(key, "btime") == 0) { lua_pushnumber(L, i->btime); }
  else if (strcmp(key, "num_attrs") == 0) { lua_pushnumber(L, i->num_attrs); }
  else { lua_pushnil(L); }
  return 1;
}
static int h5lua_H5O_info_t__newindex(lua_State *L)
{
  luaL_checkudata(L, 1, "HDF5::H5O_info_t");
  luaL_error(L, "object does not support item assignment");
  return 0;
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


// -----------------------------------------------------------------------------
// By-hand wrappers
// -----------------------------------------------------------------------------
static herr_t _H5Literate_cb(hid_t g_id, const char *name,
			     const H5L_info_t *info, void *op_data)
{
  /*
   *   function expects
   *
   * + op_data is a lua_State*
   * + function on top of Lua stack
   */
  lua_State *L = (lua_State*) op_data;
  lua_pushvalue(L, -1);
  lua_pushstring(L, name);
  lua_call(L, 1, 0);
  return 0;
}
int h5lua_H5Literate(lua_State *L)
{
  hid_t group_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5_index_t index_type = luaL_checkinteger(L, 2);
  H5_iter_order_t order = luaL_checkinteger(L, 3);
  hsize_t *idx = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  luaL_checktype(L, 5, LUA_TFUNCTION);
  H5L_iterate_t op = _H5Literate_cb;
  int ret = H5Literate(group_id, index_type, order, idx, op, L);
  return ret;
}


// -----------------------------------------------------------------------------
// Python-generated wrappers
// -----------------------------------------------------------------------------
#include "h5funcs.c"


int luaopen_h5lua(lua_State *L)
{
  luaL_Reg h5lua_types[] = {
    {"new_hid_t", h5lua_new_hid_t},
    {"new_herr_t", h5lua_new_herr_t},
    {"new_H5O_info_t", h5lua_new_H5O_info_t},
    //    {"new_double_arr", h5lua_new_double_arr},
    {"new_hsize_t_arr", h5lua_new_hsize_t_arr},
    {NULL, NULL}};

  luaL_Reg H5O_info_t_meta[] = {
    {"__index", h5lua_H5O_info_t__index},
    {"__newindex", h5lua_H5O_info_t__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::H5O_info_t");
  luaL_setfuncs(L, H5O_info_t_meta, 0);
  lua_pop(L, 1);

  luaL_Reg hsize_t_arr_meta[] = {
    {"__index", h5lua_hsize_t_arr__index},
    {"__newindex", h5lua_hsize_t_arr__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::hsize_t_arr");
  luaL_setfuncs(L, hsize_t_arr_meta, 0);
  lua_pop(L, 1);

  /*
  luaL_Reg double_arr_meta[] = {
    {"__index", h5lua_double_arr__index},
    {"__newindex", h5lua_double_arr__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::double_arr");
  luaL_setfuncs(L, double_arr_meta, 0);
  lua_pop(L, 1);
  */

  luaL_newmetatable(L, "HDF5::hid_t");
  lua_pop(L, 1);
  luaL_newmetatable(L, "HDF5::herr_t");
  lua_pop(L, 1);


  // ---------------------------------------------------------------------------
  // Module definition
  // ---------------------------------------------------------------------------
  lua_newtable(L);
  luaL_setfuncs(L, h5lua_types, 0);
  luaL_setfuncs(L, H5A_funcs, 0);
  luaL_setfuncs(L, H5D_funcs, 0);
  luaL_setfuncs(L, H5E_funcs, 0);
  luaL_setfuncs(L, H5F_funcs, 0);
  luaL_setfuncs(L, H5G_funcs, 0);
  luaL_setfuncs(L, H5I_funcs, 0);
  luaL_setfuncs(L, H5L_funcs, 0);
  luaL_setfuncs(L, H5O_funcs, 0);
  luaL_setfuncs(L, H5P_funcs, 0);
  luaL_setfuncs(L, H5R_funcs, 0);
  luaL_setfuncs(L, H5S_funcs, 0);
  luaL_setfuncs(L, H5T_funcs, 0);
  luaL_setfuncs(L, H5Z_funcs, 0);
  register_constants(L);

  return 1;
}
