portal:
	gcc src/*.c -lncurses -o ./portal
dev:
	gcc -g3 -Wall -Wextra -fsanitize=address,undefined src/*.c -lncurses -o ./portal.dev
clean:
	rm -f ./portal
	rm -f ./portal.dev
	rm -rf ./portal.dev.dSYM
