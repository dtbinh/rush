"""
rSpy main menu
"""
import wx

class MainMenu(wx.MenuBar):
    """Application main menu class"""
    def __init__(self): 
        wx.MenuBar.__init__(self)
        
        self.ID_OpenFile = wx.NewId()
        self.ID_Save = wx.NewId()
        self.ID_About = wx.NewId()

        file_menu = wx.Menu()
        file_menu.Append(self.ID_OpenFile, "&Open File...\tCtrl+O")
                
        item = wx.MenuItem( file_menu, self.ID_Save, "&Save\tCtrl+S", "Save" )
        file_menu.AppendItem(item)
        file_menu.AppendSeparator()
        
        file_menu.Append(wx.ID_EXIT, "Exit")

        help_menu = wx.Menu()
        help_menu.Append(self.ID_About, "About...")
        
        self.Append(file_menu, "File")
        self.Append(help_menu, "Help")