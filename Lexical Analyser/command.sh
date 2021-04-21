flex -o myLex.c myLex.l
g++ myLex.c -lfl -o myLex.out
./myLex.out input.txt
