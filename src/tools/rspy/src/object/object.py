from signal import Signal, SignalType
    
class RushObj:
    def __init__(self, type = "object", name = ""):
        self.type = type
        self.name = name
        self.children = []
        self.props = []
        self.signals = []
        self.parent = None
        
    def createChild(self, type = "object", name = ""):
        child = RushObj(type, name)
        self.children.append(child)
        child.parent = self
        return child
    
    def addChild(self, obj):
        self.children.append(obj)
        obj.parent = self
    
    def addProp(self, name, value=""):
        self.props.append((name, value))
        
    def addSignal(self, prop, path, type):
        self.signals.append(Signal(prop, path, type))
        
    def toJML(self, indent=0):
        res = ""
        if self.name == "":
            res += ('%s%s\n' % (indent*4*' ', self.type))
        else:
            res += ('%s%s "%s"\n' % (indent*4*' ', self.type, self.name))
        res += ('%s{\n' % (indent*4*' '))
        for prop in self.props:
            if not prop[0] in ["text", "gametitle"]:
                res += ('%s%s = "%s";\n' % ((indent + 1)*4*' ', prop[0], prop[1]))
        for sig in self.signals:
            res += ('%s%s = %s%s;\n' % ((indent + 1)*4*' ', sig.prop, SignalType().toStr(sig.type), sig.path))
        for child in self.children:
            res += child.toJML(indent + 1)
        res += ('%s}\n' % (indent*4*' '))
        return res
        