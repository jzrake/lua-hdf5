
import re

class FunctionPrototype(object):
    def __init__(self, prototype):
        target = re.compile(r"H5_DLL (\w+)\s+(\w+)(.*)")
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

def H5F_func():
    f = open("/Library/Science/hdf5/include/H5Fpublic.h")
    for line in f.read().split(';'):
        line = "".join([c for c in line if c not in ["\n","\t"]])
        try:
            proto = FunctionPrototype(line)
            proto.show()
        except AttributeError:
            pass

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

H5F_func()
#H5F()
#H5S()
#H5P()
#H5T()
