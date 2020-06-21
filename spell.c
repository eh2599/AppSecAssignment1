#include <stdlib.h>
#include "dictionary.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

bool check_word(const char* word, hashmap_t hashtable[])
{
	//Remove Punctuation from end
	int i,j;
	char * str = strdup(word);
	bool is_digit = true;
	for(i = strlen(str)-1; !isalpha(str[i]) && !isdigit(str[i]); i--)
	{
		str[i] = '\0';
	}
	char *str2;
	for(str2 = str; ispunct(*str2); ++str2);
	strcpy(str, str2);

	for(i = 0; i < strlen(str); i++)
	{
		if(!isdigit(str[i]))
		{
			is_digit = false;
			break;
		}
	}

	if (is_digit == true)
	{
		return true;
	}

	int bucket;
	bucket = hash_function(str);
	hashmap_t cursor = hashtable[bucket];

	while (cursor != NULL)
	{
		if (strcmp(str, cursor->word) == 0)
		{
			return true;
		}
		cursor = cursor->next;
	}
	
	
	char * lwrcase_word = strdup(str);

	unsigned char *p = (unsigned char *)lwrcase_word;
	while(*p)
	{
		*p = tolower((unsigned char)*p);
		p++;
	}

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
	

	return true;

}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	int num_misspelled = 0;
	char * line;
	size_t len = 0;
	ssize_t line_size;
	char * word;
	int i,j;
	char * str;
	

	//Loop through line by line until the end of file
	while(getline(&line, &len, fp) != -1)
	{
		//Split line by spaces and punctuation
		word = strtok(line, " \n");
		while(word != NULL)
		{

			//Remove punctuation
			str = strdup(word);
			for(i = strlen(str)-1; !isalpha(str[i]) && !isdigit(str[i]); i--)
			{
				str[i] = '\0';
			}
			char *str2;
			for(str2 = str; ispunct(*str2); ++str2);
			strcpy(str, str2);
			
			//Check to see if the word is in the dictionary			
			if (!check_word(str, hashtable))
			{
				//Add misspelled word to the array
				misspelled[num_misspelled] = str;
				num_misspelled++;
				printf("%s\n", str);
			}
			//Get next word from line
			word = strtok(NULL, " \n");
			
		}
		line = NULL;
	}

	free(line);
	line = NULL;
	fclose(fp);

	return num_misspelled;
}
