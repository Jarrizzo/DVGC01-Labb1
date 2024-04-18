make:driver.c parser.c
	gcc -Wall -c driver.c
	gcc -Wall -c parser.c
	gcc -Wall -c keytoktab.c 
	gcc -Wall -c lexer.c
	gcc -Wall -c symtab.c
	gcc -Wall -c optab.c

	gcc -Wall -o parser driver.c parser.c keytoktab.c lexer.c symtab.c optab.c

	./parser <testok1.pas