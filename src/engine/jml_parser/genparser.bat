copy xgrammar.y tools\xgrammar.y
copy xgrammar.l tools\xgrammar.l
cd tools
flex -L -i xgrammar.l
bison --no-lines xgrammar.y
copy  xgrammar_tab.c+lex.yy.c JMLGrammar.cpp
del  xgrammar_tab.c
del lex.yy.c
del  xgrammar.y
del  xgrammar.l
cd..
copy tools\JMLGrammar.cpp ..\rb_core\JMLGrammar.cpp
del tools\JMLGrammar.cpp