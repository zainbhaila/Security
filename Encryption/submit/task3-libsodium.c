#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LEN 20 // length of string to generate

char *gen(char *point);

int main(int argc, char *argv[]) {
	srand(time(NULL)); // seed for rand

	const char *match = argv[2]; // hex to match
	unsigned char out[crypto_hash_sha512_BYTES > crypto_hash_sha256_BYTES ? crypto_hash_sha512_BYTES : crypto_hash_sha256_BYTES];
	char string[MAX_LEN]; // init random string
	int flag = 1;
	int count = 0;

	char store[strlen(match) * 2]; // keep hex values of digest
	const unsigned char *string2;

	while (flag) {
	    gen(string);

	    string2 = (unsigned char *) string;

	    if (argv[1][3] == '2') {
			crypto_hash_sha256(out, string2, strlen(string));
		}
		else {
			crypto_hash_sha512(out, string2, strlen(string));
		}

	    for (int i = 0; i < strlen(match)/2; i++) { // store hex values of digest
	    	sprintf(&store[i*2], "%02x", out[i]);
	    }

		count++;

	    if (!memcmp(store, match, strlen(match) * sizeof(char))) {	    	
	    	break;
	    }
	}

	printf("%d\n", count);
	unsigned int file = open("task3.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(file, string2, strlen(string) * sizeof(char));
	close(file);
	return 0;
}


char *gen(char *point) // generate random strings
{
    const char possible[] = "abcdefghijklmnopqrstuvwxyz"; // charset for random string

    int n;
    for (n = 0; n < MAX_LEN; n++) { // add chars randomly to string
        int val = rand() % ((int) (sizeof(possible) - 1));
        point[n] = possible[val];
    }

    point[MAX_LEN - 1] = '\0'; // null terminate
    return point; // return pointer to string
}