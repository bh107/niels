CXX=g++
LIBS=-lfl
CXX_FLAGS=-std=c++11

all: clean niels run

lex.yy.c: niels.l
	flex $< 

niels.tab.cpp: niels.ypp
	bison -d $<
	
niels: niels.tab.cpp lex.yy.c
	$(CXX) driver.cpp utils.cpp ast.cpp symboltable.cpp $^ $(CXX_FLAGS) $(LIBS) -g -o niels

run:
	./niels > graph.dot
	dot -T pdf graph.dot > graph.pdf
	dot -T png graph.dot > graph.png
	mupdf graph.pdf

clean:
	rm -f bah
	rm -f niels
	rm -f lex.yy.c
	rm -f niels.tab.cpp
	rm -f niels.tab.hpp
