default: updater update
debug: updater update-debug
o3: updater update-o3
updater:
	gcc tools/recompiler.c -o bin/dist -O3 -Wall
update:
	gcc src/cli/main.c -o bin/osmic -Wall
update-o3:
	gcc src/cli/main.c -o bin/osmic -O3 -Wall
update-debug:
	gcc src/cli/main.c -o bin/osmic -g3 -Wall
update-o3-debug:
	gcc src/cli/main.c -o bin/osmic -O3 -g3 -Wall
clean-res:
	rm 16*
