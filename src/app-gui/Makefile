CXX = g++

TARGET = pfw_gui
SOURCES = main.cpp pfw.cpp
HEADERS = pfw.h

.PHONY: all clean install remove

all: $(TARGET)


$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(SOURCES) `pkg-config gtkmm-3.0 --cflags --libs` -o $@

clean:
	rm -f $(TARGET)

install:
	cp -f $(TARGET) /usr/local/sbin

remove:
	rm -f /usr/local/sbin/$(TARGET)