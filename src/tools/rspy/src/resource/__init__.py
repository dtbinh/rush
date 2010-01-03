import os.path

def getMediaPath(fname):
    datadir = os.path.abspath(os.path.dirname(__file__))
    datadir = os.path.abspath(os.path.join(datadir, "../../data"))
    return os.path.join(datadir, fname)