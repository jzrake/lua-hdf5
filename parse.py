
import re
import sys

if len(sys.argv) > 1:
    Makefile_in = sys.argv[1]
else:
    Makefile_in = "Makefile.in"

print "[HDF5 Parser] using Makefile.in:", Makefile_in
verbose = False # print more stuff

# Read the Makefile.in
Makefile_in_dict = dict([tuple(p) for p in
                         ["".join([c for c in x if c not in [" ", "\n"]]).split('=')
                          for x in [d for d in open(Makefile_in).readlines() if
                                    d.strip()]]])

hdf5_inc = Makefile_in_dict.get('HDF_HOME', '/usr/local') + "/include"
use_mpio = Makefile_in_dict.get('USE_MPIO', 'False') # Include MPI constants and functions
use_mpio = eval(use_mpio)
print "[HDF5 Parser] USE_MPIO:", use_mpio

# Function prototypes not to wrap
dont_wrap = ["H5Tenum_nameof",
             "H5Scombine_select",
             "H5Sselect_select",
             "H5Scombine_hyperslab"]


class FunctionPrototype(object):
    passed = [ ]
    failed = [ ]
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
            if "[]" in a:
                self.arg_list[-1][0] += " *"

    def show(self):
        print "function:", self.func_name
        print "\treturns:", self.ret_type
        print "\targlist:"
        for t,n in self.arg_list:
            print "\t\t %s[%s]" % (t,n)

    def write(self, outfile):
        get_lua_args = [ ]
        if self.func_name in dont_wrap:
            self.failed.append(self.func_name)
            raise ValueError("function %s could not be wrapped: "
                             "output char*" %
                             self.func_name)
        for n,arg in enumerate(self.arg_list):
            t, name = arg
            if False:
                pass
            elif t == "void *":
                get_lua_args.append(
                    "void *%s = lua_touserdata(L, %d); "
                    "luaL_checktype(L, %d, LUA_TUSERDATA);" % (name, n+1, n+1))
            elif t == "const void *":
                get_lua_args.append(
                    "const void *%s = lua_touserdata(L, %d); "
                    "luaL_checktype(L, %d, LUA_TUSERDATA);" % (name, n+1, n+1))
            elif t == "const char *":
                get_lua_args.append(
                    "const char *%s = luaL_checkstring(L, %d);" % (name, n+1))
            elif t == "hid_t":
                get_lua_args.append(
                    "hid_t %s = *((hid_t*) luaL_checkudata("
                    "L, %d, \"HDF5::hid_t\"));" % (name, n+1))
            elif t == "hsize_t *":
                get_lua_args.append(
                    "hsize_t *%s = (hsize_t*) luaL_checkudata("
                    "L, %d, \"HDF5::hsize_t_arr\");" % (name, n+1))
            elif t == "const hsize_t *":
                get_lua_args.append(
                    "const hsize_t *%s = (hsize_t*) luaL_checkudata("
                    "L, %d, \"HDF5::hsize_t_arr\");" % (name, n+1))
            elif t == "H5O_info_t *":
                get_lua_args.append(
                    "H5O_info_t *%s = (H5O_info_t*) luaL_checkudata("
                    "L, %d, \"HDF5::H5O_info_t\");" % (name, n+1))
            elif t == "H5T_direction_t":
                get_lua_args.append(
                    "H5T_class_t %s = luaL_checkinteger(L, %d);" % (name, n+1))
            elif t == "H5T_class_t":
                get_lua_args.append(
                    "H5T_direction_t %s = luaL_checkinteger(L, %d);" % (name, n+1))
            elif t == "int":
                get_lua_args.append(
                    "int %s = luaL_checkinteger(L, %d);" % (name, n+1))
            elif t == "unsigned":
                get_lua_args.append(
                    "unsigned %s = luaL_checkunsigned(L, %d);" % (name, n+1))
            elif t == "size_t":
                get_lua_args.append(
                    "size_t %s = luaL_checkunsigned(L, %d);" % (
                        name, n + 1))
            elif t == "hsize_t":
                get_lua_args.append(
                    "hsize_t %s = luaL_checkunsigned(L, %d);" % (
                        name, n + 1))
            elif t in ["H5S_class_t",
                       "H5S_seloper_t",
                       "H5R_type_t",
                       "H5FD_mpio_xfer_t",
                       "H5FD_mpio_chunk_opt_t"]:
                get_lua_args.append(
                    "int %s = luaL_checkinteger(L, %d);" % (
                        name, n + 1))
            else:
                self.failed.append(self.func_name)
                raise ValueError("function %s could not be wrapped: "
                                 "arg type(s) not known" %
                                 self.func_name)
        if self.ret_type == "hid_t":
            ret_statement = "lh5_push_hid_t(L, res);"
        elif self.ret_type == "herr_t":
            ret_statement = "lh5_push_herr_t(L, res);"
        elif self.ret_type == "htri_t":
            ret_statement = "lua_pushboolean(L, res);"
        elif self.ret_type in ["ssize_t", "hssize_t", "size_t", "int",
                               "H5T_class_t",
                               "H5D_layout_t"]:
            ret_statement = "lua_pushnumber(L, res);"
        else:
            self.failed.append(self.func_name)
            raise ValueError("function %s could not be wrapped: "
                             "return type not known" %
                             self.func_name)
        func = (
            """
static int _%(func_name)s(lua_State *L)
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
        self.passed.append(self.func_name)


def stripcomments(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'): return ""
        else: return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE)
    return re.sub(pattern, replacer, text)


def header_functions(prefix, outfile=None, byhand=[], extras=[], header=None,
                     luaL_Reg=None):
    if not header: header = "H5%spublic.h" % prefix
    f = open(hdf5_inc + '/' + header)
    if not luaL_Reg: luaL_Reg = "H5%s_funcs[]" % prefix
    passed = [ ]
    failed = [ ]
    raw = stripcomments(f.read())
    for line in raw.split(';') + extras:
        line = "".join([c for c in line if c not in ["\n","\t"]])
        try:
            proto = FunctionPrototype(line)
            try:
                #proto.show()
                proto.write(outfile)
                passed.append(proto.func_name)
            except ValueError as e:
                #print e
                failed.append(proto.func_name)
        except AttributeError as e:
            pass
    outfile.write(
        """\nstatic luaL_Reg %s = {\n  %s,\n  {NULL,NULL}};\n""" % (
            luaL_Reg, ',\n  '.join(["""{"%s", _%s}""" % (n, n)
                                    for n in (passed + byhand)])))


def header_data(pref1, outfile=None, regtype="number", linestart="define",
                pref2=None, header=None, mpi=False, byhand={}):
    if not header: header = "H5%spublic.h" % pref1
    f = open(hdf5_inc + '/' + header)
    if not pref2: pref2 = pref1
    for line in f:
        if linestart == "define":
            target = re.compile(r"#define (H5%s_\w+)\s*" % pref2)
        else:
            target = re.compile(r"\s*(H5%s_\w+)\s*" % pref2)
        m = target.match(line)
        if m:
            s = m.group(1)

            if s != "H5_DLL" and s == s.upper():
                if "MPI" not in s or mpi:
                    if s in byhand:
                        print 'by hand', s
                        outfile.write("  REG_NUMBER2(%s,%d);\n" % (s, byhand[s]))
                    elif regtype == "number":
                        outfile.write("  REG_NUMBER(%s);\n" % s)
                    elif regtype == "hid":
                        outfile.write("  REG_HID(%s);\n" % s)
byhand = {
    "L": ["H5Literate"]
}

# For collecting functions from header files:
# ----------------------------------------------------------
print "[HDF5 Parser] parsing HDF5:", hdf5_inc

wrap = open("h5funcs.c", "w")
for s in "ADEFGILOPRSTZ":
    header_functions(s, outfile=wrap, byhand=byhand.get(s, []))

if use_mpio:
    header_functions("P", outfile=wrap, header="H5FDmpio.h", luaL_Reg="H5P_MPI_funcs[]")
else:
    wrap.write("static luaL_Reg H5P_MPI_funcs[] = {{NULL,NULL}};")

print "[HDF5 Parser] %d functions wrapped successfully" % len(FunctionPrototype.passed)
if verbose: print '\t'+'\n\t'.join(FunctionPrototype.passed)
print "[HDF5 Parser] %d functions not wrapped successfully" % len(FunctionPrototype.failed)
if verbose: print '\t'+'\n\t'.join(FunctionPrototype.failed)


# For collecting constants from header files:
# ----------------------------------------------------------
wrap.write("static void register_constants(lua_State *L)\n{\n")
wrap.write("#define REG_NUMBER(s) lua_pushnumber(L, s); lua_setfield(L, -2, #s)\n")
wrap.write("#define REG_NUMBER2(s,t) lua_pushnumber(L, t); lua_setfield(L, -2, #s)\n")
wrap.write("#define REG_HID(s) lh5_push_hid_t(L, s); lua_setfield(L, -2, #s)\n")
wrap.write("  REG_HID(H5P_DEFAULT);\n")

header_data("", wrap, regtype="number", linestart="space")
header_data("D", wrap, regtype="number", linestart="space")
header_data("F", wrap, regtype="number", linestart="define")
header_data("S", wrap, regtype="number", linestart="space")
header_data("S", wrap, regtype="number", linestart="define",
            byhand={'H5S_UNLIMITED': -1})
header_data("T", wrap, regtype="hid", linestart="define")
header_data("T", wrap, regtype="number", linestart="space")
header_data("P", wrap, regtype="hid", linestart="define")
header_data("O", wrap, regtype="hid", linestart="define")
header_data("O", wrap, regtype="number", linestart="space")
header_data("L", wrap, regtype="number", linestart="space")

if use_mpio:
    header_data("P", wrap, regtype="number", linestart="space", pref2="D", mpi=True)
    header_data("FD", wrap, regtype="number", linestart="space",
                header="H5FDmpi.h", mpi=True)

wrap.write("#undef REG_NUMBER\n\n")
wrap.write("#undef REG_HID\n")
wrap.write("}\n")
