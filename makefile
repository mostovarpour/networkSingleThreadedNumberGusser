make: numberGuesser.c server.c
	g++ numberGuesser.c server.c -o out

clean:
	rm out
