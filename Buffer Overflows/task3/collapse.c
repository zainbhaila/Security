#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 2048 // max size of word (includes null char)

int main() {
	char input[BUFFER_SIZE] = {0}; // storing immediate user input
	char current[BUFFER_SIZE] = {0}; // storing previous string
	char temp[BUFFER_SIZE + 12] = {0}; // 12 is the size of int + space + newline on end
	unsigned int count = 1; // consecutive counter
	char * string2;
	int lastindex = 0;
	char * string = malloc(1);
	if (string == NULL) {
		fputs("Error\n", stdout);
		free(string);
		return -1;
	}
	string[0] = '\0'; 

	// go until eof
	while(fgets(input, BUFFER_SIZE, stdin)) {
		if (input[strlen(input) - 1] != '\n') { // last character must be \n
			fputs("Error\n", stdout);
			free(string);
			return -1;
		}

		lastindex = strlen(input) - 1; // strip trailing whitespace
		while (isspace(input[lastindex]) && lastindex >= 0) {
			lastindex--;
		}

		input[lastindex + 1] = '\0';

		if (strcspn(&input[strspn(input, "\t ")], " \t") != strlen(&input[strspn(input, "\t ")])) { // if whitespace character occurs mid string
			fputs("Error\n", stdout);
			free(string);
			return -1;
		}

		if (lastindex < 0 || strlen(input) == 0) {
			memset(input, 0, BUFFER_SIZE); // reset input
		}
		else if (strcmp(&current[strspn(current, "\t ")], &input[strspn(input, "\t ")]) == 0) { // if its a repeat string
			count++; // update count
			memset(input, 0, BUFFER_SIZE); // reset input
		}
		else { // if its a new string
			if (strlen(current) > 0) { // dont allow strings of length 0
				sprintf(temp, "%d ", count); // counter
				sprintf(&temp[strlen(temp)], "%s", &current[strspn(current, "\t ")]); // in its own statement so users cant put formatting in the string
				sprintf(&temp[strlen(temp)], "\n"); // newline
			}

			string2 = realloc(string, strlen(string) + strlen(temp) + 1);
			if (string2 == NULL) {
				fputs("Error\n", stdout);
				free(string);
				return -1;
			}
			string = string2;

			sprintf(&string[strlen(string)], "%s", temp);

			memset(temp, 0, BUFFER_SIZE + 12);
			memcpy(current, input, BUFFER_SIZE); // save new string
			memset(input, 0, BUFFER_SIZE); // reset input
			count = 1; // reset count
		}
	}
	// last string inputted
	if (strlen(current) > 0) { // dont allow strings of length 0
		sprintf(temp, "%d ", count); // counter
		sprintf(&temp[strlen(temp)], "%s", &current[strspn(current, "\t ")]); // in its own statement so users cant put formatting in the string
		sprintf(&temp[strlen(temp)], "\n"); // newline
	}

	string = realloc(string, strlen(string) + strlen(temp) + 1);
	if (string == NULL) {
		fputs("Error\n", stdout);
		free(string);
		return -1;
	}

	sprintf(&string[strlen(string)], "%s", temp);

	printf("%s", string);
	free(string);
	return 0;
}