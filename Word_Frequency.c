#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
//struct to keep track of frequency of each word
typedef struct {
	char word[101];
	int frequency;
} WordArray ;

int compareWords(const void *f1, const void *f2){
	WordArray *a = (WordArray *)f1;
	WordArray *b = (WordArray *)f2;
	return (b->frequency - a->frequency);
}

void countFrequency(void *arg){
	
	char *fileName = (char*)malloc(sizeof(char));
	fileName = (char*)arg;
	WordArray *words = (WordArray*)calloc((1000),sizeof(WordArray));
	char *result = (char*)malloc(sizeof(result)*100);
	int counter = 0;
	int isUnique;
	int i = 0;
	FILE *file;
	char buff[1000];

	// Clear the array, so threads don't modify the program after they've been created
	for (i = 0; i < sizeof(WordArray); i++){
		words[i].word[0] = '\0';
		words[i].frequency = 0;
	}

	file = fopen(fileName, "r");
	if (file == NULL){
		printf("Couldn't open file: ");
	}
			
	else {
		while ( (fscanf(file, "%s", buff)) != EOF)
		{
			isUnique = -1;

			// String comparison - to check if the word is already in the array
			int k;
			for (k = 0; k < counter; k++){
				if (strcmp(words[k].word, buff) == 0){
					isUnique = k;
				}
			}
			// If the string is not in the array, put it in the array
			if (isUnique == -1){
				strcpy(words[counter].word, buff);
				words[counter].frequency = 1;
				counter++;
			}
			// Increase frequency of non-unique words
			else {
				words[isUnique].frequency++;
			}
			// Re-allocate memory for the array
			words = realloc(words, (sizeof(*words) + counter) * sizeof(WordArray));	
		}
	}

	// Sort the words to find most frequent words
	qsort(words, counter, sizeof(WordArray), compareWords);
	
	fclose(file);
	
	printf("%s", fileName);
	printf(" = %d", counter);
	printf(" matching words\n");

	FILE *fp;
	int n;

	fp=fopen("Matching words.txt", "w");
	//loops through words array and prints the word and its frequency
	for (n=0;n<=counter;n++){
	fprintf(fp,"%s", words[n].word);
	
	fprintf(fp," = %d\n", words[n].frequency);
	}
	fclose(fp);

}

int main(int argc, int *argv[]){
	
	// Declare threads and default attributes
	pthread_t threads[argc-1];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	char* file1 = "WordFile1.txt";
	char* file2 = "WordFile2.txt";
	char* file3 = "WordFile3.txt";
	char* file4 = "WordFile4.txt";   

	printf("%d",argc-1);
	printf(" threds used \n");



	// assign files to threads
	int i;
	

	if (argc-1 > 4){
	
	printf("Too many arguments added. No more then 4 \n");

	}
	//if 4 threads
	if (argc-1 == 4){
		
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file1);
		pthread_create(&threads[1], &attr, (void*) countFrequency, (void*) file2);
		pthread_create(&threads[2], &attr, (void*) countFrequency, (void*) file3);
		pthread_create(&threads[3], &attr, (void*) countFrequency, (void*) file4);
	}
	//if 3 threads
	if (argc-1 == 3){
		// Create a new thread for every argument passed in by the user, and count word frequency for each
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file1);
		pthread_create(&threads[1], &attr, (void*) countFrequency, (void*) file2);
		pthread_create(&threads[2], &attr, (void*) countFrequency, (void*) file3);
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file4);
	}
	//if 2 threads
	if (argc-1 == 2){
		// Create a new thread for every argument passed in by the user, and count word frequency for each
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file1);
		pthread_create(&threads[1], &attr, (void*) countFrequency, (void*) file2);
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file3);
		pthread_create(&threads[1], &attr, (void*) countFrequency, (void*) file4);
	}
	// if 1 thread
	if (argc-1 == 1){
		// Create a new thread for every argument passed in by the user, and count word frequency for each
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file1);
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file2);
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file3);
		pthread_create(&threads[0], &attr, (void*) countFrequency, (void*) file4);
	}
	for (i = 1; i < argc; i++){
		// Join threads to avoid memory leaks
		pthread_join(threads[i], NULL);
	}

	return 0;
}
