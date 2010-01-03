"""
ResTree is project resource tree, reflecting hierarchy of the resources, used in context of currently edited script.
Usually resource tree is represented by subtree of file system, plus contents of resource packages
"""
import wx

class ResTree(wx.TreeCtrl):
    def __init__(self, parent): 
        wx.TreeCtrl.__init__(self, parent)
        
    def setRootDir(self, path):
        pass