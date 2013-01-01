

#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

enum { 
  BUFFER_TYPE_INT=1,
  BUFFER_TYPE_CHAR=2,
  BUFFER_TYPE_FLOAT=3,
  BUFFER_TYPE_DOUBLE=4,
} ;

// -----------------------------------------------------------------------------
// buffer
// -----------------------------------------------------------------------------
static void *buf_push_buffer(lua_State *L, const void *p, size_t size)
{
  void *newbuf = lua_newuserdata(L, size);
  if (newbuf == NULL) {
    luaL_error(L, "buffer: not enough memory to allocate buffer");
  }
  if (p != NULL) {
    memcpy(newbuf, p, size);
  }
  else {
    memset(newbuf, 0, size);
  }
  luaL_setmetatable(L, "buffer");
  return newbuf;
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
  default:
    N = 0;
    luaL_error(L, "buffer: argument must be either number or string");
  }
  buf_push_buffer(L, buf, N);
  return 1;
}

static int buffer_sizeof(lua_State *L)
{
  unsigned int T = luaL_checkunsigned(L, 1);
  size_t s = 0;
  switch (T) {
  case BUFFER_TYPE_CHAR: s = sizeof(char); break;
  case BUFFER_TYPE_INT: s = sizeof(int); break;
  case BUFFER_TYPE_FLOAT: s = sizeof(float); break;
  case BUFFER_TYPE_DOUBLE: s = sizeof(double); break;
  default:
    luaL_error(L, "buffer: unknown data type specification");
  }
  lua_pushnumber(L, s);
  return 1;
}

static int buffer_get_typed(lua_State *L)
{
  const char *buf = luaL_checkudata(L, 1, "buffer");
  unsigned int T = luaL_checkunsigned(L, 2); // type
  unsigned int n = luaL_checkunsigned(L, 3); // index
  unsigned int N = lua_rawlen(L, 1); // buffer size
  size_t offset;

#define CASE(t)								\
  do {									\
    offset = n * sizeof(t);						\
    if (offset >= N) luaL_error(L, "buffer: index out of range");	\
    lua_pushnumber(L, *((t*)(buf + offset)));				\
  } while(0)								\

  switch (T) {
  case BUFFER_TYPE_CHAR: CASE(char); break;
  case BUFFER_TYPE_INT: CASE(int); break;
  case BUFFER_TYPE_FLOAT: CASE(float); break;
  case BUFFER_TYPE_DOUBLE: CASE(double); break;
  default:
    luaL_error(L, "buffer: unknown data type specification");
  }
#undef CASE

  return 1;
}
static int buffer_set_typed(lua_State *L)
{
  const char *buf = luaL_checkudata(L, 1, "buffer");
  unsigned int T = luaL_checkunsigned(L, 2); // type
  unsigned int n = luaL_checkunsigned(L, 3); // index
  double v = luaL_checknumber(L, 4); // value
  unsigned int N = lua_rawlen(L, 1); // buffer size
  size_t offset;

#define CASE(t)								\
  do {									\
    offset = n * sizeof(t);						\
    if (offset >= N) luaL_error(L, "buffer: index out of range");	\
    *((t*)(buf + offset)) = v;						\
  } while(0)								\

  switch (T) {
  case BUFFER_TYPE_CHAR: CASE(char); break;
  case BUFFER_TYPE_INT: CASE(int); break;
  case BUFFER_TYPE_FLOAT: CASE(float); break;
  case BUFFER_TYPE_DOUBLE: CASE(double); break;
  default:
    luaL_error(L, "buffer: unknown data type specification");
  }
#undef CASE

  return 0;
}

static int buffer_light(lua_State *L)
{
  char *buf = luaL_checkudata(L, 1, "buffer");
  lua_pushlightuserdata(L, buf);
  return 1;
}

static int buffer_isbuffer(lua_State *L)
{
  lua_pushboolean(L, luaL_testudata(L, 1, "buffer") != NULL);
  return 1;
}

static int buffer_extract(lua_State *L)
/* -----------------------------------------------------------------------------
 * Extracts a slice from B1, and returns it as the contiguous array 'B0'
 * -----------------------------------------------------------------------------
 * 1. @buf    : [buffer] a buffer instance (source)
 * 2. @rank   : [unsigned int] dimensionality of buf
 * 3. @byte   : [unsigned int] number of bytes at each element
 * 4. @extent : [int*] buf size along each axis
 * 5. @start  : [int*] starting indices into buf
 * 6. @stride : [int*] number of elements to skip along each each axis
 * 7. @count  : [int*] number of elements to select along each axis
 *
 * Returns
 * 1. dst     : contiguous slice extracted from buf
 * -----------------------------------------------------------------------------
 */
{
  int d, M=0, m=0, ntot=1;

  char *buf = (char*) luaL_checkudata(L, 1, "buffer");
  unsigned int rank = luaL_checkunsigned(L, 2);
  unsigned int byte = luaL_checkunsigned(L, 3);
  int *exten = (int*) lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  int *start = (int*) lua_touserdata(L, 5); luaL_checktype(L, 5, LUA_TUSERDATA);
  int *strid = (int*) lua_touserdata(L, 6); luaL_checktype(L, 6, LUA_TUSERDATA);
  int *count = (int*) lua_touserdata(L, 7); luaL_checktype(L, 7, LUA_TUSERDATA);

  for (d=0; d<rank; ++d) {
    ntot *= count[d];
  }
  if (ntot > lua_rawlen(L, 1) / byte) {
    luaL_error(L, "buffer: slice is to large for source buffer");
  }
  if (rank != lua_rawlen(L, 4) / sizeof(int) ||
      rank != lua_rawlen(L, 5) / sizeof(int) ||
      rank != lua_rawlen(L, 6) / sizeof(int) ||
      rank != lua_rawlen(L, 7) / sizeof(int)) {
    luaL_error(L, "buffer: slice description has wrong size");
  }

  int *J = (int*) malloc(rank * sizeof(int)); // current indices into buffer
  int *N = (int*) malloc(rank * sizeof(int)); // number of elements to select
  int *S = (int*) malloc(rank * sizeof(int)); // skips along each axis
  char *dst = (char*) buf_push_buffer(L, NULL, ntot * byte); // destination

  for (d=0; d<rank; ++d) {
    J[d] = 0;
    N[d] = count[d];
  }
  S[rank-1] = 1;
  for (d=rank-2; d>=0; --d) {
    S[d] = S[d+1] * exten[d+1];
  }
  while (J[0] < N[0]) {
    M = 0;
    for (d=0; d<rank; ++d) {
      M += (J[d] * strid[d] + start[d]) * S[d];
    }
    memcpy(dst + (m++)*byte, buf + M*byte, byte);
    ++J[rank-1];
    for (d=rank-1; d!=0; --d) {
      if (J[d] == N[d]) {
	J[d] = 0;
	++J[d-1];
      }
    }
  }

  free(J);
  free(N);
  free(S);
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
  unsigned int N = lua_rawlen(L, 1);  // max index
  if (n < N) {
    buf[n] = val;
  }
  else {
    luaL_error(L, "buffer: index %d out of range on buffer of length %d", n, N);
  }
  return 0;
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
    {"sizeof", buffer_sizeof},
    {"light", buffer_light},
    {"get_typed", buffer_get_typed},
    {"set_typed", buffer_set_typed},
    {"extract", buffer_extract},
    {"isbuffer", buffer_isbuffer},
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

#define REG_NUMBER(s,t) lua_pushnumber(L, s); lua_setfield(L, -2, t);
  REG_NUMBER(BUFFER_TYPE_CHAR, "char");
  REG_NUMBER(BUFFER_TYPE_INT, "int");
  REG_NUMBER(BUFFER_TYPE_FLOAT, "float");
  REG_NUMBER(BUFFER_TYPE_DOUBLE, "double");
#undef REG_NUMBER

  return 1;
}
