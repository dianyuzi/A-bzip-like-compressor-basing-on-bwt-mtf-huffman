CC = gcc
SRCS = src/huffman.c src/mtf.c src/bwt.c src/bitio.c src/main.c src/multiple_processing.c
OFILE = $(SRCS:.c=.o)
TARGET= compressor
$(TARGET): $(OFILE)
	$(CC) $(OFILE) -o $(TARGET)
%.o: %.c
	$(CC) -Iinclude -c $< -o $@ -static
clean:
	cmd /C del /F /Q src\*.o
	cmd /C del /F /Q compressor.exe
.PHONY: clean 