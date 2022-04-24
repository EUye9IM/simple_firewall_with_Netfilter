CXX = g++
LDFLAGS = -lresolv

TARGET = pfw_cli

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp -f $(TARGET) /usr/local/bin

remove:
	rm -f /usr/local/bin/$(TARGET)