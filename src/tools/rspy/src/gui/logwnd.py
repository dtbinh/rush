"""
Log window implementation
"""
import sys
import wx
import wx.lib.mixins.listctrl as listmix

class LogWindow(wx.ListCtrl, listmix.ColumnSorterMixin, listmix.ListCtrlAutoWidthMixin):
    """Log Window, showing output from different sources"""
    def __init__(self, parent): 
        wx.ListCtrl.__init__(self, parent, style=wx.LC_REPORT | wx.BORDER_NONE)
        
        self.SetBackgroundColour(wx.Colour(240, 240, 220))
        
        self.InsertColumn(0, "Severity")
        self.InsertColumn(1, "Source")
        self.InsertColumn(2, "Message")
        
        entries = [
            ("INFO", "rboot", "Engine Started..."),
            ("WARNING", "rboot", 'Could not open script file "physics.rjml"'),
            ("INFO", "rspy", "Nice day, huh?.."),
            ("ERROR", "rspy", 'In script "forest.rjml": "Expected "}" (at char 29), (line:3, col:5)'),
        ]
        
        key = 1
        self.itemDataMap = {}
        for data in entries:
            errtype = data[0]
            index = self.InsertStringItem(sys.maxint, errtype)
            self.SetStringItem(index, 1, data[1])
            self.SetStringItem(index, 2, data[2])
            self.SetItemData(index, key)
            self.itemDataMap[key] = data
            key = key + 1
            item = self.GetItem(index)
            if errtype == "INFO": item.SetTextColour(wx.BLACK)
            if errtype == "WARNING": item.SetTextColour(wx.BLUE)
            if errtype == "ERROR": 
                item.SetTextColour(wx.BLACK)
                item.SetBackgroundColour(wx.Colour(255, 200, 200))
            self.SetItem(item)
            
        listmix.ListCtrlAutoWidthMixin.__init__(self)
        listmix.ColumnSorterMixin.__init__(self, 3)
        self.SetColumnWidth(0, 80)
        self.SetColumnWidth(1, 80)
        self.SetColumnWidth(2, 100)
    
    # Used by the ColumnSorterMixin
    def GetListCtrl(self):
        return self

        
        
        
