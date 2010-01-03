"""
Scratch Board is a working area to temporarily hold nodes, shortcuts etc
"""
import  wx
        
class ScratchBoard(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent, -1)