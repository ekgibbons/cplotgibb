all: plotter pdf

plotter: plotter.c cplotgibb.h
	gcc -O3 -Wall -Wconversion -o $@ $<


pdf: test.tex
	pdflatex $^
	make clean

clean:
	rm -rf auto/ *.aux *.log
