assmbler: main.o preassmbler.o firstwalk.o secondwalk.o sidefunctions.o
	gcc -ansi -Wall main.o preassmbler.o firstwalk.o secondwalk.o sidefunctions.o -o assmbler
sidefunctions.o: sidefunctions.c structs.h sidefunctions.h
	gcc -c -ansi -Wall sidefunctions.c -o sidefunctions.o
secondwalk.o: secondwalk.c firstwalk.h structs.h sidefunctions.h
	gcc -c -ansi -Wall secondwalk.c -o secondwalk.o
firstwalk.o: firstwalk.c firstwalk.h structs.h sidefunctions.h
	gcc -c -ansi -Wall firstwalk.c -o firstwalk.o
preassmbler.o: preassmbler.c preassmbler.h structs.h sidefunctions.h
	gcc -c -ansi -Wall preassmbler.c -o preassmbler.o
main.o: main.c firstwalk.h structs.h sidefunctions.h preassmbler.h
	gcc -c -ansi -Wall main.c -o main.o
