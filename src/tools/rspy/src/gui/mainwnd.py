"""
rSpy main window
"""
import os.path
import wx.aui
import resource


from object.persist import rjml
from object.persist import rxml

import gui

def GetIcon():
    icon = wx.EmptyIcon()
    icon.LoadFile(resource.getMediaPath("rboot.ico"), wx.BITMAP_TYPE_ICO )
    return icon

class MainWindow(wx.aui.AuiMDIParentFrame):
    
    def __init__(self, parent, id=-1, title="", pos=wx.DefaultPosition,
                 size=wx.DefaultSize):
        style = wx.DEFAULT_FRAME_STYLE | wx.SUNKEN_BORDER | wx.CLIP_CHILDREN;
        wx.aui.AuiMDIParentFrame.__init__(self, parent, id, title, pos, size, style)
        
        # tell FrameManager to manage this frame        
        self._mgr = wx.aui.AuiManager()
        self._mgr.SetManagedWindow(self)
        
        self._perspectives = []
        self.n = 0
        self.x = 0
        
        self.SetIcon(GetIcon())

        # create menu
        mb = gui.MainMenu()
        
        self.SetMenuBar(mb)

        self.statusbar = self.CreateStatusBar(2, wx.ST_SIZEGRIP)
        self.statusbar.SetStatusWidths([-2, -3])
        self.statusbar.SetStatusText("Ready", 0)
        self.statusbar.SetStatusText("Welcome To rSpy!", 1)

        # min size for the frame itself isn't completely done.
        # see the end up FrameManager::Update() for the test
        # code. For now, just hard code a frame minimum size
        self.SetMinSize(wx.Size(400, 300))

        self._mgr.AddPane(self.createLogCtrl(), wx.aui.AuiPaneInfo().
                          Name("log").Caption("Log Window").
                          Bottom().Layer(1).Position(1).CloseButton(True).MaximizeButton(True))

        # make default perspective
        perspective_default = self._mgr.SavePerspective()
        self._perspectives.append(perspective_default)

        # "commit" all changes made to FrameManager   
        self._mgr.Update()

        self.Bind(wx.EVT_ERASE_BACKGROUND, self.onEraseBackground)
        self.Bind(wx.EVT_SIZE, self.onSize)
        self.Bind(wx.EVT_CLOSE, self.onClose)

        self.Bind(wx.EVT_MENU, self.onOpenFile, id = mb.ID_OpenFile)
        self.Bind(wx.EVT_MENU, self.onSave, id = mb.ID_Save)
             
        self.Bind(wx.EVT_MENU, self.onExit, id = wx.ID_EXIT)
        self.Bind(wx.EVT_MENU, self.onAbout, id = mb.ID_About)
        
        self.createPalette()
        self.createResTree()
        
        self.createScriptWnd()
        self.createScriptWnd()
        self.createScriptWnd()
        self.createModelWnd()

    def onClose(self, event):
        self._mgr.UnInit()
        del self._mgr
        self.Destroy()

    def onOpenFile(self, event):
        wildcard = "Rush Script Files (*.rxml; *.rjml; *.rbin)|*.rxml; *.rjml; *.rbin|"\
           "Rush Model Files (*.rmdl)|*.rmdl|"\
           "Rush Animation Files (*.ranm)|*.ranm|"\
           "All files (*.*)|*.*"
           
        dlg = wx.FileDialog(
            self, message="Choose a file",
            defaultDir=os.getcwd(), 
            defaultFile="",
            wildcard=wildcard,
            style=wx.OPEN | wx.MULTIPLE | wx.CHANGE_DIR )
        if dlg.ShowModal() == wx.ID_OK:
            paths = dlg.GetPaths()
            for path in paths:
                ext = os.path.splitext(path)[1]
                fp = open(path, 'rb')
                txt = fp.read()
                fp.close()
                obj_tree_root = None
                if ext == '.rxml': obj_tree_root = rxml.parse(txt)
                if ext == '.rjml': obj_tree_root = rjml.parse(txt)
                self.createTreeCtrl(obj_tree_root)
                self.createPropGrid()
                
        
    def onSave(self, event):
        pass

    def onExit(self, event):
        self.Close()

    def onAbout(self, event):
        msg = "Rush Engine Editor\n" + \
              "(c) Copyright 2007, Digital Cucumbers"
        dlg = wx.MessageDialog(self, msg, "About rSpy",
                               wx.OK | wx.ICON_INFORMATION)
        dlg.ShowModal()
        dlg.Destroy()        


    def onEraseBackground(self, event):
        event.Skip()

    def onSize(self, event):
        event.Skip()

    def onCopyPerspective(self, event):
        s = self._mgr.SavePerspective()
        if wx.TheClipboard.Open():
            wx.TheClipboard.SetData(wx.TextDataObject(s))
            wx.TheClipboard.Close()
        
    def onRestorePerspective(self, event):
        self._mgr.LoadPerspective(self._perspectives[event.GetId()])

    def getStartPosition(self):
        self.x = self.x + 20
        x = self.x
        pt = self.ClientToScreen(wx.Point(0, 0))
        return wx.Point(pt.x + x, pt.y + x)
            
    def createPalette(self):
        palette = gui.ObjectPalette(self)
        paneinfo = wx.aui.AuiPaneInfo()
        paneinfo.BestSize( wx.Size(80,300) )
        paneinfo.Name( "pal" )
        paneinfo.Caption( "Palette" )
        paneinfo.Left()
        paneinfo.Layer( 1 )
        paneinfo.Position( 0 )
        paneinfo.PinButton( False )
        paneinfo.MaximizeButton( False )
        paneinfo.MinimizeButton( False )
        paneinfo.CloseButton( True )
        paneinfo.MinSize( wx.Size(80, 40) )
        paneinfo.MaxSize( wx.Size(80, 1300) )
        self._mgr.AddPane(palette, paneinfo)
        self._mgr.Update()
        
    def createResTree(self):
        restree = gui.ResTree(self)
        restree.setRootDir("d:/home/rush/projects/redhat2")
        paneinfo = wx.aui.AuiPaneInfo()
        paneinfo.BestSize( wx.Size(180,300) )
        paneinfo.Name( "res" )
        paneinfo.Caption( "Resources" )
        paneinfo.Left()
        paneinfo.Layer( 2 )
        paneinfo.Position( 0 )
        paneinfo.PinButton( False )
        paneinfo.MaximizeButton( True )
        paneinfo.MinimizeButton( False )
        paneinfo.CloseButton( True )
        paneinfo.MinSize( wx.Size(80, 40) )
        paneinfo.MaxSize( wx.Size(80, 1300) )
        self._mgr.AddPane(restree, paneinfo)
        self._mgr.Update()
        
    def createScriptWnd(self):
        wnd = gui.ScriptWindow(self, 1)
        wnd.Show()
        self._mgr.Update()
        
    def createModelWnd(self):
        wnd = gui.ModelWindow(self, 1)
        wnd.Show()
        self._mgr.Update()
        
    def createPropGrid(self):
        prop_grid = gui.ObjectInspector(self)
        paneinfo = wx.aui.AuiPaneInfo()
        paneinfo.BestSize( wx.Size(180,300) )
        paneinfo.Name( "prop" )
        paneinfo.Caption( "Properties" )
        paneinfo.Right()
        paneinfo.Layer( 1 )
        paneinfo.Position( 0 )
        paneinfo.PinButton( False )
        paneinfo.MaximizeButton( True )
        paneinfo.MinimizeButton( True )
        paneinfo.CloseButton( True )
        paneinfo.MinSize( wx.Size(180, 30) )
        paneinfo.MaxSize( wx.Size(180, 1300) )
        self._mgr.AddPane(prop_grid, paneinfo)
        self._mgr.Update()
        
    def createLogCtrl(self):
        log_box = gui.LogWindow(self)
        
        paneinfo = wx.aui.AuiPaneInfo()
        paneinfo.BestSize( wx.Size(640,150) )
        paneinfo.Name( "log" )
        paneinfo.Caption( "Log Window" )
        paneinfo.Bottom()
        paneinfo.Layer( 2 )
        paneinfo.Position( 0 )
        paneinfo.PinButton( False )
        paneinfo.MaximizeButton( True )
        paneinfo.MinimizeButton( False )
        paneinfo.CloseButton( True )
        paneinfo.MinSize( wx.Size(180, 40) )
        self._mgr.AddPane(log_box, paneinfo)
        self._mgr.Update()
    
    def createTreeCtrl(self, obj_tree_root = None):
        objtree = gui.ObjectTree(self, obj_tree_root)
        
        paneinfo = wx.aui.AuiPaneInfo()
        paneinfo.BestSize( wx.Size(180,300) )
        paneinfo.Name( "objtree" )
        paneinfo.Caption( obj_tree_root.name )
        paneinfo.Left()
        paneinfo.Layer( 0 )
        paneinfo.Position( 0 )
        paneinfo.PinButton( False )
        paneinfo.MaximizeButton( True )
        paneinfo.MinimizeButton( False )
        paneinfo.CloseButton( True )
        paneinfo.MinSize( wx.Size(180, 40) )
        self._mgr.AddPane(objtree, paneinfo)
        self._mgr.Update()
