cc -O2 -g -Wall -Iinclude -c src/*.c
cc -O2 -g -Wall *.o -o aefetch -lm
rm -f *.o
mv -f src/aefetch bin/aefetch
./bin/aefetch
