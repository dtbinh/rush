import wx
import os
import scanner
import xlrd

class MainMenu(wx.MenuBar):
    """Application main menu class"""
    def __init__(self): 
        wx.MenuBar.__init__(self)
        
        self.ID_Open     = wx.NewId()
        self.ID_Save     = wx.NewId()
        self.ID_SaveAs   = wx.NewId()
        self.ID_Import   = wx.NewId()
        self.ID_Export   = wx.NewId()
        self.ID_About    = wx.NewId()
        self.ID_Help     = wx.NewId()

        file_menu = wx.Menu()
        
        file_menu.Append( self.ID_Open,     "&Open Table...\tCtrl+O" )
        file_menu.Append( self.ID_Save,     "&Save\tCtrl+S", "Save" )
        file_menu.Append( self.ID_SaveAs,   "&Save As...\tCtrl+Shift+S", "Save" )

        file_menu.AppendSeparator()
                
        file_menu.Append( self.ID_Import, "&Import...\tCtrl+I", "Import" )
        file_menu.Append( self.ID_Export, "&Export...\tCtrl+E", "Export" )

        file_menu.AppendSeparator()
        
        file_menu.Append( wx.ID_EXIT, "Exit" )

        help_menu = wx.Menu()
        help_menu.Append( self.ID_Help, "&Help...\tF1", "Help" )
        help_menu.Append( self.ID_About, "About...")
        
        self.Append( file_menu, "File" )
        self.Append( help_menu, "Help" )
       
wildcard = "String Table (*.rstr)|*.rstr|"     \
           "All files (*.*)|*.*" 
#---------------------------------------------------------------------------
class MyMiniFrame(wx.MiniFrame):
    def __init__(
        self, parent, title, pos=wx.DefaultPosition, size=wx.DefaultSize,
        style=wx.DEFAULT_FRAME_STYLE 
        ):

        wx.MiniFrame.__init__(self, parent, -1, title, pos, size, style)
        panel = wx.Panel(self, -1)

        mb = MainMenu()
        self.SetMenuBar(mb)
        
        self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)     
 
        self.Bind(wx.EVT_MENU, self.onOpen,   id = mb.ID_Open  )
        self.Bind(wx.EVT_MENU, self.onSave,   id = mb.ID_Save  )
        self.Bind(wx.EVT_MENU, self.onSaveAs, id = mb.ID_SaveAs)          
        
        self.Bind(wx.EVT_MENU, self.onImport, id = mb.ID_Import)
        self.Bind(wx.EVT_MENU, self.onExport, id = mb.ID_Export)

    def onImport(self, event):
        # In this case we include a "New directory" button. 
        dlg = wx.DirDialog(self, "Choose a directory:",
                          style = wx.DD_DEFAULT_STYLE
                           | wx.DD_DIR_MUST_EXIST
                           #| wx.DD_CHANGE_DIR
                           )
        
        if dlg.ShowModal() == wx.ID_OK:
            scanner.ScanDir( dlg.GetPath() )
        
        # Only destroy a dialog after you're done with it.
        dlg.Destroy()
        
    def onExport(self, event):
        # In this case we include a "New directory" button. 
        dlg = wx.DirDialog(self, "Choose a directory:",
                          style = wx.DD_DEFAULT_STYLE
                           | wx.DD_DIR_MUST_EXIST
                           #| wx.DD_CHANGE_DIR
                           )
        
        # If the user selects OK, then we process the dialog's data.
        # This is done by getting the path data from the dialog - BEFORE
        # we destroy it. 
        if dlg.ShowModal() == wx.ID_OK:
            self.log.WriteText('You selected: %s\n' % dlg.GetPath())
        
        # Only destroy a dialog after you're done with it.
        dlg.Destroy()
        
    def onOpen(self, evt):
        dlg = wx.FileDialog(
            self, message="Choose a file",
            defaultDir=os.getcwd(), 
            defaultFile="",
            wildcard=wildcard,
            style=wx.OPEN | wx.MULTIPLE | wx.CHANGE_DIR
            )

        if dlg.ShowModal() == wx.ID_OK:
            paths = dlg.GetPaths()
            for path in paths:
                book = xlrd.open_workbook( path )
                sheet = book.sheet_by_index( 1 )
                print( sheet.name.encode('unicode_escape') )
                #cell = sheet.cell(1, 0)
                #print( str( cell.value ) )
        dlg.Destroy()

    def onSave(self, evt):
        pass


    def onSaveAs(self, evt):
        # Create the dialog. In this case the current directory is forced as the starting
        # directory for the dialog, and no default file name is forced. This can easilly
        # be changed in your program. This is an 'save' dialog.
        #
        # Unlike the 'open dialog' example found elsewhere, this example does NOT
        # force the current working directory to change if the user chooses a different
        # directory than the one initially set.
        dlg = wx.FileDialog(
            self, message="Save file as ...", defaultDir=os.getcwd(), 
            defaultFile="", wildcard=wildcard, style=wx.SAVE
            )

        # This sets the default filter that the user will initially see. Otherwise,
        # the first filter in the list will be used by default.
        dlg.SetFilterIndex(2)

        # Show the dialog and retrieve the user response. If it is the OK response, 
        # process the data.
        if dlg.ShowModal() == wx.ID_OK:
            path = dlg.GetPath()
            self.log.WriteText('You selected "%s"' % path)

            # Normally, at this point you would save your data using the file and path
            # data that the user provided to you, but since we didn't actually start
            # with any data to work with, that would be difficult.
            # 
            # The code to do so would be similar to this, assuming 'data' contains
            # the data you want to save:
            #
            # fp = file(path, 'w') # Create file anew
            # fp.write(data)
            # fp.close()
            #
            # You might want to add some error checking :-)
            #

        # Note that the current working dir didn't change. This is good since
        # that's the way we set it up.
        self.log.WriteText("CWD: %s\n" % os.getcwd())

        # Destroy the dialog. Don't do this until you are done with it!
        # BAD things can happen otherwise!
        dlg.Destroy()

    def OnCloseWindow(self, event):
        print "OnCloseWindow"
        self.Destroy()

#---------------------------------------------------------------------------

app = wx.PySimpleApp(0)
win = MyMiniFrame(None, "rloco - RushEngine localization tool",
                  style=wx.DEFAULT_FRAME_STYLE | wx.TINY_CAPTION_HORIZ)
win.SetSize((800, 600))
win.CenterOnParent(wx.BOTH)
win.Show(True)
app.MainLoop()

"""\

#---------------------------------------------------------------------------

app = wx.PySimpleApp(0)
wx.InitAllImageHandlers()
frame = TestPanel(None, None)
app.SetTopWindow(frame)
frame.Show()
app.MainLoop()
"""
