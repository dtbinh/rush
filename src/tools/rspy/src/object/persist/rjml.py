"""
Parses object tree from .rjml file 
"""
import time
from pyparsing import Word, QuotedString, Literal, Forward, Optional, SkipTo, ZeroOrMore, ParseException
from pyparsing import alphas, alphanums, cppStyleComment
import psyco
from object import RushObj, SignalType

psyco.full()

class RJML:
    def __init__(self):
        self.root_obj = None
        self.cur_obj = None
        self.cur_sig_type = SignalType.Unknown
        self.obj_stack = []
        
    def onSigIn(self):
        self.cur_sig_type = SignalType.In
    
    def onSigOut(self):
        self.cur_sig_type = SignalType.Out
    
    def onSigInOut(self):
        self.cur_sig_type = SignalType.InOut
    
    def onAttr(self, toks):
        self.cur_obj.addProp(toks[0], toks[1])
    
    def onSignal(self, toks):
        self.cur_obj.addSignal(toks[0], toks[1], self.cur_sig_type)
    
    def onStartObj(self, toks):
        type = toks[0]
        name = ""
        if len(toks) > 1:
            name = toks[1]
        obj = RushObj(type, name)
        if self.cur_obj == None:
            self.root_obj = obj
        else:
            self.cur_obj.addChild(obj)
        self.cur_obj = obj
        self.obj_stack.append(obj)
    
    def onEndObj(self):
        self.obj_stack.pop()
        self.cur_obj = None
        if len(self.obj_stack) > 0:
            self.cur_obj = self.obj_stack[-1]
    
    def onPartObj(self, toks):
        pass
        
    def getGrammar(self):
        identifier = Word(alphas, alphanums + "_")
        # attribute
        attr_value = QuotedString('"') | SkipTo( ";" )
        attr = identifier + Literal("=").suppress() + attr_value + Literal(";").suppress()
        attr.setParseAction(self.onAttr)
        # signal
        sig_in = Literal("<<").setParseAction( self.onSigIn )
        sig_out = Literal(">>").setParseAction( self.onSigOut )
        sig_inout = Literal("<>").setParseAction( self.onSigInOut )
        signal = identifier + Literal("=").suppress() + \
            (sig_in | sig_out | sig_inout).suppress() + \
            attr_value + Literal(";").suppress()
        signal.setParseAction(self.onSignal)
        # object    
        obj_name = QuotedString('"')
        obj_decl = Forward()
        full_obj_decl = (identifier + Optional(obj_name)).setParseAction(self.onStartObj) + \
            "{" + ZeroOrMore(signal | attr | obj_decl) + Literal("}").setParseAction(self.onEndObj)
        part_obj_decl = identifier + obj_name + Literal(";").suppress()
        obj_decl << (part_obj_decl | full_obj_decl)
        part_obj_decl.setParseAction(self.onPartObj)
        # grammar
        grammar = obj_decl
        grammar.ignore(cppStyleComment)
        return grammar

    def parse(self, str):
        grammar = self.getGrammar()        
        try:
            grammar.parseString(str)
        except ParseException, err:
            print err.line
            print " "*(err.column-1) + "^"
            print err
            return None
        return self.root_obj

def parse(str):
    return RJML().parse(str)

if __name__ == "__main__":
    fp = open('../../../data/game.rjml', 'rb')
    text = fp.read()
    fp.close()
    
    st_time = time.clock()
    parser = RJML()
    tree = parser.parse(text)
    
    jml = tree.toJML()
    
    print(jml)
    
    print( "Done in %.2f second(s)." % (time.clock() - st_time))
