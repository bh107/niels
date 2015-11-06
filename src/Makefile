CXX=g++
LIBS=-lfl -lbh
CXX_FLAGS=-std=c++11

all: clean niels run

scanner.c: scanner.l
	flex -o scanner.c $< 

parser.cc: parser.yy
	bison --defines=parser.hh -o parser.cc $<
	
niels: parser.cc scanner.c 
	$(CXX) -L/home/safl/.local/lib -I/home/safl/.local/include/bohrium -I/home/safl/.local/include driver.cc utils.cc ast.cc symboltable.cc $^ $(CXX_FLAGS) $(LIBS) -g -o niels

run:
	./niels

dot: clean niels run
	./niels > graph.dot
	dot -T pdf graph.dot > graph.pdf
	dot -T png graph.dot > graph.png
	mupdf graph.pdf

clean:
	rm -f niels
	rm -f scanner.c
	rm -f parser.hh
	rm -f parser.cc
