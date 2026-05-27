#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>

/* Configuration macros */
#define WORDS 8 /* Amount of words to be present in the password */
#define MAX_WORD_LEN 64 /* Max amount of character to be included in single word (further characters get trashed) */

/* Password */
static char pass[WORDS*MAX_WORD_LEN];

/* Amount of words in words.txt (w = word) */
static size_t wsize = 1;

/* Initialize buffer pointer for raw data from words.txt and random words buffer */
static char wbuffer[WORDS][MAX_WORD_LEN+1];
static char* buffer = NULL;

/* Random numbers from 1 to N (r = random) */
static size_t rbuffer[WORDS];

/* Fill buffer with words from words.txt (f = fill) */
void fbuffer(void) {
	char c;
	size_t cnt = 0, size = 0; /* cnt = count */
	FILE *fptr = fopen("words.txt","r"); /* words.txt should be atleast 2 words long */
	if(fptr == NULL) {
		perror("Failed to open words.txt");
		exit(EXIT_FAILURE);
	}

	char prev = '\n';
	while((c=fgetc(fptr))!=EOF) {
		size++;
		if((c=='\n' || c=='\t' || c==' ') && prev!='\n' && prev!='\t' && prev!=' ')
			wsize++;
		prev = c;
	}

	buffer = malloc((size+1) * sizeof(char));
	if(buffer == NULL) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	rewind(fptr);

	/* Fill buffer with words from words.txt */
	int ccnt = 0; /* c = char , cnt = count (stated above) */
	while((c=fgetc(fptr))!=EOF) {
		if((c=='\n' || c=='\t' || c==' ') && cnt!=0 && buffer[cnt-1]=='\n')
			continue;
		else {
			if(c=='\n' || c=='\t' || c==' ') {
				buffer[cnt] = '\n';
				cnt++;
				ccnt = 0;
			}
			else if(ccnt<MAX_WORD_LEN) { /* Characters with numbers bigger than MAX_WORD_LEN are trashed */
				buffer[cnt] = c;
				cnt++;
				ccnt++;
			}
		}
	}
	buffer[cnt]='\0';
	fclose(fptr);
}

/* Fill random numbers buffer */
void frbuffer(void) {
	for(int i = 0; i < WORDS; i++) {
		/* Random numbers between 1 and wsize inclusive */
		ssize_t random = getrandom((rbuffer+i), sizeof(size_t), 0);
		if(random<0) {
			perror("getrandom() failed");
			exit(EXIT_FAILURE);
		}

		rbuffer[i] %= (wsize-1);
		rbuffer[i]++;
	}
}

/* Fill wbuffer with random words from buffer */
void fwbuffer(void) {
	size_t cnt = 1, offset = 0;
	int wcnt = 0;

	for(int i = 0; i < WORDS; i++) {
		/* Sets offset variable to offset from a buffer pointer to a start of the word */
		while(cnt!=rbuffer[i]) {
			if(buffer[offset] == '\n') {
				cnt++;
				offset++;
			}
     			else offset++;
		}
		/* Fill wbuffer[i] */
		while(buffer[offset]!='\n' && buffer[offset]!='\0') {
			wbuffer[i][wcnt] = buffer[offset];
			wcnt++;
			offset++;
		}
     		wbuffer[i][wcnt] = '\0';
		cnt = 1;
		wcnt = 0;
     		offset = 0;
     	}
}

/* Fill pass with words from wbuffer and insert '-' symbol between them */
void fpass(void) {
	int cnt = 0;
	for(int i = 0; i < WORDS; i++) {
		for(int j = 0; wbuffer[i][j] != '\0'; j++) {
			pass[cnt] = wbuffer[i][j];
			if(wbuffer[i][j+1] == '\0') {
				cnt++;
				pass[cnt] = '-';
			}
			cnt++;
		}
	}
	pass[cnt-1] = '\0';
}

/* Generates new password into pass */
void genpass(void) {
	fbuffer();
	frbuffer();
	fwbuffer();
	fpass();
	free(buffer);
	buffer = NULL;	
}

int main(void) {
	genpass();
	printf("%s\n", pass);

	free(buffer);
	buffer = NULL;
	return 0;
}
