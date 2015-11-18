all: run

run:
	mkdir -p build
	touch build/empty
	cd build && cmake ../../src && make

test:
	../testing/runner.py

graph:
	niels input.nls build/graph.dot
	dot -T pdf build/graph.dot > build/graph.pdf
	chromium-browser build/graph.pdf

clean:
	rm -fr build
	rm -f *.dot
	rm -f *_auto.cc
	rm -f *_auto.hh
