import os
import os.path

root_dir = "../../../projects/redhat"
convertor = "../../../bin/rboot_d.exe"
exclude_dirs = ['assets', ".svn"]

print( "Converting models in directory: %s" % os.path.abspath(root_dir))
def visit(indent, dirname, names):
    if not os.path.exists(dirname):
        print( "Directory %s does not exist!" % os.path.abspath(dirname))
        return
    print( "%s%s:" % (' '*indent*4, os.path.abspath(dirname)))
    for cfile in names:
        cpath = os.path.abspath(os.path.join(dirname, cfile))
        file, ext = os.path.splitext(cfile)
        if (os.path.isdir(cpath)):
            if cfile in exclude_dirs:
                print( "Directory %s skipped..." % os.path.abspath(cpath))
                names.remove(cfile)
        else:
            next = ""
            cmd = ""
            if ext == ".rbm":
                next = ".rmdl"
                cmd = "model"
            if ext == ".rba":
                next = ".ranm"
                cmd = "anim"
            if next != "":
                nfile = file + next
                print( "%s%s => %s" % (' '*4*indent, cpath, nfile))
                cmdline = '--convert --%s="%s"' %(cmd, cpath)
                conv = os.path.abspath(convertor)
                print( conv + " " + cmdline )
                os.spawnv( os.P_WAIT, conv, (conv,) + tuple([cmdline]))
                #print( "converted")
    
os.path.walk(root_dir, visit, 1) 
print( "DONE!" )

