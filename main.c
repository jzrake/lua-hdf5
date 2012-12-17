

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


int luaopen_h5lua(lua_State *L);
int luaopen_buffer(lua_State *L);
int luaopen_buffer_view(lua_State *L) // pure Lua extension to buffer module
{
  int err = luaL_dofile(L, "view.lua");
  if (err) {
    const char *msg = lua_tostring(L, -1);
    luaL_error(L, msg);
  }
  return 0;
}


int main(int argc, char **argv)
{
  int n;
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_requiref(L, "h5lua", luaopen_h5lua, 0);
  luaL_requiref(L, "buffer", luaopen_buffer, 0);

  luaopen_buffer_view(L);


  // Create the global `arg` table
  // ---------------------------------------------------------------------------
  lua_newtable(L);
  for (n=0; n<argc; ++n) {
    lua_pushstring(L, argv[n]);
    lua_rawseti(L, -2, n);
  }
  lua_setglobal(L, "arg");


  // Run the script
  // ---------------------------------------------------------------------------
  if (argc == 1) {
    printf("usage: main script.lua [arg1=val1 arg2=val2]\n");
  }
  else {
    if (luaL_dofile(L, argv[1])) {
      printf("%s\n", lua_tostring(L, -1));
    }
  }

  lua_close(L);
  return 0;
}
