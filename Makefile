CXX      = g++
CC       = gcc
OBJ      = main.o ShutdownWindow.o
LINKOBJ  = main.o ShutdownWindow.o
CFLAGS = -std=c++11 -Wall -g -c -Wno-parentheses
LIBS = `pkg-config gtkmm-3.0 dbus-cxx-1.0 --cflags --libs` -lX11
BIN	 = examos-shutdown

.PHONY: all all-before all-after clean clean-custom
default: build ;

install: $(BIN)
	mkdir -p $(DESTDIR)usr/bin && cp $(BIN) $(DESTDIR)usr/bin/$(BIN)
	mkdir -p $(DESTDIR)usr/share/applications && cp examos-shutdown.desktop $(DESTDIR)usr/share/applications/examos-shutdown.desktop
	mkdir -p $(DESTDIR)usr/share/icons/hicolor/scalable/apps && cp examos-shutdown.svg $(DESTDIR)usr/share/icons/hicolor/scalable/apps/examos-shutdown.svg

build: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(LINKOBJ) -o $(BIN) $(LIBS)

./%.o: ./%.cpp
	$(CXX) $(CFLAGS) $< -o $@ $(LIBS)

.PHONY : clean
clean :
	-rm -rf $(OBJ) $(BIN) usr/
