"""
Object tree reflects the hierarchy of RushObjects
"""
import wx

class ObjectTree(wx.TreeCtrl):
    """Tree view for the object hierarchy"""
    def __init__(self, parent, obj_tree_root = None): 
        wx.TreeCtrl.__init__(self, parent, wx.ID_ANY, wx.Point(0, 0), wx.Size(160, 250),
                           wx.TR_DEFAULT_STYLE | wx.NO_BORDER)
        imglist = wx.ImageList(16, 16, True, 2)
        imglist.Add(wx.ArtProvider_GetBitmap(wx.ART_FOLDER, wx.ART_OTHER, wx.Size(16,16)))
        imglist.Add(wx.ArtProvider_GetBitmap(wx.ART_NORMAL_FILE, wx.ART_OTHER, wx.Size(16,16)))
        self.AssignImageList(imglist)
        
        self.tree_root = self.setObject(obj_tree_root)
        if self.tree_root != None:
            self.Expand(self.tree_root)
        
    def getIconID(self, obj):
        type = obj.type
        if len(obj.children) > 0: return 0
        return 1
    
    def setObject(self, obj_tree_root, tree_root = None):
        if obj_tree_root == None:
            return
        if tree_root == None:
            tree_root = self.AddRoot("")
        name = obj_tree_root.name
        if name == "":
            name = obj_tree_root.type
        self.SetItemText(tree_root, name)
        self.SetItemImage(tree_root, self.getIconID(obj_tree_root))
        for obj_tree_child in obj_tree_root.children:
            tree_child = self.AppendItem(tree_root,"")
            self.setObject(obj_tree_child, tree_child)
        return tree_root
            
        
        