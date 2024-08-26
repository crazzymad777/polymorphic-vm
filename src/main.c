#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <povm.h>

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

	const char* protect_field = "PROTECT2";
	char buffer[9] = {0};
	int b = fread(buffer, 8, 1, fd);
	if (b < 1) {
		fprintf(stderr, "Invalid header signature");
		return -1;
	}

	if (strcmp(buffer, protect_field) != 0) {
		fprintf(stderr, "Invalid header signature");
		return -2;
	}

	size_t stack_size = 0;
	fread(&stack_size, 8, 1, fd);

	if (stack_size == 0) {
		stack_size = 4096;
	}

	union udatum stack[stack_size / 8];
	int32_t types[stack_size / 8];

	int exit_code = povm_execute(fd, stack, types);

	if (stdin != fd) {
		fclose(fd);
	}
	return exit_code;
}

