default: updater update
debug: updater update-debug
o3: updater update-o3
updater:
	gcc src/recompiler.c -o dist -O3 -Wall
update:
	gcc src/main.c -o osmic -Wall
update-o3:
	gcc src/main.c -o osmic -O3 -Wall
update-debug:
	gcc src/main.c -o osmic -g3 -Wall
update-o3-debug:
	gcc src/main.c -o osmic -O3 -g3 -Wall
clean-res:
	rm 16*
	