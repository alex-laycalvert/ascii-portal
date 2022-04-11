portal:
	gcc "src/main.c" -lncurses -o ./portal
dev:
	gcc -g3 -Wall -Wextra -fsanitize=address,undefined "src/main.c" -lncurses -o ./portal.dev
clean:
	rm -f ./portal
	rm -f ./portal.dev
