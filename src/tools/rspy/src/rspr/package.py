"""
Implemetation of the RushEngine sprite format loading/saving
"""
import struct
import array
import rspr
from frame import Frame, Rect
        
class Package:
    def __init__(self):
        self.frames = []
        
    def dump(self):
        res = ""
        for f in self.frames:
            res += "w:%d h:%d pivX:%d pivY:%d ext:{%d, %d, %d, %d} format:%s filtered:%d nrect:%d\n"%\
                   (f.width, f.height, f.pivotx, f.pivoty, \
                    f.ext.x, f.ext.y, f.ext.w, f.ext.h, \
                    rspr.PixelFormat().toStr(f.format), f.filtered, len(f.partition))
        return res
        
    def loadFromFile(self, fname):
        fp = open(fname, "rb")
        data = fp.read()
        fp.close()
        header_size = 14
        num_bytes = len(data)
        if num_bytes < header_size:
            return False
        header = data[0:header_size]
        id, num_sprites = struct.unpack("4sH8x", header)
        
        if id != 'RBSP':
            # not a RushEngine sprite file
            return False
        
        # read frame headers
        pos = header_size
        num_header_words = 13
        for i in range(0, num_sprites):
            frame = Frame()
            pos_end = pos + num_header_words*2
            frame.width, frame.height, frame.format, \
                frame.ext.x, frame.ext.y, frame.ext.w, frame.ext.h, \
                frame.filtered, num_rects, \
                frame.pivotx, frame.pivoty, frame.shiftx, frame.shifty \
                = struct.unpack("%dH"%num_header_words, data[pos:pos_end])
            pos = pos_end
            # read partition
            for irct in range(0, num_rects):
                rc = Rect()
                pos_end = pos + 2*4
                rc.x, rc.y, rc.w, rc.h = struct.unpack("4H", data[pos:pos_end])
                frame.partition.append(rc)
                pos = pos_end
            self.frames.append(frame)
            
        #read pixels
        for frame in self.frames:
            stride = rspr.PixelFormat().getBPP(frame.format)
            num_bytes = frame.getNumBytes()
            frame.pixels = array.array('L', [0]*frame.width*frame.height)
            ext = frame.ext
            border = 1 if frame.filtered else 0 
            fmt = frame.format
            for rc in frame.partition:
                w, h = rc.w, rc.h
                if rc.x + w > ext.r: w = ext.r - rc.x;
                if rc.y + h > ext.b: h = ext.b - rc.y;
                for x in range(rc.x - border, rc.x + w):
                    for y in range(rc.y - border, rc.y + h):
                        pos_end = pos + stride
                        clrval = data[pos:pos_end]
                        clr = rspr.PixelFormat().toARGB8(clrval, fmt)
                        frame.setPixel(x, y, clr)
                        pos = pos_end
        return True