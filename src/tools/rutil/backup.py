import os, time, zipfile, glob
import smtplib, base64

src_dirs = ['c:/home/src']
outdir = 'c:/home/backups'
ignore_dirs = ['.svn', 'VTune', 'temp', 'dependencies', 'Debug', 'Release', ".metadata", ".plugins"]
ignore_exts = ['.pyc']

if not os.path.exists(outdir):
    os.mkdir(outdir)
             
fname = "rush." + time.strftime('%d%b%Y') + ".zip"
print( "Creating backup file: " + fname )
fpath = os.path.join( outdir, fname )
zip = zipfile.ZipFile( fpath, "w" )

def addDir( zipFile, path ):
    for subPath in glob.glob( os.path.join( path, "*" ) ):
        if os.path.isdir( subPath ):
            if not (os.path.basename( subPath ) in ignore_dirs):
                print( "adding directory: " + subPath )
                addDir( zipFile, subPath )
            else:
                print( "skipped directory: " + subPath )
        else:
            if not (os.path.splitext( subPath )[1] in ignore_exts):
                zip.write(subPath)  
            else:
                print( "skipped file: " + subPath )

for path in src_dirs:
    addDir( zip, path )
    
zip.close()

print( "Backup process finished." )
