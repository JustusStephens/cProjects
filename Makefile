interactiveMode.o: interactiveMode.c
	gcc -c interactiveMode.c
batchMode.o: batchMode.c
	gcc -c batchMode.c
checkCommand.o: checkCommand.c
	gcc -c checkCommand.c
executeCommand.o: executeCommand.c
	gcc -c executeCommand.c
redirect.o: redirect.c
	gcc -c redirect.c
parallel.o: parallel.c
	gcc -c parallel.c
wish.o: wish.c
	gcc -c wish.c
wish: wish.o interactiveMode.o batchMode.o checkCommand.o executeCommand.o redirect.o parallel.o
	gcc -o wish wish.o interactiveMode.o batchMode.o checkCommand.o executeCommand.o redirect.o parallel.o