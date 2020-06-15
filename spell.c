#include <stdlib.h>
#include "dictionary.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


char * remove_punctuation(char * word)
{
	char * word_without_punctuation;
	int i = 0;
	int j = 0;
	while(! isalpha(word[i]))
	{
		i++;
	}
	while(i < strlen(word))
	{
		word_without_punctuation[j] = word[i];
		i++;
		j++;
	}
	i = strlen(word);
	while (! isalpha(word[i]) && i > 0)
	{
		word[i] = '\0';
		i--;
	}
	return word_without_punctuation;
}

char *strlwr(const char *str)
{
	unsigned char *p = (unsigned char *)str;
	while (*p)
	{
		*p = tolower((unsigned char)*p);
		p++;
	}

	return str;
}

bool check_word(const char* word, hashmap_t hashtable[])
{
	int bucket;
	bucket = hash_function(word);
	hashmap_t cursor = hashtable[bucket];

	while (cursor != NULL)
	{
		if (strcmp(word, cursor->word) == 0)
		{
			return true;
		}
		cursor = cursor->next;
	}
	
	char * lwrcase_word;

	/***for(int i = 0; i < strlen(word); i++)
	{
		lwrcase_word[i] = tolower(word[i]);
	}***/

	lwrcase_word = strlwr(word);

	bucket = hash_function(lwrcase_word);
	cursor = hashtable[bucket];

	while (cursor != NULL)
	{
		if (strcmp(lwrcase_word, cursor->word) == 0)
		{
			return true;
		}
		cursor = cursor->next;
	}
	return false;
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

	char word[1024];
	int bucket;
	//Add words from file to hashtable
	while (fscanf (dict_file, "%1023s", word) == 1)
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
	//Close dictionary file	
	fclose(dict_file);
	/***for(i = 0; i < 20; i++)
	{
		if (hashtable[i] != NULL)
		{
			printf("%s\n", hashtable[i]->word);
		}
	}***/

	return true;

}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	int num_misspelled = 0;
	char * line;
	size_t len = 0;
	ssize_t line_size;
	char * word;
	int i;
	

	//Loop through file until the end
	while(getline(&line, &len, fp) != -1)
	{
		//printf("%s\n", line);
		word = strtok(line, " ,.!?\n");
		//printf("%s\n", word);
		while(word != NULL)
		{
			//new_word = remove_punctuation(word);
			//printf("Successfully removed punctuation from %s\n", word);
			if (!check_word(word, hashtable))
			{
				printf("%s\n", word);
				word = misspelled[num_misspelled];
				num_misspelled++;
			}
			word = strtok(NULL, " ,.!?\n");
			
		}
		line = NULL;
	}

	free(line);
	line = NULL;
	fclose(fp);

	return num_misspelled;
}
