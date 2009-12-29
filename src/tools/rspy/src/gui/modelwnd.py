import wx.aui
        
class ModelWindow(wx.aui.AuiMDIChildFrame):
    def __init__(self, parent, count):
        wx.aui.AuiMDIChildFrame.__init__(self, parent, -1,
                                         title="Model: %d" % count)
        self.panel = wx.Panel(self)
        sizer = wx.BoxSizer()
        sizer.Add(self.panel, 1, wx.EXPAND)
        self.SetSizer(sizer)
        wx.CallAfter(self.Layout)
        
        self.hwnd = self.panel.GetHandle()
        
        cmd = 'd:/home/rush/bin/rboot.exe'
        cmd += ' --media="d:\home\rush\projects\rview\media\" --anim="d:\home\rush\projects\rview\media\models\redhat\cut_redhat_window.ranm"'
        cmd += ' --window=%d' % self.hwnd
        
        self.process = wx.Process(self)
        self.pid = wx.Execute(cmd, wx.EXEC_ASYNC, self.process)
        
        self.panel.Bind(wx.EVT_SIZE, self.onSize)
        
    def onSize(self, event):
        w, h = event.GetSize()
        print("size: %d %d" %(w, h) )
        
        event.Skip()
        
    def __del__(self):
        if self.process is not None:
            self.process.Detach()
            self.process = None
            self.pid = 0
            
            