all: plotter

plotter: plotter.c cplotgibb.h
	gcc -O3 -Wall -Wconversion -Werror -o $@ $<

clean:
	rm -rf auto/ *.aux *.log
