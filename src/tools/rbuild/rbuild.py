import wx
import os.path
import sys
import SCons.Script; 

cleanup = False
dump_tree = True

args = []

if cleanup:
    args += ['--clean']
    
if dump_tree:
    args += ['--tree=all,prune']

sys.argv[1:] = args + sys.argv[1:]

os.chdir('../../..')

saveout = sys.stdout                                     
fpdump = open('tree.txt', 'w')                             
sys.stdout = fpdump                                       

SCons.Script.main()

sys.stdout = saveout                                     
fpdump.close() 

