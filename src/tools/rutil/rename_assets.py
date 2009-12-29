import os
import os.path

root_dir = "../../../projects/m2"
src_exts = ['.txt', '.rbx', '.rbs']
dst_exts = ['.rjml', '.rxml', '.rspr']
exclude_files = ['readme.txt']

print( "Changing extensions: %s=>%s, in directory: %s" % (str(src_exts), str(dst_exts), os.path.abspath(root_dir)))
def visit(indent, dirname, names):
    if not os.path.exists(dirname):
        print( "Directory %s does not exist!" % os.path.abspath(dirname))
        return
    print( "%s%s:" % (' '*indent*4, os.path.abspath(dirname)))
    for cfile in names:
        cpath = os.path.join(dirname, cfile)
        file, ext = os.path.splitext(cfile)
        if (not os.path.isdir(cpath)) and (not cfile.lower() in exclude_files) and (ext in src_exts):
            ext = dst_exts[src_exts.index(ext)]
            nfile = file + ext
            print( "%s%s => %s" % (' '*4*indent, cfile, nfile))
            os.rename(cpath, os.path.join(dirname, nfile))
    
os.path.walk(root_dir, visit, 1) 
print( "DONE!" )

