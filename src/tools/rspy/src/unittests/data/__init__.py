import os.path

def getFullPath(fname):
    datadir = os.path.abspath(os.path.dirname(__file__))
    return os.path.join(datadir, fname)