#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"


int main(int argc, char **argv)
{
	//create hashmap to hold dictionary words
	hashmap_t hashtable[HASH_SIZE];
	char * misspelled[1000];

	printf("\nLoading dictionary\n");

	//load dictionary into hashmap
	bool success = load_dictionary(argv[2], hashtable);

	if (success)
	{
		printf("\nDictionary successfully loaded\n");
		FILE * fp = fopen(argv[1], "r");
		if (fp == NULL)
		{	
			printf("Could not open test file\n");	
			return false;
		}
		printf("\n Checking for misspelled words\n");
		int num_misspelled = check_words(fp, hashtable, misspelled);
		printf("Number of misspelled words: %d\n", num_misspelled); 
	}
	else
	{
		printf("\nDictionary failed to load\n");
	}

	

	return 0;
}
