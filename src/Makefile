CXX=g++
LIBS=-lfl
CXX_FLAGS=-std=c++11

all: clean niels run

lex.yy.c: niels.l
	flex $< 

niels.tab.cpp: niels.ypp
	bison -d $<
	
niels: niels.tab.cpp lex.yy.c ast.cpp
	$(CXX) ast.cpp lex.yy.c $(CXX_FLAGS) $< $(LIBS) -o niels

run:
	./niels < input.nls

clean:
	rm -f bah
	rm -f niels
	rm -f lex.yy.c
