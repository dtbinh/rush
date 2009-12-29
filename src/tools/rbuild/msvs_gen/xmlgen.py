class Node:
    "xml node class"
    def __init__(self, name):
        self.name = name
        self.attributes = []
        self.children = []
        
    def add_attr(self, name, value):
        self.attributes.append( (name, value) )
        
    def add_child(self, name):
        self.children.append( Node( name ) )
        return self.children[-1]
    
    def to_string(self, indent = 0):
        res = '%s<%s' % ( ' '*indent*4, self.name )
        #write attributes
        if len( self.attributes ) > 1:
            for attr in self.attributes:
                res += '\n%s%s="%s"' % ( ' '*(indent + 1)*4, attr[0], attr[1] )
        elif len( self.attributes ) == 1:
            attr = self.attributes[0]
            res += ' %s="%s"' % ( attr[0], attr[1] )
        #write children
        if (len( self.children ) == 0):
            res += '/>\n'
        else:
            res += '>\n'
            for ch in self.children:
                res += ch.to_string( indent + 1 )
            res += '%s</%s>\n' % ( ' '*indent*4, self.name )
        return res
    
    def to_string_doc(self, encoding):
        res = ""
        res += '<?xml version="1.0" encoding="%s"?>\n' % encoding
        res += self.to_string()
        return res