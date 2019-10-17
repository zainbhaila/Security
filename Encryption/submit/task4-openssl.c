#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LEN 7 // length of string to generate
#define TAGLEN 12

char *gen(char *point);

int main(int argc, char *argv[]) {
	int i, outlen, templen;
    unsigned char iv[MAX_LEN];
    gen((char *) iv);
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

	if (argc != 4 ) {
		puts("ERROR");
		return 0;
	}

	FILE *fp = fopen(argv[2], "r+"); // open key file
    if (fp == NULL) {
		puts("ERROR");
		EVP_CIPHER_CTX_free(ctx);
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
		EVP_CIPHER_CTX_free(ctx);
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
		unsigned char tag[TAGLEN];
		unsigned char outbuf[len2];
		if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_ccm(), NULL, key, iv) || !EVP_EncryptUpdate(ctx, NULL, &outlen, NULL, len2) || !EVP_EncryptUpdate(ctx, outbuf, &outlen, message, len2) || !EVP_EncryptFinal_ex(ctx, outbuf + outlen, &templen) || !EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_GET_TAG, TAGLEN, tag)) {
  			puts("ERROR");
		    fclose(fp);
		    fclose(fp2);
  		    EVP_CIPHER_CTX_free(ctx);
		    return 0;
		}

		EVP_CIPHER_CTX_free(ctx);
		
		FILE *fpout = fopen("cipher.bin", "w+"); // open output file
	    if (fpout == NULL) {
			puts("ERROR");
		    fclose(fp);
		    fclose(fp2);
	    	return 0;
	    }

	    for (i = 0; i < TAGLEN; i++) {
	    	fputc(tag[i], fpout);
	    }
	    for (i = 0; i < MAX_LEN; i++) {
	    	fputc(iv[i], fpout);
	    }
	    for (i = 0; i < outlen + templen; i++) {
	        fputc(outbuf[i], fpout);
	    }

	    fclose(fpout);
	}
	else if (!strcmp(argv[1], "read")) {
		unsigned char outbuf[len2];
		unsigned char tag[TAGLEN];
	    for (i = 0; i < TAGLEN; i++) {
	    	tag[i] = message[i];
	    }
	    for (i = 0; i < MAX_LEN; i++) {
	    	iv[i] = message[TAGLEN + i];
	    }
		if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_ccm(), NULL, NULL, NULL) || !EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, TAGLEN, tag) || !EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv) || !EVP_DecryptUpdate(ctx, NULL, &outlen, NULL, len2 - TAGLEN - MAX_LEN) || !EVP_DecryptUpdate(ctx, outbuf, &outlen, &message[TAGLEN + MAX_LEN], len2 - TAGLEN - MAX_LEN)) {
  			puts("INVALID");
		    fclose(fp);
		    fclose(fp2);
  		    EVP_CIPHER_CTX_free(ctx);
		    return 0;
		}

		EVP_CIPHER_CTX_free(ctx);

		for (i = 0; i < outlen; i++) {
	        putc(outbuf[i], stdout);
	    }
	}
	else {
		puts("ERROR");
	    fclose(fp);
	    fclose(fp2);
		EVP_CIPHER_CTX_free(ctx);
		return 0;
	}
	
    fclose(fp);
    fclose(fp2);
	return 0;
}

char *gen(char *point) // generate random strings - used for iv
{
    const char possible[] = "123456789"; // charset for random string

    int n;
    for (n = 0; n < MAX_LEN; n++) { // add chars randomly to string
        int val = rand() % ((int) (sizeof(possible) - 1));
        point[n] = possible[val];
    }

    point[MAX_LEN - 1] = '\0'; // null terminate
    return point; // return pointer to string
}