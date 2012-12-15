
static int h5lua_H5Acreate2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *attr_name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t acpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t res = H5Acreate2(loc_id, attr_name, type_id, space_id, acpl_id, aapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Acreate_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t acpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 7, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 8, "HDF5::hid_t"));
  hid_t res = H5Acreate_by_name(loc_id, obj_name, attr_name, type_id, space_id, acpl_id, aapl_id, lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aopen(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *attr_name = luaL_checkstring(L, 2);
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Aopen(obj_id, attr_name, aapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aopen_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t aapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Aopen_by_name(loc_id, obj_name, attr_name, aapl_id, lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Awrite(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Awrite(attr_id, type_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Aread(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Aread(attr_id, type_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Aclose(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Aclose(attr_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Aget_space(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Aget_space(attr_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aget_type(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Aget_type(attr_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aget_create_plist(lua_State *L)
{
  hid_t attr_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Aget_create_plist(attr_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Arename(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *old_name = luaL_checkstring(L, 2);
  const char *new_name = luaL_checkstring(L, 3);
  herr_t res = H5Arename(loc_id, old_name, new_name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Arename_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *old_attr_name = luaL_checkstring(L, 3);
  const char *new_attr_name = luaL_checkstring(L, 4);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Arename_by_name(loc_id, obj_name, old_attr_name, new_attr_name, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Adelete(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Adelete(loc_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Adelete_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Adelete_by_name(loc_id, obj_name, attr_name, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Aexists(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *attr_name = luaL_checkstring(L, 2);
  htri_t res = H5Aexists(obj_id, attr_name);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Aexists_by_name(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *obj_name = luaL_checkstring(L, 2);
  const char *attr_name = luaL_checkstring(L, 3);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  htri_t res = H5Aexists_by_name(obj_id, obj_name, attr_name, lapl_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Acreate1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t acpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Acreate1(loc_id, name, type_id, space_id, acpl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aopen_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Aopen_name(loc_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aopen_idx(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned idx = luaL_checkunsigned(L, 2);
  hid_t res = H5Aopen_idx(loc_id, idx);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Aget_num_attrs(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Aget_num_attrs(loc_id);
  lua_pushnumber(L, res);
  return 1;
}
static luaL_Reg H5A_funcs[] = {
  {"H5Acreate2", h5lua_H5Acreate2},
  {"H5Acreate_by_name", h5lua_H5Acreate_by_name},
  {"H5Aopen", h5lua_H5Aopen},
  {"H5Aopen_by_name", h5lua_H5Aopen_by_name},
  {"H5Awrite", h5lua_H5Awrite},
  {"H5Aread", h5lua_H5Aread},
  {"H5Aclose", h5lua_H5Aclose},
  {"H5Aget_space", h5lua_H5Aget_space},
  {"H5Aget_type", h5lua_H5Aget_type},
  {"H5Aget_create_plist", h5lua_H5Aget_create_plist},
  {"H5Arename", h5lua_H5Arename},
  {"H5Arename_by_name", h5lua_H5Arename_by_name},
  {"H5Adelete", h5lua_H5Adelete},
  {"H5Adelete_by_name", h5lua_H5Adelete_by_name},
  {"H5Aexists", h5lua_H5Aexists},
  {"H5Aexists_by_name", h5lua_H5Aexists_by_name},
  {"H5Acreate1", h5lua_H5Acreate1},
  {"H5Aopen_name", h5lua_H5Aopen_name},
  {"H5Aopen_idx", h5lua_H5Aopen_idx},
  {"H5Aget_num_attrs", h5lua_H5Aget_num_attrs},
  {NULL,NULL}};

static int h5lua_H5Dcreate2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t dcpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 7, "HDF5::hid_t"));
  hid_t res = H5Dcreate2(loc_id, name, type_id, space_id, lcpl_id, dcpl_id, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dcreate_anon(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Dcreate_anon(file_id, type_id, space_id, plist_id, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dopen2(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Dopen2(file_id, name, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dclose(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Dclose(dset_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dget_space(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_space(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dget_type(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_type(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dget_create_plist(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_create_plist(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dget_access_plist(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Dget_access_plist(dset_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dread(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t mem_type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t mem_space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t file_space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 6); luaL_checktype(L, 6, LUA_TUSERDATA);
  herr_t res = H5Dread(dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dwrite(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t mem_type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t mem_space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t file_space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  const void *buf = lua_touserdata(L, 6); luaL_checktype(L, 6, LUA_TUSERDATA);
  herr_t res = H5Dwrite(dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dvlen_reclaim(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  herr_t res = H5Dvlen_reclaim(type_id, space_id, plist_id, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dvlen_get_buf_size(lua_State *L)
{
  hid_t dataset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Dvlen_get_buf_size(dataset_id, type_id, space_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dfill(lua_State *L)
{
  const void *fill = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t fill_type = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  hid_t buf_type = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t space = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Dfill(fill, fill_type, buf, buf_type, space);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dset_extent(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Dset_extent(dset_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Ddebug(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Ddebug(dset_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Dcreate1(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t dcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Dcreate1(file_id, name, type_id, space_id, dcpl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dopen1(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Dopen1(file_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Dextend(lua_State *L)
{
  hid_t dset_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Dextend(dset_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
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
static int h5lua_H5Dopen(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t dapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Dopen(loc_id, name, dapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static luaL_Reg H5D_funcs[] = {
  {"H5Dcreate2", h5lua_H5Dcreate2},
  {"H5Dcreate_anon", h5lua_H5Dcreate_anon},
  {"H5Dopen2", h5lua_H5Dopen2},
  {"H5Dclose", h5lua_H5Dclose},
  {"H5Dget_space", h5lua_H5Dget_space},
  {"H5Dget_type", h5lua_H5Dget_type},
  {"H5Dget_create_plist", h5lua_H5Dget_create_plist},
  {"H5Dget_access_plist", h5lua_H5Dget_access_plist},
  {"H5Dread", h5lua_H5Dread},
  {"H5Dwrite", h5lua_H5Dwrite},
  {"H5Dvlen_reclaim", h5lua_H5Dvlen_reclaim},
  {"H5Dvlen_get_buf_size", h5lua_H5Dvlen_get_buf_size},
  {"H5Dfill", h5lua_H5Dfill},
  {"H5Dset_extent", h5lua_H5Dset_extent},
  {"H5Ddebug", h5lua_H5Ddebug},
  {"H5Dcreate1", h5lua_H5Dcreate1},
  {"H5Dopen1", h5lua_H5Dopen1},
  {"H5Dextend", h5lua_H5Dextend},
  {"H5Dcreate", h5lua_H5Dcreate},
  {"H5Dopen", h5lua_H5Dopen},
  {NULL,NULL}};

static int h5lua_H5Eregister_class(lua_State *L)
{
  const char *cls_name = luaL_checkstring(L, 1);
  const char *lib_name = luaL_checkstring(L, 2);
  const char *version = luaL_checkstring(L, 3);
  hid_t res = H5Eregister_class(cls_name, lib_name, version);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Eunregister_class(lua_State *L)
{
  hid_t class_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eunregister_class(class_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Eclose_msg(lua_State *L)
{
  hid_t err_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eclose_msg(err_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Eclose_stack(lua_State *L)
{
  hid_t stack_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eclose_stack(stack_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Eset_current_stack(lua_State *L)
{
  hid_t err_stack_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eset_current_stack(err_stack_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Epop(lua_State *L)
{
  hid_t err_stack = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t count = luaL_checkunsigned(L, 2);
  herr_t res = H5Epop(err_stack, count);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Eclear2(lua_State *L)
{
  hid_t err_stack = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Eclear2(err_stack);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Eget_num(lua_State *L)
{
  hid_t error_stack_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  ssize_t res = H5Eget_num(error_stack_id);
  lua_pushnumber(L, res);
  return 1;
}
static luaL_Reg H5E_funcs[] = {
  {"H5Eregister_class", h5lua_H5Eregister_class},
  {"H5Eunregister_class", h5lua_H5Eunregister_class},
  {"H5Eclose_msg", h5lua_H5Eclose_msg},
  {"H5Eclose_stack", h5lua_H5Eclose_stack},
  {"H5Eset_current_stack", h5lua_H5Eset_current_stack},
  {"H5Epop", h5lua_H5Epop},
  {"H5Eclear2", h5lua_H5Eclear2},
  {"H5Eget_num", h5lua_H5Eget_num},
  {NULL,NULL}};

static int h5lua_H5Fis_hdf5(lua_State *L)
{
  const char *filename = luaL_checkstring(L, 1);
  htri_t res = H5Fis_hdf5(filename);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Fcreate(lua_State *L)
{
  const char *filename = luaL_checkstring(L, 1);
  unsigned flags = luaL_checkunsigned(L, 2);
  hid_t create_plist = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t access_plist = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t res = H5Fcreate(filename, flags, create_plist, access_plist);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Fopen(lua_State *L)
{
  const char *filename = luaL_checkstring(L, 1);
  unsigned flags = luaL_checkunsigned(L, 2);
  hid_t access_plist = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Fopen(filename, flags, access_plist);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Freopen(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Freopen(file_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Fclose(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Fclose(file_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Fget_create_plist(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Fget_create_plist(file_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Fget_access_plist(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Fget_access_plist(file_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Fget_obj_count(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned types = luaL_checkunsigned(L, 2);
  ssize_t res = H5Fget_obj_count(file_id, types);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Fmount(lua_State *L)
{
  hid_t loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t child = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t plist = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Fmount(loc, name, child, plist);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Funmount(lua_State *L)
{
  hid_t loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Funmount(loc, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Fget_filesize(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Fget_filesize(file_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Freset_mdc_hit_rate_stats(lua_State *L)
{
  hid_t file_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Freset_mdc_hit_rate_stats(file_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5F_funcs[] = {
  {"H5Fis_hdf5", h5lua_H5Fis_hdf5},
  {"H5Fcreate", h5lua_H5Fcreate},
  {"H5Fopen", h5lua_H5Fopen},
  {"H5Freopen", h5lua_H5Freopen},
  {"H5Fclose", h5lua_H5Fclose},
  {"H5Fget_create_plist", h5lua_H5Fget_create_plist},
  {"H5Fget_access_plist", h5lua_H5Fget_access_plist},
  {"H5Fget_obj_count", h5lua_H5Fget_obj_count},
  {"H5Fmount", h5lua_H5Fmount},
  {"H5Funmount", h5lua_H5Funmount},
  {"H5Fget_filesize", h5lua_H5Fget_filesize},
  {"H5Freset_mdc_hit_rate_stats", h5lua_H5Freset_mdc_hit_rate_stats},
  {NULL,NULL}};

static int h5lua_H5Gcreate2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t gcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t gapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t res = H5Gcreate2(loc_id, name, lcpl_id, gcpl_id, gapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Gcreate_anon(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t gcpl_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t gapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Gcreate_anon(loc_id, gcpl_id, gapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Gopen2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t gapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Gopen2(loc_id, name, gapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Gget_create_plist(lua_State *L)
{
  hid_t group_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Gget_create_plist(group_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Gclose(lua_State *L)
{
  hid_t group_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Gclose(group_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Gcreate1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  size_t size_hint = luaL_checkunsigned(L, 3);
  hid_t res = H5Gcreate1(loc_id, name, size_hint);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Gopen1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Gopen1(loc_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Gmove(lua_State *L)
{
  hid_t src_loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  const char *dst_name = luaL_checkstring(L, 3);
  herr_t res = H5Gmove(src_loc_id, src_name, dst_name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Gmove2(lua_State *L)
{
  hid_t src_loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  herr_t res = H5Gmove2(src_loc_id, src_name, dst_loc_id, dst_name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Gunlink(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Gunlink(loc_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Gset_comment(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  const char *comment = luaL_checkstring(L, 3);
  herr_t res = H5Gset_comment(loc_id, name, comment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Gget_num_objs(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *num_objs = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Gget_num_objs(loc_id, num_objs);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5G_funcs[] = {
  {"H5Gcreate2", h5lua_H5Gcreate2},
  {"H5Gcreate_anon", h5lua_H5Gcreate_anon},
  {"H5Gopen2", h5lua_H5Gopen2},
  {"H5Gget_create_plist", h5lua_H5Gget_create_plist},
  {"H5Gclose", h5lua_H5Gclose},
  {"H5Gcreate1", h5lua_H5Gcreate1},
  {"H5Gopen1", h5lua_H5Gopen1},
  {"H5Gmove", h5lua_H5Gmove},
  {"H5Gmove2", h5lua_H5Gmove2},
  {"H5Gunlink", h5lua_H5Gunlink},
  {"H5Gset_comment", h5lua_H5Gset_comment},
  {"H5Gget_num_objs", h5lua_H5Gget_num_objs},
  {NULL,NULL}};

static int h5lua_H5Iget_file_id(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Iget_file_id(id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Iinc_ref(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Iinc_ref(id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Idec_ref(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Idec_ref(id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Iget_ref(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Iget_ref(id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Iis_valid(lua_State *L)
{
  hid_t id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Iis_valid(id);
  lua_pushboolean(L, res);
  return 1;
}
static luaL_Reg H5I_funcs[] = {
  {"H5Iget_file_id", h5lua_H5Iget_file_id},
  {"H5Iinc_ref", h5lua_H5Iinc_ref},
  {"H5Idec_ref", h5lua_H5Idec_ref},
  {"H5Iget_ref", h5lua_H5Iget_ref},
  {"H5Iis_valid", h5lua_H5Iis_valid},
  {NULL,NULL}};

static int h5lua_H5Lmove(lua_State *L)
{
  hid_t src_loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lmove(src_loc, src_name, dst_loc, dst_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Lcopy(lua_State *L)
{
  hid_t src_loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lcopy(src_loc, src_name, dst_loc, dst_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Lcreate_hard(lua_State *L)
{
  hid_t cur_loc = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *cur_name = luaL_checkstring(L, 2);
  hid_t dst_loc = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lcreate_hard(cur_loc, cur_name, dst_loc, dst_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Lcreate_soft(lua_State *L)
{
  const char *link_target = luaL_checkstring(L, 1);
  hid_t link_loc_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *link_name = luaL_checkstring(L, 3);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Lcreate_soft(link_target, link_loc_id, link_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Ldelete(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  herr_t res = H5Ldelete(loc_id, name, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Lget_val(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *buf = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  size_t size = luaL_checkunsigned(L, 4);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Lget_val(loc_id, name, buf, size, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Lexists(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  htri_t res = H5Lexists(loc_id, name, lapl_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Lcreate_external(lua_State *L)
{
  const char *file_name = luaL_checkstring(L, 1);
  const char *obj_name = luaL_checkstring(L, 2);
  hid_t link_loc_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *link_name = luaL_checkstring(L, 4);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Lcreate_external(file_name, obj_name, link_loc_id, link_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5L_funcs[] = {
  {"H5Lmove", h5lua_H5Lmove},
  {"H5Lcopy", h5lua_H5Lcopy},
  {"H5Lcreate_hard", h5lua_H5Lcreate_hard},
  {"H5Lcreate_soft", h5lua_H5Lcreate_soft},
  {"H5Ldelete", h5lua_H5Ldelete},
  {"H5Lget_val", h5lua_H5Lget_val},
  {"H5Lexists", h5lua_H5Lexists},
  {"H5Lcreate_external", h5lua_H5Lcreate_external},
  {NULL,NULL}};

static int h5lua_H5Oopen(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Oopen(loc_id, name, lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Oget_info(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  H5O_info_t *oinfo = (H5O_info_t*) luaL_checkudata(L, 2, "HDF5::H5O_info_t");
  herr_t res = H5Oget_info(loc_id, oinfo);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Oget_info_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  H5O_info_t *oinfo = (H5O_info_t*) luaL_checkudata(L, 3, "HDF5::H5O_info_t");
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Oget_info_by_name(loc_id, name, oinfo, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Olink(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t new_loc_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *new_name = luaL_checkstring(L, 3);
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Olink(obj_id, new_loc_id, new_name, lcpl_id, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Oincr_refcount(lua_State *L)
{
  hid_t object_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Oincr_refcount(object_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Odecr_refcount(lua_State *L)
{
  hid_t object_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Odecr_refcount(object_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Ocopy(lua_State *L)
{
  hid_t src_loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *src_name = luaL_checkstring(L, 2);
  hid_t dst_loc_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  const char *dst_name = luaL_checkstring(L, 4);
  hid_t ocpypl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Ocopy(src_loc_id, src_name, dst_loc_id, dst_name, ocpypl_id, lcpl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Oset_comment(lua_State *L)
{
  hid_t obj_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *comment = luaL_checkstring(L, 2);
  herr_t res = H5Oset_comment(obj_id, comment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Oset_comment_by_name(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  const char *comment = luaL_checkstring(L, 3);
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Oset_comment_by_name(loc_id, name, comment, lapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Oclose(lua_State *L)
{
  hid_t object_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Oclose(object_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5O_funcs[] = {
  {"H5Oopen", h5lua_H5Oopen},
  {"H5Oget_info", h5lua_H5Oget_info},
  {"H5Oget_info_by_name", h5lua_H5Oget_info_by_name},
  {"H5Olink", h5lua_H5Olink},
  {"H5Oincr_refcount", h5lua_H5Oincr_refcount},
  {"H5Odecr_refcount", h5lua_H5Odecr_refcount},
  {"H5Ocopy", h5lua_H5Ocopy},
  {"H5Oset_comment", h5lua_H5Oset_comment},
  {"H5Oset_comment_by_name", h5lua_H5Oset_comment_by_name},
  {"H5Oclose", h5lua_H5Oclose},
  {NULL,NULL}};

static int h5lua_H5Pcreate(lua_State *L)
{
  hid_t cls_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pcreate(cls_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Pset(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pset(plist_id, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pexist(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  htri_t res = H5Pexist(plist_id, name);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Pget_class(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_class(plist_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Pget_class_parent(lua_State *L)
{
  hid_t pclass_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_class_parent(pclass_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Pget(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pget(plist_id, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pequal(lua_State *L)
{
  hid_t id1 = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t id2 = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Pequal(id1, id2);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Pisa_class(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t pclass_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Pisa_class(plist_id, pclass_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Pcopy_prop(lua_State *L)
{
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 3);
  herr_t res = H5Pcopy_prop(dst_id, src_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Premove(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Premove(plist_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Punregister(lua_State *L)
{
  hid_t pclass_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  herr_t res = H5Punregister(pclass_id, name);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pclose_class(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pclose_class(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pclose(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pclose(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pcopy(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pcopy(plist_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Pset_attr_phase_change(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned max_compact = luaL_checkunsigned(L, 2);
  unsigned min_dense = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_attr_phase_change(plist_id, max_compact, min_dense);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_attr_creation_order(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_order_flags = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_attr_creation_order(plist_id, crt_order_flags);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_userblock(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_userblock(plist_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_userblock(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_userblock(plist_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_sizes(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t sizeof_addr = luaL_checkunsigned(L, 2);
  size_t sizeof_size = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_sizes(plist_id, sizeof_addr, sizeof_size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_sym_k(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned ik = luaL_checkunsigned(L, 2);
  unsigned lk = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_sym_k(plist_id, ik, lk);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_istore_k(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned ik = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_istore_k(plist_id, ik);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_shared_mesg_nindexes(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned nindexes = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_shared_mesg_nindexes(plist_id, nindexes);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_shared_mesg_index(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned index_num = luaL_checkunsigned(L, 2);
  unsigned mesg_type_flags = luaL_checkunsigned(L, 3);
  unsigned min_mesg_size = luaL_checkunsigned(L, 4);
  herr_t res = H5Pset_shared_mesg_index(plist_id, index_num, mesg_type_flags, min_mesg_size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_shared_mesg_phase_change(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned max_list = luaL_checkunsigned(L, 2);
  unsigned min_btree = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_shared_mesg_phase_change(plist_id, max_list, min_btree);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_alignment(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t threshold = luaL_checkunsigned(L, 2);
  hsize_t alignment = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_alignment(fapl_id, threshold, alignment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_alignment(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *threshold = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  hsize_t *alignment = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_alignment(fapl_id, threshold, alignment);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_driver(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t driver_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const void *driver_info = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pset_driver(plist_id, driver_id, driver_info);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_driver(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_driver(plist_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Pset_family_offset(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t offset = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_family_offset(fapl_id, offset);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_family_offset(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *offset = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_family_offset(fapl_id, offset);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_gc_references(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned gc_ref = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_gc_references(fapl_id, gc_ref);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_meta_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_meta_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_meta_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_meta_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_sieve_buf_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_sieve_buf_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_small_data_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_small_data_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_small_data_block_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *size = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  herr_t res = H5Pget_small_data_block_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_external_count(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Pget_external_count(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Pget_nfilters(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Pget_nfilters(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Pall_filters_avail(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Pall_filters_avail(plist_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Pset_deflate(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned aggression = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_deflate(plist_id, aggression);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_szip(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned options_mask = luaL_checkunsigned(L, 2);
  unsigned pixels_per_block = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_szip(plist_id, options_mask, pixels_per_block);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_shuffle(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pset_shuffle(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_nbit(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pset_nbit(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_fletcher32(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Pset_fletcher32(plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_fill_value(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pset_fill_value(plist_id, type_id, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_fill_value(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Pget_fill_value(plist_id, type_id, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_buffer(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  void *tconv = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  void *bkg = lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  herr_t res = H5Pset_buffer(plist_id, size, tconv, bkg);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_preserve(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Pget_preserve(plist_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Pset_hyper_vector_size(lua_State *L)
{
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_hyper_vector_size(fapl_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_create_intermediate_group(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_intmd = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_create_intermediate_group(plist_id, crt_intmd);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_local_heap_size_hint(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size_hint = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_local_heap_size_hint(plist_id, size_hint);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_link_phase_change(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned max_compact = luaL_checkunsigned(L, 2);
  unsigned min_dense = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_link_phase_change(plist_id, max_compact, min_dense);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_est_link_info(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned est_num_entries = luaL_checkunsigned(L, 2);
  unsigned est_name_len = luaL_checkunsigned(L, 3);
  herr_t res = H5Pset_est_link_info(plist_id, est_num_entries, est_name_len);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_link_creation_order(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_order_flags = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_link_creation_order(plist_id, crt_order_flags);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_nlinks(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t nlinks = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_nlinks(plist_id, nlinks);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_elink_prefix(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *prefix = luaL_checkstring(L, 2);
  herr_t res = H5Pset_elink_prefix(plist_id, prefix);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pget_elink_fapl(lua_State *L)
{
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Pget_elink_fapl(lapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Pset_elink_fapl(lua_State *L)
{
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t fapl_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  herr_t res = H5Pset_elink_fapl(lapl_id, fapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_elink_acc_flags(lua_State *L)
{
  hid_t lapl_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned flags = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_elink_acc_flags(lapl_id, flags);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Pset_copy_object(lua_State *L)
{
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned crt_intmd = luaL_checkunsigned(L, 2);
  herr_t res = H5Pset_copy_object(plist_id, crt_intmd);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5P_funcs[] = {
  {"H5Pcreate", h5lua_H5Pcreate},
  {"H5Pset", h5lua_H5Pset},
  {"H5Pexist", h5lua_H5Pexist},
  {"H5Pget_class", h5lua_H5Pget_class},
  {"H5Pget_class_parent", h5lua_H5Pget_class_parent},
  {"H5Pget", h5lua_H5Pget},
  {"H5Pequal", h5lua_H5Pequal},
  {"H5Pisa_class", h5lua_H5Pisa_class},
  {"H5Pcopy_prop", h5lua_H5Pcopy_prop},
  {"H5Premove", h5lua_H5Premove},
  {"H5Punregister", h5lua_H5Punregister},
  {"H5Pclose_class", h5lua_H5Pclose_class},
  {"H5Pclose", h5lua_H5Pclose},
  {"H5Pcopy", h5lua_H5Pcopy},
  {"H5Pset_attr_phase_change", h5lua_H5Pset_attr_phase_change},
  {"H5Pset_attr_creation_order", h5lua_H5Pset_attr_creation_order},
  {"H5Pset_userblock", h5lua_H5Pset_userblock},
  {"H5Pget_userblock", h5lua_H5Pget_userblock},
  {"H5Pset_sizes", h5lua_H5Pset_sizes},
  {"H5Pset_sym_k", h5lua_H5Pset_sym_k},
  {"H5Pset_istore_k", h5lua_H5Pset_istore_k},
  {"H5Pset_shared_mesg_nindexes", h5lua_H5Pset_shared_mesg_nindexes},
  {"H5Pset_shared_mesg_index", h5lua_H5Pset_shared_mesg_index},
  {"H5Pset_shared_mesg_phase_change", h5lua_H5Pset_shared_mesg_phase_change},
  {"H5Pset_alignment", h5lua_H5Pset_alignment},
  {"H5Pget_alignment", h5lua_H5Pget_alignment},
  {"H5Pset_driver", h5lua_H5Pset_driver},
  {"H5Pget_driver", h5lua_H5Pget_driver},
  {"H5Pset_family_offset", h5lua_H5Pset_family_offset},
  {"H5Pget_family_offset", h5lua_H5Pget_family_offset},
  {"H5Pset_gc_references", h5lua_H5Pset_gc_references},
  {"H5Pset_meta_block_size", h5lua_H5Pset_meta_block_size},
  {"H5Pget_meta_block_size", h5lua_H5Pget_meta_block_size},
  {"H5Pset_sieve_buf_size", h5lua_H5Pset_sieve_buf_size},
  {"H5Pset_small_data_block_size", h5lua_H5Pset_small_data_block_size},
  {"H5Pget_small_data_block_size", h5lua_H5Pget_small_data_block_size},
  {"H5Pget_external_count", h5lua_H5Pget_external_count},
  {"H5Pget_nfilters", h5lua_H5Pget_nfilters},
  {"H5Pall_filters_avail", h5lua_H5Pall_filters_avail},
  {"H5Pset_deflate", h5lua_H5Pset_deflate},
  {"H5Pset_szip", h5lua_H5Pset_szip},
  {"H5Pset_shuffle", h5lua_H5Pset_shuffle},
  {"H5Pset_nbit", h5lua_H5Pset_nbit},
  {"H5Pset_fletcher32", h5lua_H5Pset_fletcher32},
  {"H5Pset_fill_value", h5lua_H5Pset_fill_value},
  {"H5Pget_fill_value", h5lua_H5Pget_fill_value},
  {"H5Pset_buffer", h5lua_H5Pset_buffer},
  {"H5Pget_preserve", h5lua_H5Pget_preserve},
  {"H5Pset_hyper_vector_size", h5lua_H5Pset_hyper_vector_size},
  {"H5Pset_create_intermediate_group", h5lua_H5Pset_create_intermediate_group},
  {"H5Pset_local_heap_size_hint", h5lua_H5Pset_local_heap_size_hint},
  {"H5Pset_link_phase_change", h5lua_H5Pset_link_phase_change},
  {"H5Pset_est_link_info", h5lua_H5Pset_est_link_info},
  {"H5Pset_link_creation_order", h5lua_H5Pset_link_creation_order},
  {"H5Pset_nlinks", h5lua_H5Pset_nlinks},
  {"H5Pset_elink_prefix", h5lua_H5Pset_elink_prefix},
  {"H5Pget_elink_fapl", h5lua_H5Pget_elink_fapl},
  {"H5Pset_elink_fapl", h5lua_H5Pset_elink_fapl},
  {"H5Pset_elink_acc_flags", h5lua_H5Pset_elink_acc_flags},
  {"H5Pset_copy_object", h5lua_H5Pset_copy_object},
  {NULL,NULL}};

static int h5lua_H5Rcreate(lua_State *L)
{
  void *ref = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 3);
  int ref_type = luaL_checkinteger(L, 4);
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  herr_t res = H5Rcreate(ref, loc_id, name, ref_type, space_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Rdereference(lua_State *L)
{
  hid_t dataset = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int ref_type = luaL_checkinteger(L, 2);
  const void *ref = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  hid_t res = H5Rdereference(dataset, ref_type, ref);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Rget_region(lua_State *L)
{
  hid_t dataset = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int ref_type = luaL_checkinteger(L, 2);
  const void *ref = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  hid_t res = H5Rget_region(dataset, ref_type, ref);
  lh5_push_hid_t(L, res);
  return 1;
}
static luaL_Reg H5R_funcs[] = {
  {"H5Rcreate", h5lua_H5Rcreate},
  {"H5Rdereference", h5lua_H5Rdereference},
  {"H5Rget_region", h5lua_H5Rget_region},
  {NULL,NULL}};

static int h5lua_H5Screate(lua_State *L)
{
  int type = luaL_checkinteger(L, 1);
  hid_t res = H5Screate(type);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Scopy(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Scopy(space_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Sclose(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sclose(space_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sdecode(lua_State *L)
{
  const void *buf = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t res = H5Sdecode(buf);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Sget_simple_extent_ndims(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Sget_simple_extent_ndims(space_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Sget_simple_extent_dims(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *dims = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  hsize_t *maxdims = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  int res = H5Sget_simple_extent_dims(space_id, dims, maxdims);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Sis_simple(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Sis_simple(space_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Sselect_hyperslab(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int op = luaL_checkinteger(L, 2);
  const hsize_t *start = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  const hsize_t *_stride = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  const hsize_t *count = (hsize_t*) luaL_checkudata(L, 5, "HDF5::hsize_t_arr");
  const hsize_t *_block = (hsize_t*) luaL_checkudata(L, 6, "HDF5::hsize_t_arr");
  herr_t res = H5Sselect_hyperslab(space_id, op, start, _stride, count, _block);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sselect_elements(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int op = luaL_checkinteger(L, 2);
  size_t num_elem = luaL_checkunsigned(L, 3);
  const hsize_t *coord = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sselect_elements(space_id, op, num_elem, coord);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sset_extent_none(lua_State *L)
{
  hid_t space_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sset_extent_none(space_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sextent_copy(lua_State *L)
{
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  herr_t res = H5Sextent_copy(dst_id, src_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sextent_equal(lua_State *L)
{
  hid_t sid1 = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t sid2 = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Sextent_equal(sid1, sid2);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Sselect_all(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sselect_all(spaceid);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sselect_none(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Sselect_none(spaceid);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sselect_valid(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Sselect_valid(spaceid);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Sget_select_hyper_blocklist(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t startblock = luaL_checkunsigned(L, 2);
  hsize_t numblocks = luaL_checkunsigned(L, 3);
  hsize_t *buf = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sget_select_hyper_blocklist(spaceid, startblock, numblocks, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sget_select_elem_pointlist(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t startpoint = luaL_checkunsigned(L, 2);
  hsize_t numpoints = luaL_checkunsigned(L, 3);
  hsize_t *buf = (hsize_t*) luaL_checkudata(L, 4, "HDF5::hsize_t_arr");
  herr_t res = H5Sget_select_elem_pointlist(spaceid, startpoint, numpoints, buf);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Sget_select_bounds(lua_State *L)
{
  hid_t spaceid = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *start = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  hsize_t *end = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  herr_t res = H5Sget_select_bounds(spaceid, start, end);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5S_funcs[] = {
  {"H5Screate", h5lua_H5Screate},
  {"H5Scopy", h5lua_H5Scopy},
  {"H5Sclose", h5lua_H5Sclose},
  {"H5Sdecode", h5lua_H5Sdecode},
  {"H5Sget_simple_extent_ndims", h5lua_H5Sget_simple_extent_ndims},
  {"H5Sget_simple_extent_dims", h5lua_H5Sget_simple_extent_dims},
  {"H5Sis_simple", h5lua_H5Sis_simple},
  {"H5Sselect_hyperslab", h5lua_H5Sselect_hyperslab},
  {"H5Sselect_elements", h5lua_H5Sselect_elements},
  {"H5Sset_extent_none", h5lua_H5Sset_extent_none},
  {"H5Sextent_copy", h5lua_H5Sextent_copy},
  {"H5Sextent_equal", h5lua_H5Sextent_equal},
  {"H5Sselect_all", h5lua_H5Sselect_all},
  {"H5Sselect_none", h5lua_H5Sselect_none},
  {"H5Sselect_valid", h5lua_H5Sselect_valid},
  {"H5Sget_select_hyper_blocklist", h5lua_H5Sget_select_hyper_blocklist},
  {"H5Sget_select_elem_pointlist", h5lua_H5Sget_select_elem_pointlist},
  {"H5Sget_select_bounds", h5lua_H5Sget_select_bounds},
  {NULL,NULL}};

static int h5lua_H5Tcopy(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tcopy(type_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tclose(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Tclose(type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tequal(lua_State *L)
{
  hid_t type1_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type2_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Tequal(type1_id, type2_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Tlock(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Tlock(type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tcommit2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t lcpl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  hid_t tcpl_id = *((hid_t*) luaL_checkudata(L, 5, "HDF5::hid_t"));
  hid_t tapl_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Tcommit2(loc_id, name, type_id, lcpl_id, tcpl_id, tapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Topen2(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t tapl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t res = H5Topen2(loc_id, name, tapl_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tcommit_anon(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  hid_t tcpl_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  hid_t tapl_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Tcommit_anon(loc_id, type_id, tcpl_id, tapl_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tget_create_plist(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tget_create_plist(type_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tcommitted(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Tcommitted(type_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Tdecode(lua_State *L)
{
  const void *buf = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  hid_t res = H5Tdecode(buf);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tinsert(lua_State *L)
{
  hid_t parent_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  size_t offset = luaL_checkunsigned(L, 3);
  hid_t member_id = *((hid_t*) luaL_checkudata(L, 4, "HDF5::hid_t"));
  herr_t res = H5Tinsert(parent_id, name, offset, member_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tpack(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  herr_t res = H5Tpack(type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tenum_create(lua_State *L)
{
  hid_t base_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tenum_create(base_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tenum_insert(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  const void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Tenum_insert(type, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tenum_valueof(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Tenum_valueof(type, name, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tvlen_create(lua_State *L)
{
  hid_t base_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tvlen_create(base_id);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tarray_create2(lua_State *L)
{
  hid_t base_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned ndims = luaL_checkunsigned(L, 2);
  const hsize_t *dim = (hsize_t*) luaL_checkudata(L, 3, "HDF5::hsize_t_arr");
  hid_t res = H5Tarray_create2(base_id, ndims, dim);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tget_array_ndims(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Tget_array_ndims(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_array_dims2(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hsize_t *dims = (hsize_t*) luaL_checkudata(L, 2, "HDF5::hsize_t_arr");
  int res = H5Tget_array_dims2(type_id, dims);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tset_tag(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *tag = luaL_checkstring(L, 2);
  herr_t res = H5Tset_tag(type, tag);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tget_super(lua_State *L)
{
  hid_t type = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t res = H5Tget_super(type);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tget_size(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t res = H5Tget_size(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_precision(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t res = H5Tget_precision(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_offset(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Tget_offset(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_ebias(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t res = H5Tget_ebias(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_nmembers(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  int res = H5Tget_nmembers(type_id);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_member_index(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  int res = H5Tget_member_index(type_id, name);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_member_offset(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  size_t res = H5Tget_member_offset(type_id, membno);
  lua_pushnumber(L, res);
  return 1;
}
static int h5lua_H5Tget_member_type(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  hid_t res = H5Tget_member_type(type_id, membno);
  lh5_push_hid_t(L, res);
  return 1;
}
static int h5lua_H5Tget_member_value(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  unsigned membno = luaL_checkunsigned(L, 2);
  void *value = lua_touserdata(L, 3); luaL_checktype(L, 3, LUA_TUSERDATA);
  herr_t res = H5Tget_member_value(type_id, membno, value);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tis_variable_str(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  htri_t res = H5Tis_variable_str(type_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Tset_size(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t size = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_size(type_id, size);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tset_precision(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t prec = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_precision(type_id, prec);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tset_offset(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t offset = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_offset(type_id, offset);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tset_fields(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t spos = luaL_checkunsigned(L, 2);
  size_t epos = luaL_checkunsigned(L, 3);
  size_t esize = luaL_checkunsigned(L, 4);
  size_t mpos = luaL_checkunsigned(L, 5);
  size_t msize = luaL_checkunsigned(L, 6);
  herr_t res = H5Tset_fields(type_id, spos, epos, esize, mpos, msize);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tset_ebias(lua_State *L)
{
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  size_t ebias = luaL_checkunsigned(L, 2);
  herr_t res = H5Tset_ebias(type_id, ebias);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tcompiler_conv(lua_State *L)
{
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  htri_t res = H5Tcompiler_conv(src_id, dst_id);
  lua_pushboolean(L, res);
  return 1;
}
static int h5lua_H5Tconvert(lua_State *L)
{
  hid_t src_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  hid_t dst_id = *((hid_t*) luaL_checkudata(L, 2, "HDF5::hid_t"));
  size_t nelmts = luaL_checkunsigned(L, 3);
  void *buf = lua_touserdata(L, 4); luaL_checktype(L, 4, LUA_TUSERDATA);
  void *background = lua_touserdata(L, 5); luaL_checktype(L, 5, LUA_TUSERDATA);
  hid_t plist_id = *((hid_t*) luaL_checkudata(L, 6, "HDF5::hid_t"));
  herr_t res = H5Tconvert(src_id, dst_id, nelmts, buf, background, plist_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Tcommit1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t type_id = *((hid_t*) luaL_checkudata(L, 3, "HDF5::hid_t"));
  herr_t res = H5Tcommit1(loc_id, name, type_id);
  lh5_push_herr_t(L, res);
  return 1;
}
static int h5lua_H5Topen1(lua_State *L)
{
  hid_t loc_id = *((hid_t*) luaL_checkudata(L, 1, "HDF5::hid_t"));
  const char *name = luaL_checkstring(L, 2);
  hid_t res = H5Topen1(loc_id, name);
  lh5_push_hid_t(L, res);
  return 1;
}
static luaL_Reg H5T_funcs[] = {
  {"H5Tcopy", h5lua_H5Tcopy},
  {"H5Tclose", h5lua_H5Tclose},
  {"H5Tequal", h5lua_H5Tequal},
  {"H5Tlock", h5lua_H5Tlock},
  {"H5Tcommit2", h5lua_H5Tcommit2},
  {"H5Topen2", h5lua_H5Topen2},
  {"H5Tcommit_anon", h5lua_H5Tcommit_anon},
  {"H5Tget_create_plist", h5lua_H5Tget_create_plist},
  {"H5Tcommitted", h5lua_H5Tcommitted},
  {"H5Tdecode", h5lua_H5Tdecode},
  {"H5Tinsert", h5lua_H5Tinsert},
  {"H5Tpack", h5lua_H5Tpack},
  {"H5Tenum_create", h5lua_H5Tenum_create},
  {"H5Tenum_insert", h5lua_H5Tenum_insert},
  {"H5Tenum_valueof", h5lua_H5Tenum_valueof},
  {"H5Tvlen_create", h5lua_H5Tvlen_create},
  {"H5Tarray_create2", h5lua_H5Tarray_create2},
  {"H5Tget_array_ndims", h5lua_H5Tget_array_ndims},
  {"H5Tget_array_dims2", h5lua_H5Tget_array_dims2},
  {"H5Tset_tag", h5lua_H5Tset_tag},
  {"H5Tget_super", h5lua_H5Tget_super},
  {"H5Tget_size", h5lua_H5Tget_size},
  {"H5Tget_precision", h5lua_H5Tget_precision},
  {"H5Tget_offset", h5lua_H5Tget_offset},
  {"H5Tget_ebias", h5lua_H5Tget_ebias},
  {"H5Tget_nmembers", h5lua_H5Tget_nmembers},
  {"H5Tget_member_index", h5lua_H5Tget_member_index},
  {"H5Tget_member_offset", h5lua_H5Tget_member_offset},
  {"H5Tget_member_type", h5lua_H5Tget_member_type},
  {"H5Tget_member_value", h5lua_H5Tget_member_value},
  {"H5Tis_variable_str", h5lua_H5Tis_variable_str},
  {"H5Tset_size", h5lua_H5Tset_size},
  {"H5Tset_precision", h5lua_H5Tset_precision},
  {"H5Tset_offset", h5lua_H5Tset_offset},
  {"H5Tset_fields", h5lua_H5Tset_fields},
  {"H5Tset_ebias", h5lua_H5Tset_ebias},
  {"H5Tcompiler_conv", h5lua_H5Tcompiler_conv},
  {"H5Tconvert", h5lua_H5Tconvert},
  {"H5Tcommit1", h5lua_H5Tcommit1},
  {"H5Topen1", h5lua_H5Topen1},
  {NULL,NULL}};

static int h5lua_H5Zregister(lua_State *L)
{
  const void *cls = lua_touserdata(L, 1); luaL_checktype(L, 1, LUA_TUSERDATA);
  herr_t res = H5Zregister(cls);
  lh5_push_herr_t(L, res);
  return 1;
}
static luaL_Reg H5Z_funcs[] = {
  {"H5Zregister", h5lua_H5Zregister},
  {NULL,NULL}};
