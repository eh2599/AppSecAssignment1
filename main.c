#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

int main(int argc, char **argv)
{
	//create hashmap to hold dictionary words
	hashmap_t hashtable[HASH_SIZE];

	printf("\nLoading dictionary\n");

	//load dictionary into hashmap
	bool success = load_dictionary("wordlist.txt", hashtable);

	if (success)
	{
		printf("\nDictionary successfully loaded");
	}
	else
	{
		printf("\nDictionary failed to load\n");
	}

	return 0;
}
