all: run test

run:
	mkdir -p build
	touch build/empty
	cd build && cmake ../ && make

test:
	./build/niels input.nls

graph:
	./build/niels input.nls artifacts/graph.dot
	dot -T pdf artifacts/graph.dot > artifacts/graph.pdf
	chromium-browser artifacts/graph.pdf

clean:
	rm -fr build
	rm -f *.dot
	rm -f *_auto.cc
	rm -f *_auto.hh
