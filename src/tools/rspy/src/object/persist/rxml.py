"""
Parses object tree from .rxml file 
"""
import xml.dom.minidom
from object import RushObj, SignalType

def _extract_node(obj_root, xml_root):
    obj_root.name = xml_root.getAttribute("name")
    obj_root.type = xml_root.localName
    for i in range(len(xml_root.attributes)):
        attr = xml_root.attributes.item(i)
        if attr.name != "name":
            val = attr.nodeValue
            obj_root.addProp(attr.name, val)
    for xml_child in xml_root.childNodes:
        if xml_child.nodeType == xml.dom.minidom.Node.ELEMENT_NODE:
            if xml_child.localName == "signal":
                prop = xml_child.getAttribute("attr")
                path = xml_child.getAttribute("path")
                type = xml_child.getAttribute("dir")
                type_enum = SignalType.Unknown
                if type == "in": type_enum = SignalType.In
                if type == "out": type_enum = SignalType.Out
                if type == "inout": type_enum = SignalType.InOut
                obj_root.addSignal(prop, path, type_enum)
            else:
                obj_child = obj_root.createChild()
                _extract_node(obj_child, xml_child)
            
                
def parse(str):
    try:
        encStr = str#'<?xml version="1.0" encoding="UTF-8"?>\n' + str
        doc = xml.dom.minidom.parseString(encStr)
    except xml.parsers.expat.ExpatError, err:
        print encStr.split( "\n" )[err.lineno - 1] 
        print " "*(err.offset - 1) + "^"
        print err
        return None
    obj_root = RushObj()
    _extract_node(obj_root, doc.documentElement)
    return obj_root
    
if __name__ == "__main__":
    fp = open('../../../data/game.rxml', 'rb')
    text = fp.read()
    fp.close()
    
    tree = parse(text)
    jml = tree.toJML()
    
    print(jml)
    print("Done!")

        