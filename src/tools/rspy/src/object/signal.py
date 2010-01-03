
class SignalType:
    Unknown = 0
    In      = 1
    Out     = 2
    InOut   = 3
    def toStr(self, sigtype):
        if sigtype == SignalType.Unknown: return "?"
        if sigtype == SignalType.In: return "<<"
        if sigtype == SignalType.Out: return ">>"
        if sigtype == SignalType.InOut: return "<>" 

    
class Signal:
    def __init__(self, prop, path, type):
        self.prop = prop
        self.path = path
        self.type = type