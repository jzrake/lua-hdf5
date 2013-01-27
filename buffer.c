

#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

enum { 
  BUFFER_TYPE_CHAR=1,
  BUFFER_TYPE_INT=2,
  BUFFER_TYPE_LONG=3,
  BUFFER_TYPE_FLOAT=4,
  BUFFER_TYPE_DOUBLE=5,
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
  case BUFFER_TYPE_LONG: s = sizeof(long); break;
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
  case BUFFER_TYPE_LONG: CASE(long); break;
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
  case BUFFER_TYPE_LONG: CASE(long); break;
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

static int buffer_copy(lua_State *L)
/* -----------------------------------------------------------------------------
 *
 * Copy a `rank1` memory selection of `buf1` into a `rank0` memory selection of
 * `buf0`.
 *
 * -----------------------------------------------------------------------------
 */
{
  int d, M0=0, M1=0, ntot0=1, ntot1=1;
  char *buf0 = (char*) luaL_checkudata(L, 1, "buffer");
  int *exten0 = (int*) luaL_checkudata(L, 2, "buffer");
  int *start0 = (int*) luaL_checkudata(L, 3, "buffer");
  int *strid0 = (int*) luaL_checkudata(L, 4, "buffer");
  int *count0 = (int*) luaL_checkudata(L, 5, "buffer");
  char *buf1 = (char*) luaL_checkudata(L, 6, "buffer");
  int *exten1 = (int*) luaL_checkudata(L, 7, "buffer");
  int *start1 = (int*) luaL_checkudata(L, 8, "buffer");
  int *strid1 = (int*) luaL_checkudata(L, 9, "buffer");
  int *count1 = (int*) luaL_checkudata(L,10, "buffer");
  unsigned int byte = luaL_checkunsigned(L, 11);

  int rank0 = lua_rawlen(L, 2) / sizeof(int);
  int rank1 = lua_rawlen(L, 7) / sizeof(int);

  for (d=0; d<rank0; ++d) {
    ntot0 *= count0[d];
  }
  for (d=0; d<rank1; ++d) {
    ntot1 *= count1[d];
  }

  if (ntot0 != ntot1) {
    luaL_error(L, "source and destination selections have different sizes");
  }
  if (ntot0 * byte > lua_rawlen(L, 1)) {
    luaL_error(L, "buffer: slice is to large for source buffer");
  }
  if (ntot1 * byte > lua_rawlen(L, 6)) {
    luaL_error(L, "buffer: slice is to large for desination buffer");
  }

  if (rank0 != lua_rawlen(L, 3) / sizeof(int) ||
      rank0 != lua_rawlen(L, 4) / sizeof(int) ||
      rank0 != lua_rawlen(L, 5) / sizeof(int)) {
    luaL_error(L, "buffer: slice description for source has wrong size");
  }
  if (rank1 != lua_rawlen(L, 8) / sizeof(int) ||
      rank1 != lua_rawlen(L, 9) / sizeof(int) ||
      rank1 != lua_rawlen(L,10) / sizeof(int)) {
    luaL_error(L, "buffer: slice description for destination has wrong size");
  }

  int *J0 = (int*) malloc(rank0 * sizeof(int)); // current indices into buffer
  int *N0 = (int*) malloc(rank0 * sizeof(int)); // number of elements to select
  int *S0 = (int*) malloc(rank0 * sizeof(int)); // skips along each axis

  int *J1 = (int*) malloc(rank1 * sizeof(int));
  int *N1 = (int*) malloc(rank1 * sizeof(int));
  int *S1 = (int*) malloc(rank1 * sizeof(int));


  for (d=0; d<rank0; ++d) {
    J0[d] = 0;
    N0[d] = count0[d];
  }

  for (d=0; d<rank1; ++d) {
    J1[d] = 0;
    N1[d] = count1[d];
  }

  S0[rank0-1] = 1;
  S1[rank1-1] = 1;

  for (d=rank0-2; d>=0; --d) {
    S0[d] = S0[d+1] * exten0[d+1];
  }
  for (d=rank1-2; d>=0; --d) {
    S1[d] = S1[d+1] * exten1[d+1];
  }

  while (J0[0] < N0[0]) {

    M0 = 0;
    M1 = 0;

    for (d=0; d<rank0; ++d) {
      M0 += (J0[d] * strid0[d] + start0[d]) * S0[d];
    }
    for (d=0; d<rank1; ++d) {
      M1 += (J1[d] * strid1[d] + start1[d]) * S1[d];
    }

    memcpy(buf0 + M0*byte, buf1 + M1*byte, byte);

    ++J0[rank0-1];
    ++J1[rank1-1];

    for (d=rank0-1; d!=0; --d) {
      if (J0[d] == N0[d]) {
	J0[d] = 0;
	++J0[d-1];
      }
    }
    for (d=rank1-1; d!=0; --d) {
      if (J1[d] == N1[d]) {
	J1[d] = 0;
	++J1[d-1];
      }
    }
  }

  free(J0);
  free(N0);
  free(S0);
  free(J1);
  free(N1);
  free(S1);
  return 0;
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
    {"copy", buffer_copy},
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
  REG_NUMBER(BUFFER_TYPE_LONG, "long");
  REG_NUMBER(BUFFER_TYPE_FLOAT, "float");
  REG_NUMBER(BUFFER_TYPE_DOUBLE, "double");
#undef REG_NUMBER

  return 1;
}
