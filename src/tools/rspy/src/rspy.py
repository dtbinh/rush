import wx
import gui

if __name__ == "__main__":
    app = wx.PySimpleApp(0)
    wx.InitAllImageHandlers()
    frame = gui.MainWindow(None, wx.ID_ANY, "rSpy", size=(750, 590))
    app.SetTopWindow(frame)
    frame.Show()
    app.MainLoop()
    

