"""
"""
import rspr
     
class Rect:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.w = 0
        self.h = 0
        
    def getR(self):
        return self.x + self.w
    
    def getB(self):
        return self.y + self.h
    
    def setR(self, val):
        self.w = val - self.x
    
    def setB(self, val):
        self.h = val - self.y
        
    r = property(getR, setR, None, "right edge of the rectangle")
    b = property(getB, setB, None, "bottom edge of the rectangle")
        
    
    
class Frame:
    def __init__(self):
        self.format = rspr.PixelFormat.Unknown
        self.width  = 0
        self.height = 0
        self.pivotx = 0
        self.pivoty = 0
        self.shiftx = 0
        self.shifty = 0
        self.ext    = Rect()
        self.filtered = False
        self.pixels = []
        self.partition = []
        
    def getPixel(self, x, y):
        if x < 0 or x >= self.width or y < 0 or y >= self.height:
            return 0x00000000
        return self.pixels[x + y*self.width]
    
    def setPixel(self, x, y, clr):
        if x < 0 or x >= self.width or y < 0 or y >= self.height:
            return
        self.pixels[x + y*self.width] = clr
    
    def getNumBytes(self):
        sz = 0
        ext = self.ext
        stride = rspr.PixelFormat().getBPP(self.format)
        for rc in self.partition:
            rcw = rc.w
            rch = rc.h
            if ext.x + ext.w < rc.x + rc.w: rcw = ext.x + ext.w - rc.x
            if ext.y + ext.h < rc.y + rc.h: rch = ext.y + ext.h - rc.y
            if self.filtered:
                sz += (rcw + 2)*(rch + 2)*stride
            else:
                sz += rcw*rch*stride;
        return sz
