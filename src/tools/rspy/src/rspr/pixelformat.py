"""

"""
class PixelFormat:
    ARGB4444    = 0
    ARGB8888    = 1
    RGB565      = 2
    RGB888      = 3
    A8          = 4
    Unknown     = 255
    
    def getBPP(self, fmt):
        try:
            return [2, 4, 2, 3, 1][fmt]
        except:
            return 0
    
    def toStr(self, fmt):
        try:
            return ["ARGB4444", "ARGB8888", "RGB565", "RGB888", "A8", "Unknown"][fmt]
        except:
            return "Unknown"
        
    def toARGB8(self, val, fmt):
        if fmt == PixelFormat.ARGB4444:
            a = ((val&0xF000)>>16)|0xF
            r = ((val&0x0F00)>>8)|0xF
            g = (val&0x00F0)|0xF
            b = ((val&0x000F)<<8)|0xF
            return (a<<24)|(r<<16)|(g<<8)|b
        if fmt == PixelFormat.ARGB8888:
            return val
        if fmt == PixelFormat.A8:
            return (val<<24)|0x00FFFFFF
        if fmt == PixelFormat.RGB888:
            return 0xFF000000|val
        if fmt == PixelFormat.RGB565:
            return 0
        return 0x00000000