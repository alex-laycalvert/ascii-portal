portal:
	gcc "src/main.c" "src/portal.c" "src/menu.c" -lncurses -o ./portal
dev:
	gcc -g3 -Wall -Wextra -fsanitize=address,undefined "src/main.c" "src/portal.c" "src/menu.c" -lncurses -o ./portal.dev
clean:
	rm -f ./portal
	rm -f ./portal.dev
