CXX = g++

TARGET = pfw_cli

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp -f $(TARGET) /usr/local/sbin

remove:
	rm -f /usr/local/sbin/$(TARGET)