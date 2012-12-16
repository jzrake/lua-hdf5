
import re

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
            elif t in ["H5S_class_t", "H5S_seloper_t", "H5R_type_t"]:
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
        elif self.ret_type in ["ssize_t", "size_t", "int"]:
            ret_statement = "lua_pushnumber(L, res);"
        else:
            self.failed.append(self.func_name)
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


def header_functions(prefix, outfile=None, byhand=[], extras=[]):
    f = open("/Library/Science/hdf5/include/H5%spublic.h" % prefix)
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
        """\nstatic luaL_Reg H5%s_funcs[] = {\n  %s,\n  {NULL,NULL}};\n""" % (
            prefix, ',\n  '.join(["""{"%s", h5lua_%s}""" % (n, n)
                                for n in (passed + byhand)])))

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

def H5O():
    f = open("/Library/Science/hdf5/include/H5Opublic.h")    
    for line in f:
        target = re.compile(r"\s*(H5O_T\w+).*")
        m = target.match(line)
        if m:
            s = m.group(1)
            print "  REG_NUMBER(%s);" % s

def H5L():
    f = open("/Library/Science/hdf5/include/H5Lpublic.h")    
    for line in f:
        target = re.compile(r"\s*(H5L_T\w+).*")
        m = target.match(line)
        if m:
            s = m.group(1)
            print "  REG_NUMBER(%s);" % s

def H5():
    f = open("/Library/Science/hdf5/include/H5public.h")    
    for line in f:
        target = re.compile(r"\s*(H5_\w+).*")
        m = target.match(line)
        if m:
            s = m.group(1)
            print "  REG_NUMBER(%s);" % s

byhand = {
    "L": ["H5Literate"]
}

# For collecting functions from header files:
# ----------------------------------------------------------
wrap = open("h5funcs.c", "w")
for s in "ADEFGILOPRSTZ":
    header_functions(s, outfile=wrap, byhand=byhand.get(s, []))


print "%d functions wrapped successfully" % len(FunctionPrototype.passed)
#print '\t'+'\n\t'.join(FunctionPrototype.passed)
print "%d functions not wrapped successfully" % len(FunctionPrototype.failed)
#print '\t'+'\n\t'.join(FunctionPrototype.failed)


# For collecting constants from header files:
# ----------------------------------------------------------
#H5F()
#H5S()
#H5P()
#H5T()
#H5O()
#H5L()
#H5()
