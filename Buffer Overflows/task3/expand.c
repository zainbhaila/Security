#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 2060 // max size of word (includes null char) (2048) + max length of uint (10) + space and newline (2)

int main() {
	char line[BUFFER_SIZE] = {0}; // storing line
	unsigned int count;
	char *ptr;
	char *string = malloc(1);
	char *string2;
	int lastindex = 0;
	if (string == NULL) {
		fputs("Error\n", stdout);
		return -1;
	}
	string[0] = '\0'; 
	int space;
	int i;

	// go until eof
	while(fgets(line, BUFFER_SIZE, stdin)) {
		strcpy(line, &line[strspn(line, "\t ")]); // strip leading whitespace

		if (line[strlen(line) - 1] != '\n') { // last character must be \n
			fputs("Error\n", stdout);
			free(string);
			return -1;
		}

		space = strcspn(line, " "); // divide string between number and word
		if (space > 10) { // max uint length is 10
			fputs("Error\n", stdout);
			free(string);
			return -1;
		}
		else {
			line[space] =  '\0';
		}

		lastindex = strlen(&line[space + 1]) - 1 + space + 1; // strip trailing whitespace
		while (isspace(line[lastindex]) && lastindex >= 0) {
			lastindex--;
		}

		line[lastindex + 1] = '\0';

		if ((lastindex < 0 || strlen(&line[space+1]) == 0) && isspace(line[0])) {
			memset(line, 0, BUFFER_SIZE); // reset input
			continue; // if line is empty
		}

		for (i = 0; i < space; i++) { // malformed number
			if (isdigit(line[i]) == 0) {
				fputs("Error\n", stdout);
				free(string);
				return -1;
			}
		}
		count = (unsigned int) strtoul(line, &ptr, 10); // number of times to print
		
		if (count <  0 || count > 4294967295 || (count == 0 && isdigit(line[0]) == 0)) { // bad value
			fputs("Error\n", stdout);
			free(string);
			return -1;
		}

		for (i = 0; i < count; i++) {
			string2 = realloc(string, strlen(string) + strlen(&line[strspn(&line[space+1], " \t") + space + 1]) + 2);
			if (string2 == NULL) {
				fputs("Error\n", stdout);
				free(string);
				return -1;
			}
			string = string2;

			if (strlen(&line[strspn(&line[space+1], " \t") + space + 1]) == 0
				|| strcspn(&line[strspn(&line[space+1], " \t") + space + 1], " \t") != strlen(&line[strspn(&line[space+1], " \t") + space + 1])) { // no word or disjoint
				fputs("Error\n", stdout);
				free(string);
				return -1;
			}

			sprintf(&string[strlen(string)], "%s\n", &line[strspn(&line[space+1], " \t") + space + 1]);
		}

		memset(line, 0, BUFFER_SIZE); // reset line
	}

	printf("%s", string);
	free(string);
	return 0;
}