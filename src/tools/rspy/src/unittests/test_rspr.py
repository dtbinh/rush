import rspr
import data

def testLoadRSPR():
    pack = rspr.Package()
    assert pack.loadFromFile(data.getFullPath("test_sprite.rspr"))
    assert len(pack.frames) == 9
    for frame in pack.frames:
        assert frame.width  > 0
        assert frame.height > 0
    frame = pack.frames[7]
    assert frame.width  == 193
    assert frame.height == 39
    assert frame.pivotx == 86
    assert frame.pivoty == 24
    assert len(frame.partition) == 42
    assert frame.filtered
    assert frame.format == rspr.PixelFormat.RGB565
    assert frame.getNumBytes() > 0
    assert len(pack.frames[0].partition) == 27
    for i in range(0, 3):
        assert pack.frames[i].getPixel(12, 42 ) == 0xFFFF0000
        assert pack.frames[i].getPixel(30, 71 ) == 0xFF0000FF
        assert pack.frames[i].getPixel(21, 127) == 0xFF00FF00
        assert pack.frames[i].getPixel(10, 178) == 0xFFFFFF00
        assert pack.frames[i].getPixel(38, 192) == 0xFFFFFF00
    for i in range(4, 7):
        assert pack.frames[i].getPixel(42,  12) == 0xFFFF0000
        assert pack.frames[i].getPixel(71,  30) == 0xFF0000FF
        assert pack.frames[i].getPixel(127, 21) == 0xFF00FF00
        assert pack.frames[i].getPixel(178, 10) == 0xFFFFFF00
        assert pack.frames[i].getPixel(192, 38) == 0xFFFFFF00

if __name__ == '__main__':
    testLoadRSPR()
    print( "OK" )

