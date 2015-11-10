all: clean dist pyg view

dist:
	python setup.py install --user --force

pyg:
	pygmentize -o test.html -O full program.nls

view:
	thunar test.html

clean:
	rm -f ~/.local/lib/python2.7/site-packages/pygments_niels-0.0.1-py2.7.egg
	rm -f test.html
	rm -rf build
	rm -rf dist
	rm -rf pygments_niels.egg-info
