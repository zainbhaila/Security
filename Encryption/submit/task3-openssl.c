#include <openssl/evp.h>
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

	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();; // new context
	const EVP_MD *sha = EVP_get_digestbyname(argv[1]); // set sha value
	const char *match = argv[2]; // hex to match
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
	char string[MAX_LEN]; // init random string
	int flag = 1;
	int count = 0;

	char store[strlen(match) * 2]; // keep hex values of digest

	while (flag) {
	    gen(string);
		EVP_DigestInit_ex(mdctx, sha, NULL);
	    EVP_DigestUpdate(mdctx, string, strlen(string));
	    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	    EVP_MD_CTX_reset(mdctx);

	    for (int i = 0; i < strlen(match)/2; i++) { // store hex values of digest
	    	sprintf(&store[i*2], "%02x", md_value[i]);
	    }

		count++;

	    if (!memcmp(store, match, strlen(match) * sizeof(char))) {	    	
	    	break;
	    }
	}

	printf("%d\n", count);
	unsigned int file = open("task3.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(file, string, strlen(string) * sizeof(char));
	close(file);
	EVP_MD_CTX_free(mdctx);
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