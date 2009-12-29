import os
import os.path

#root_dir = "../../../projects/m2"
#exts = [".cpp", ".h"]

root_dir = "../../../projects/m2"
exts = [".rjml", ".rxml", ".rbs", ".rbm", ".rbg", ".rba"]
exclude_dirs = [".svn"]

print( "Converting to lowercase files od type: %s, in directory: %s" % (str(exts), root_dir))
def visit(indent, dirname, names):
    if not os.path.exists(dirname):
        print( "Directory %s does not exist!" % os.path.abspath(dirname))
        return
    print( "%s%s:" % (' '*indent*4, os.path.abspath(dirname)))
    for cfile in names:
        cpath = os.path.join(dirname, cfile)
        if os.path.isdir(cpath):
            if cfile in exclude_dirs:
                print( "Directory %s skipped..." % os.path.abspath(cpath))
                names.remove(cfile)
            else:
                npath = os.path.join(dirname, cfile.lower())
                print( "%sdir:%s => %s" % (' '*4*indent, cpath, npath))
                os.renames(cpath, npath)
        else:
            ext = os.path.splitext(cfile)[1]
            if ext in exts:
                nfile = cfile.lower()
                print( "%s%s => %s" % (' '*4*indent, cfile, nfile))
                os.rename(cpath, os.path.join(dirname, nfile))
            
os.path.walk(root_dir, visit, 1) 
print( "DONE!" )
