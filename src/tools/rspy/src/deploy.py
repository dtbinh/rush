from distutils.core import setup
import sys
import py2exe

script = "main.py"
icon_file = "../../../res/rboot.ico"
optimize = 2
dos_console = 0
extra_data = [] 
extra_modules = ['']

args = ['py2exe', '-O' + str(optimize)]
sys.argv[1:] = args + sys.argv[1:]

options = {
  "py2exe": {
    "dist_dir":  "../../../../tools/rspy",
    "excludes":  [
    ],
    "optimize":  2,
  }
}

dataFiles = [
  "media/rboot.ico"
]

setup(description = "RushEngine editor",
        name = "rSpy",
        windows = [
          {
            "script": "rspy.py",
            "icon_resources":  [(1, icon_file)]
          }
        ],
        data_files = dataFiles,
        options = options
        )
