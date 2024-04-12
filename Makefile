make: driver.c parser.c
	gcc -Wall -c driver.c
	gcc -Wall -c parser.c
	gcc -Wall -c keytoktab.c 

	gcc -Wall -o parser driver.c parser.c keytoktab.c

	./parser