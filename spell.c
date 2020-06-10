#include <stdlib.h>
#include "dictionary.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

bool check_word(const char* word, hashmap_t hashtable[])
{

	return true;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
	int i;
	//Initialize all values in hash table to NULL
	for (i = 0; i < HASH_SIZE; i++)
	{
		hashtable[i] = NULL;
	}

	//Open dictionary file
	FILE *dict_file = fopen(dictionary_file, "r");

	//Checking success of opening the file
	if (dict_file == NULL)
	{
		return false;
	}

	char word[45];
	int bucket;
	//Add words from file to hashtable
	while (fscanf (dict_file, "%43s", word) == 1)
	{
		hashmap_t new_node = malloc(sizeof(node));
		new_node->next = NULL;
		strcpy(new_node->word, word);
		bucket = hash_function(word);
		if (hashtable[bucket] == NULL)
		{
			hashtable[bucket] = new_node;
		}
		else
		{
			new_node->next = hashtable[bucket];
			hashtable[bucket] = new_node;
		}
	}
	fclose(dict_file);
	for(i = 0; i < 10; i++)
	{
		if (hashtable[i] != NULL)
		{
			printf("%s\n", hashtable[i]->word);
		}
	}

	return true;

}

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{

	return 0;
}
