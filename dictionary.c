// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

const unsigned int N = 50;

node *table[N] = {NULL};

int dict_count = 0;

bool check(const char *word)
{
    int length = strlen(word);
    char test_word[length + 1];

    for (int i = 0; i < length; i++)
    {
        test_word[i] = tolower(word[i]);
    }
    test_word[length] = '\0';

    int hash_index = hash(test_word);
    node *cursor = table[hash_index];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, test_word) != 0)
            cursor = cursor->next;
        else
            return true;
    }
    return false;
    free(cursor);
}

unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);

    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *word++;
        c = tolower(c);
    }
    return hash % N;
}//Source: djb2 by Dan Bernstein

bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char scan_word[LENGTH + 1];
    while (fscanf(file, "%s", scan_word) != EOF)
    {
        node *n = malloc(sizeof(node));

        strncpy(n->word, scan_word, sizeof(scan_word));

        int hash_index = hash(scan_word);
        if (table[hash_index] == NULL)
        {
            table[hash_index] = n;
            n->next = NULL;
        }
        else
        {
            n->next = table[hash_index];
            table[hash_index] = n;
        }
        dict_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
