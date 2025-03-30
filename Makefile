# Define installation paths
PREFIX ?= /usr/local
INCLUDEDIR ?= $(PREFIX)/include

# Targets
all: plotter

plotter: plotter.c cplotgibb.h
	gcc -O3 -Wall -Wconversion -Werror -o $@ $<

install: cplotgibb.h
	@echo "Installing cplotgibb.h to $(INCLUDEDIR)..."
	mkdir -p $(INCLUDEDIR)
	cp cplotgibb.h $(INCLUDEDIR)/cplotgibb.h
	@echo "Installation complete."

uninstall:
	@echo "Removing cplotgibb.h from $(INCLUDEDIR)..."
	rm -f $(INCLUDEDIR)/cplotgibb.h
	@echo "Uninstallation complete."

clean:
	rm -rf auto/ *.aux *.log *.pdf
