"""
Progess Box
"""
import wx

class ProgressBox(wx.Panel):
    """Log Window, showing output from different sources"""
    def __init__(self, parent): 
        wx.Panel.__init__(self, parent)
        