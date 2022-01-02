CC=g++
CFLAGS=-Wall -O2

.SUFFIXES: .cc

TARGET=cccut

all: $(TARGET)
$(TARGET): $(TARGET).cc
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f *~ $(TARGET) core
