
all: clean imgconvert.exe

clean:
	del imgconvert.exe

imgconvert.exe: imgconvert.c
	$(CC) imgconvert.c -o $@ -std=c99