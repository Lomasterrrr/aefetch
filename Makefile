all:
	cc -O2 -g -Wall -Iinclude -c src/*.c
	cc -O2 -g -Wall *.o -o aefetch -lm
	rm -f *.o
	mv -f aefetch bin/aefetch
install: all
	mv bin/aefetch /usr/local/bin
uninstall:
	rm /usr/local/bin/aefetch
clean:
	rm bin/aefetch
