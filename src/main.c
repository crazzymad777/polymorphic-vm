#include <stdio.h>

int run_vm(FILE *fd, int close_fd);

int main(int argc, char* argv[]) {
	// lambda lib and simple stack VM would be base for the great polymorphic (stack virtual) machine
	//int matrix = 0;

	FILE* fd = stdin;
	if (argc > 1) {
		int s = 0;
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				/*
				if (argv[i][1] == 'm') {
					matrix = 1;
				} else if (argv[i][1] == 'b') {
					matrix = 0;
				}
				*/
			} else {
				FILE* f = fopen(argv[i], "r");
				if (f == NULL) {
					perror("Couldn't open file");
					return -3;
				}
				fd = f;
			}
		}
	}

	return run_vm(fd, 1);
}

