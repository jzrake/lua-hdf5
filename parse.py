
import re

class FunctionPrototype(object):
    def __init__(self, prototype):
        target = re.compile(r".*H5_DLL (\w+)\s+(\w+)(.*)")
        argtype = re.compile(r"(.*)\b(\w+)")
        m = target.match(prototype)

        ret_type, func_name, argstr = m.groups()
        self.func_name = func_name
        self.ret_type = ret_type
        self.arg_list = [ ]

        for a in argstr[1:-1].split(','):
            ma = argtype.match(a.strip())
            self.arg_list.append([g.strip() for g in ma.groups()])

    def show(self):
        print "function:", self.func_name
        print "\treturns:", self.ret_type
        print "\targlist:"
        for t,n in self.arg_list:
            print "\t\t %s[%s]" % (t,n)

    def write(self, outfile):
        get_lua_args = [ ]
        for n,arg in enumerate(self.arg_list):
            t, name = arg
            if t == "const char *":
                get_lua_args.append(
                    """const char *%s = luaL_checkstring(L, %d);""" % (
                        name, n + 1)
                    )
            elif t == "hid_t":
                get_lua_args.append(
                    """hid_t %s = *((hid_t*) luaL_checkudata(L, %d, "HDF5::hid_t"));""" % (
                        name, n + 1))
            elif t == "unsigned":
                get_lua_args.append(
                    """unsigned %s = luaL_checkunsigned(L, %d);""" % (
                        name, n + 1))
            elif t == "void *":
                get_lua_args.append(
                    """void *%s = lua_touserdata(L, %d); luaL_checktype(L, %d, LUA_TUSERDATA);""" % (
                        name, n + 1, n + 1))
            elif t == "H5S_class_t":
                get_lua_args.append(
                    """int %s = luaL_checkinteger(L, %d);""" % (
                        name, n + 1))
            else:
                raise ValueError("function %s could not be wrapped: "
                                 "arg type(s) not known" %
                                 self.func_name)
        if self.ret_type == "hid_t":
            ret_statement = """lh5_push_hid_t(L, res);"""
        elif self.ret_type == "herr_t":
            ret_statement = """lh5_push_herr_t(L, res);"""
        else:
            raise ValueError("function %s could not be wrapped: "
                             "return type not known" %
                             self.func_name)
        func = (
            """
static int h5lua_%(func_name)s(lua_State *L)
{
  %(lua_args)s
  %(ret_type)s res = %(func_name)s(%(arg_list)s);
  %(ret_statement)s
  return 1;
}""" % {'func_name': self.func_name,
        'ret_type': self.ret_type,
        'ret_statement': ret_statement,
        'arg_list': ', '.join([a[1] for a in self.arg_list]),
        'lua_args': '\n  '.join(get_lua_args)})
        outfile.write(func)


def header_functions(prefix, outfile=None, extras=[]):
    f = open("/Library/Science/hdf5/include/H5%spublic.h" % prefix)
    passed = [ ]
    failed = [ ]
    for line in f.read().split(';') + extras:
        line = "".join([c for c in line if c not in ["\n","\t"]])
        try:
            proto = FunctionPrototype(line)
            try:
                #proto.show()
                proto.write(outfile)
                passed.append(proto.func_name)
            except ValueError as e:
                print e
                failed.append(proto.func_name)
        except AttributeError as e:
            pass
    outfile.write(
        """\nstatic luaL_Reg H5%s_funcs[] = {\n  %s,\n  {NULL,NULL}};\n""" % (
            prefix, ',\n  '.join(["""{"%s", h5lua_%s}""" % (n, n)
                                for n in passed])))

def H5F():
    f = open("/Library/Science/hdf5/include/H5Fpublic.h")
    for line in f:
        target = re.compile(r"#define (H5F_\w+)\s*")
        m = target.match(line)
        if m:
            s = m.group(1)
            if "MPI" not in s:
                print "  REG_NUMBER(%s);" % s

def H5S():
    f = open("/Library/Science/hdf5/include/H5Spublic.h")    
    for line in f:
        target = re.compile(r"\s*(H5S_\w+).*")
        m = target.match(line)
        if m:
            s = m.group(1)
            print "  REG_NUMBER(%s);" % s

def H5T():
    f = open("/Library/Science/hdf5/include/H5Tpublic.h")    
    for line in f:
        target = re.compile(r"#define (H5T_\w+)\s*")
        m = target.match(line)
        if m:
            s = m.group(1)
            print "  REG_HID(%s);" % s

def H5P():
    f = open("/Library/Science/hdf5/include/H5Ppublic.h")    
    for line in f:
        target = re.compile(r"#define (H5P_\w+)\s*")
        m = target.match(line)
        if m:
            s = m.group(1)
            print "  REG_HID(%s);" % s

extras = {
    "A": ["H5_DLL hid_t H5Acreate( hid_t loc_id, const char *attr_name, "
          "hid_t type_id, hid_t space_id, hid_t acpl_id, hid_t aapl_id )"
          "H5_DLL herr_t H5Aiterate( hid_t loc_id, unsigned * idx, "
          "H5A_operator_t op, void *op_data )"],
    "D": ["H5_DLL hid_t H5Dcreate( hid_t loc_id, const char *name, "
          "hid_t dtype_id, hid_t space_id, hid_t lcpl_id, "
          "hid_t dcpl_id, hid_t dapl_id )",
          "H5_DLL hid_t H5Dopen( hid_t loc_id, const char *name, hid_t dapl_id )"]
}


# For collecting functions from header files:
# ----------------------------------------------------------
wrap = open("h5funcs.c", "w")
for s in "ADEFGILOPRSTZ":
    if s in "RZ": continue
    header_functions(s, outfile=wrap, extras=extras.get(s, []))


# For collecting constants from header files:
# ----------------------------------------------------------
#H5F()
#H5S()
#H5P()
#H5T()
