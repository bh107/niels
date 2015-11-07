all: run test

run:
	mkdir -p build
	touch build/empty
	cd build && cmake ../ && make

test:
	./build/niels

clean:
	rm -fr build
	rm -f *.dot
