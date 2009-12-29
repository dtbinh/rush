from pyparsing import *

class DepParser:
    def __init__(self):
        self.deps = {}

    def on_list(self, s, loc, toks):
        key = toks[0]
        self.deps[key] = toks[1:]
    
    def get_grammar(self):
        identifier = Word( alphas, alphanums + "_" + "+" )
        list = identifier + Literal( "=" ).suppress() + delimitedList(identifier)
        list.setParseAction(self.on_list)
        grammar = ZeroOrMore(list)
        return grammar
    
    def parse(self, text):
        self.get_grammar().parseString(text)
        return self.deps
    
if __name__ == '__main__':
    text="""\
    int_libs = rb_core, rb_util, rb_math
    ext_libs = libogg, libvorbis
    """
    p = DepParser().parse(text)
    print(p)