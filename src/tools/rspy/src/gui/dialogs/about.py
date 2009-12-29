"""
About Box
"""
import wx

class AboutBox(wx.ListView):
    """Log Window, showing output from different sources"""
    def __init__(self, parent): 
        wx.ListView.__init__(self, parent)
        
        