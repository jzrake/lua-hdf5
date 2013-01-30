
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifndef H5_VERSION_GE
#define H5_VERSION_GE(Maj,Min,Rel)					\
  (((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR==Min) && (H5_VERS_RELEASE>=Rel)) || \
   ((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR>Min)) ||			\
   (H5_VERS_MAJOR>Maj))
#endif
#ifndef H5_VERSION_LE
#define H5_VERSION_LE(Maj,Min,Rel)					\
  (((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR==Min) && (H5_VERS_RELEASE<=Rel)) || \
   ((H5_VERS_MAJOR==Maj) && (H5_VERS_MINOR<Min)) ||			\
   (H5_VERS_MAJOR<Maj))
#endif

// -----------------------------------------------------------------------------
// hid_t
// -----------------------------------------------------------------------------
static void lh5_push_hid_t(lua_State *L, hid_t id)
{
  *((hid_t*) lua_newuserdata(L, sizeof(hid_t))) = id;
  luaL_setmetatable(L, "HDF5::hid_t");
}
static int _new_hid_t(lua_State *L)
{
  lh5_push_hid_t(L, 0);
  return 1;
}
static int _hid_t__len(lua_State *L)
{
  hid_t *h = (hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t");
  lua_pushnumber(L, *h);
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
static int _new_herr_t(lua_State *L)
{
  lh5_push_herr_t(L, 0);
  return 1;
}
static int _herr_t__len(lua_State *L)
{
  herr_t *h = (herr_t*) luaL_checkudata(L, 1, "HDF5::herr_t");
  lua_pushnumber(L, *h);
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
static int _new_H5O_info_t(lua_State *L)
{
  H5O_info_t id = { };
  lh5_push_H5O_info_t(L, id);
  return 1;
}
static int _H5O_info_t__index(lua_State *L)
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
static int _H5O_info_t__newindex(lua_State *L)
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
static int _new_hsize_t_arr(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  int n = 0, N = lua_rawlen(L, 1);
  hsize_t *hs = (hsize_t*) malloc(sizeof(hsize_t) * N);

  while (n < N) {
    lua_rawgeti(L, 1, n+1);
    hsize_t val = lua_tointeger(L, -1);
    if (val == -1) val = H5S_UNLIMITED;
    hs[n] = val;
    lua_pop(L, 1);
    ++n;
  }

  lh5_push_hsize_t_arr(L, hs, N);
  free(hs);
  return 1;
}
static int _hsize_t_arr__index(lua_State *L)
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
static int _hsize_t_arr__newindex(lua_State *L)
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

static int _H5_VERSION_GE(lua_State *L)
{
  int Maj = luaL_checkinteger(L, 1);
  int Min = luaL_checkinteger(L, 2);
  int Rel = luaL_checkinteger(L, 3);
  lua_pushboolean(L, H5_VERSION_GE(Maj,Min,Rel));
  return 1;
}

static int _H5_VERSION_LE(lua_State *L)
{
  int Maj = luaL_checkinteger(L, 1);
  int Min = luaL_checkinteger(L, 2);
  int Rel = luaL_checkinteger(L, 3);
  lua_pushboolean(L, H5_VERSION_LE(Maj,Min,Rel));
  return 1;
}

static int _H5_VERS_INFO(lua_State *L)
{
  lua_pushstring(L, H5_VERS_INFO);
  return 1;
}

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
int _H5Literate(lua_State *L)
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

static int _H5Pget_mpio_actual_chunk_opt_mode(lua_State *L)
{
#ifdef H5_HAVE_PARALLEL
#if (H5_VERSION_GE(1,8,8))
  hid_t dxpl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5D_mpio_actual_chunk_opt_mode_t actual_chunk_opt_mode;
  H5Pget_mpio_actual_chunk_opt_mode(dxpl_id, &actual_chunk_opt_mode);
  lua_pushnumber(L, actual_chunk_opt_mode);
  return 1;
#else
  return 0;
#endif // H5_VERSION_GE(1,8,8)
#else
  return 0;
#endif // H5_HAVE_PARALLEL
}

static int _H5Pget_mpio_actual_io_mode(lua_State *L)
{
#ifdef H5_HAVE_PARALLEL
#if (H5_VERSION_GE(1,8,8))
  hid_t dxpl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5D_mpio_actual_io_mode_t actual_io_mode;
  H5Pget_mpio_actual_io_mode(dxpl_id, &actual_io_mode);
  lua_pushnumber(L, actual_io_mode);
  return 1;
#else
  return 0;
#endif // H5_VERSION_GE(1,8,8)
#else
  return 0;
#endif // H5_HAVE_PARALLEL
}

static int _H5Pget_mpio_no_collective_cause(lua_State *L)
{
#ifdef H5_HAVE_PARALLEL
#if (H5_VERSION_GE(1,8,10))
  hid_t dxpl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  uint32_t local_no_collective_cause;
  uint32_t global_no_collective_cause;
  H5Pget_mpio_no_collective_cause(dxpl_id, &local_no_collective_cause,
				  &global_no_collective_cause);
  lua_pushnumber(L, local_no_collective_cause);
  lua_pushnumber(L, global_no_collective_cause);
  return 2;
#else
  return 0;
#endif // H5_VERSION_GE(1,8,10)
#else
  return 0;
#endif // H5_HAVE_PARALLEL
}


// -----------------------------------------------------------------------------
// Python-generated wrappers
// -----------------------------------------------------------------------------
#include "h5funcs.c"


int luaopen_hdf5(lua_State *L)
{
  luaL_Reg hdf5_auxf[] = {
    {"new_hid_t", _new_hid_t},
    {"new_herr_t", _new_herr_t},
    {"new_H5O_info_t", _new_H5O_info_t},
    {"new_hsize_t_arr", _new_hsize_t_arr},
    {"H5_VERSION_GE", _H5_VERSION_GE},
    {"H5_VERSION_LE", _H5_VERSION_LE},
    {"H5_VERS_INFO", _H5_VERS_INFO},
    {NULL, NULL}};

  luaL_Reg H5O_info_t_meta[] = {
    {"__index", _H5O_info_t__index},
    {"__newindex", _H5O_info_t__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::H5O_info_t");
  luaL_setfuncs(L, H5O_info_t_meta, 0);
  lua_pop(L, 1);

  luaL_Reg hsize_t_arr_meta[] = {
    {"__index", _hsize_t_arr__index},
    {"__newindex", _hsize_t_arr__newindex},
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::hsize_t_arr");
  luaL_setfuncs(L, hsize_t_arr_meta, 0);
  lua_pop(L, 1);

  luaL_Reg herr_t_meta[] = {
    {"__len", _herr_t__len}, // really the number value
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::herr_t");
  luaL_setfuncs(L, herr_t_meta, 0);
  lua_pop(L, 1);

  luaL_Reg hid_t_meta[] = {
    {"__len", _hid_t__len}, // really the number value
    {NULL, NULL}};
  luaL_newmetatable(L, "HDF5::hid_t");
  luaL_setfuncs(L, hid_t_meta, 0);
  lua_pop(L, 1);


  // ---------------------------------------------------------------------------
  // Module definition
  // ---------------------------------------------------------------------------
  lua_newtable(L);
  luaL_setfuncs(L, hdf5_auxf, 0);
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
  luaL_setfuncs(L, H5P_MPI_funcs, 0);
  register_constants(L);

  return 1;
}
