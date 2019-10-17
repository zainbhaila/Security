#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int i;
	unsigned char nonce[crypto_secretbox_NONCEBYTES];
	randombytes_buf(nonce, sizeof(nonce));

	if (argc != 4 ) {
		puts("ERROR");
		return 0;
	}

	FILE *fp = fopen(argv[2], "r+"); // open key file
    if (fp == NULL) {
		puts("ERROR");
    	return 0;
    }

    fseek(fp, 0, SEEK_END); // get size of key file
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char key[len]; // convert key file to char array key
    for (i = 0; i < len; i++) {
        key[i] = fgetc(fp);
    }

	FILE *fp2 = fopen(argv[3], "r+"); // open message/cipher file
    if (fp2 == NULL) {
		puts("ERROR");
    	fclose(fp);
    	return 0;
    }

    fseek(fp2, 0, SEEK_END); // get size of message/cipher file
    int len2 = ftell(fp2);
    fseek(fp2, 0, SEEK_SET);

    unsigned char message[len2]; // convert message/cipher file to char array message
    for (i = 0; i < len2; i++) {
        message[i] = fgetc(fp2);
    }

	if (!strcmp(argv[1], "write")) {
		unsigned char ciphertext[crypto_secretbox_MACBYTES + len2];
		crypto_secretbox_easy(ciphertext, message, len2, nonce, key);
		FILE *fpout = fopen("cipher.bin", "w+"); // open output file
	    if (fpout == NULL) {
			puts("ERROR");
		    fclose(fp);
		    fclose(fp2);
	    	return 0;
	    }

		for (i = 0; i < crypto_secretbox_NONCEBYTES; i++) {
	        fputc(nonce[i], fpout);
	    }
	    for (i = 0; i < crypto_secretbox_MACBYTES + len2; i++) {
	        fputc(ciphertext[i], fpout);
	    }

	    fclose(fpout);
	}
	else if (!strcmp(argv[1], "read")) {
		unsigned char ciphertext[crypto_secretbox_MACBYTES + len2];
		for (i = 0; i < crypto_secretbox_NONCEBYTES; i++) {
	        nonce[i]  = message[i];
	    }

		if (crypto_secretbox_open_easy(ciphertext, &message[crypto_secretbox_NONCEBYTES], len2 - crypto_secretbox_NONCEBYTES, nonce, key) != 0) {
		    puts("INVALID");
		    fclose(fp);
		    fclose(fp2);
		    return 0;
		}

	    for (i = 0; i < len2 - crypto_secretbox_MACBYTES - crypto_secretbox_NONCEBYTES; i++) {
	        putc(ciphertext[i], stdout);
	    }
	}
	else {
		puts("ERROR");
	    fclose(fp);
	    fclose(fp2);
		return 0;
	}

    fclose(fp);
    fclose(fp2);
	return 0;
}