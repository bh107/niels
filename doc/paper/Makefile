REPORT=niels

view: pdf
	evince $(REPORT).pdf

pdf:
	#latex $(REPORT)
	#bibtex $(REPORT)
	#latex $(REPORT)
	#latex $(REPORT)
	latexmk $(REPORT).tex -pdf

clean:
	rm *.aux *.bbl *.blg *.fdb_latexmk *.log *.fls
