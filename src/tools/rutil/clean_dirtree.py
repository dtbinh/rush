import os
import os.path

root_dir = "../../../projects/redhat"
exts = ['.txt', '.rbx', '.rbs', '.rbm', '.rbg', '.rba', '.log']
exclude_files = ['readme.txt']

print( "Cleaning up files extensions: %s, in directory: %s" % (str(exts), os.path.abspath(root_dir)))
def visit(indent, dirname, names):
    if not os.path.exists(dirname):
        print( "Directory %s does not exist!" % os.path.abspath(dirname))
        return
    for cfile in names:
        cpath = os.path.join(dirname, cfile)
        file, ext = os.path.splitext(cfile)
        if (not os.path.isdir(cpath)) and (not cfile.lower() in exclude_files) and (ext in exts):
            print( "%sDeleted %s" % (' '*4*indent, cpath))
            os.remove(cpath)
    
os.path.walk(root_dir, visit, 1) 
print( "DONE!" )

