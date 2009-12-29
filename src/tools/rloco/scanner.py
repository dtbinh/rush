""" Scanner of the script files, to extract all the strings from them """
import os
import sys
sys.path.append('../rspy/src')

from object.persist import rjml
from object.persist import rxml

exclude_dirs = ['assets', ".svn", "system"]

def visit(indent, dirname, names):
    if not os.path.exists(dirname):
        print( "Directory %s does not exist!" % os.path.abspath(dirname))
        return
    for cfile in names:
        cpath = os.path.join(dirname, cfile)
        file, ext = os.path.splitext(cfile)
        if (not os.path.isdir(cpath)):
            parse = None
            if ext == ".rjml" or ext == ".txt":
                print( "Scanning rjml: %s" % cpath )
                parse = rjml.parse
            elif ext ==".rxml" or ext == ".rbx":
                print( "Scanning rxml: %s" % cpath )
                parse = rxml.parse
            if parse != None:
                fp = open(cpath, 'rb')
                txt = fp.read()
                fp.close()
                obj_tree_root = None
                obj_tree_root = parse(txt)
        elif cfile in exclude_dirs:
            # skip directory
            names.remove(cfile)

def ScanDir( path ):
    os.path.walk(path, visit, 1)
    
if __name__ == "__main__":
    ScanDir( "d:/home/rush/projects/redhat" )