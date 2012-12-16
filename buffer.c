

#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"





// -----------------------------------------------------------------------------
// buffer
// -----------------------------------------------------------------------------
static void buf_push_buffer(lua_State *L, const void *p, size_t size)
{
  void *newbuf = lua_newuserdata(L, size);
  if (newbuf == NULL) {
    luaL_error(L, "not enough memory to allocate buffer");
  }
  if (p != NULL) {
    memcpy(newbuf, p, size);
  }
  else {
    memset(newbuf, 0, size);
  }
  luaL_setmetatable(L, "buffer");
}
static int buffer_new_buffer(lua_State *L)
{
  int type = lua_type(L, 1);
  size_t N;
  const void *buf = NULL;
  switch (type) {
  case LUA_TNUMBER:
    N = luaL_checkunsigned(L, 1);
    buf = NULL;
    break;
  case LUA_TSTRING:
    N = lua_rawlen(L, 1);
    buf = lua_tostring(L, 1);
    break;
  }

  buf_push_buffer(L, buf, N);
  //  printf("built character buffer of length %ld\n", N);
  return 1;
}
static int buffer__index(lua_State *L)
{
  const char *buf = luaL_checkudata(L, 1, "buffer");
  unsigned int n = luaL_checkunsigned(L, 2);
  unsigned int N = lua_rawlen(L, 1);
  if (n < N) {
    lua_pushnumber(L, buf[n]);
  }
  else {
    lua_pushnil(L);
  }
  return 1;
}
static int buffer__newindex(lua_State *L)
{
  char *buf = luaL_checkudata(L, 1, "buffer"); // buffer
  unsigned int n = luaL_checkunsigned(L, 2); // index
  char val = luaL_checkinteger(L, 3); // value
  unsigned int N = lua_rawlen(L, 1) / sizeof(char);  // max index
  if (n < N) {
    buf[n] = val;
  }
  else {
    luaL_error(L, "index %d out of range on buffer of length %d", n, N);
  }
  return 1;
}
static int buffer__len(lua_State *L)
{
  luaL_checkudata(L, 1, "buffer");
  lua_pushnumber(L, lua_rawlen(L, 1));
  return 1;
}
static int buffer__tostring(lua_State *L)
{
  char *buf = luaL_checkudata(L, 1, "buffer");
  lua_pushlstring(L, buf, lua_rawlen(L, 1));
  return 1;
}


int luaopen_buffer(lua_State *L)
{
  luaL_Reg buffer_types[] = {
    {"new_buffer", buffer_new_buffer},
    {NULL, NULL}};

  luaL_Reg buffer_meta[] = {
    {"__index", buffer__index},
    {"__newindex", buffer__newindex},
    {"__len", buffer__len},
    {"__tostring", buffer__tostring},
    {NULL, NULL}};

  luaL_newmetatable(L, "buffer");
  luaL_setfuncs(L, buffer_meta, 0);
  lua_pop(L, 1);

  lua_newtable(L);
  luaL_setfuncs(L, buffer_types, 0);

  return 1;
}
