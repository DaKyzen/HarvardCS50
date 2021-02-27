// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 204415;

// Hash table
node *table[N];

int numWordsInDictionary = 0;


// Prototypes
node* allocateNewNode();
void populateNode(node* n, char* nword, node* nextNode);
void addToNodeHashTable(node* hashTable[N], int index, node* newNode);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    /*
        1. Has word to obtain a hash value
        2. Access linked list at that index in the has table
        3. Traverse linked list, looking for the word using strcasecmp()
    */

    int hashIndex = hash(word);
    node* tableNode = table[hashIndex];

    if (tableNode != NULL)
    {
        node* nodeToCheck = tableNode->next;

        do
        {
            bool isWordFound = (strcasecmp(nodeToCheck->word, word) == 0);
            if (isWordFound)
                return true;
            nodeToCheck = nodeToCheck->next;
        } while (nodeToCheck != NULL);

    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int length = strlen(word);
    int hashIndex = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        char letter = tolower(word[i]);
        hashIndex = 37 * hashIndex + letter;
    }
    hashIndex = hashIndex % N;
    return hashIndex;
}

// Loads dictionary into memory as a hashtable, returning true if successful else false
bool load(const char *dictionary)
{
    /*
        1. Open dictionary file
        2. Read strings from file one at a time
        3. Create a new node for each word
        4. Hash word to obtain a hash value
        5. Insert node into hash table at that location
    */

    FILE* dic = fopen(dictionary, "r");
    if (dic == NULL)
        return false;

    char tempWord[LENGTH + 1];

    while (fscanf(dic, "%s", tempWord) != EOF)
    {
        numWordsInDictionary ++;

        // Create new node and populate
        node* newNode = allocateNewNode();
        populateNode(newNode, tempWord, NULL);

        // Add new node to hash table
        int hashIndex = hash(newNode->word);
        addToNodeHashTable(table, hashIndex, newNode);
    }
    fclose(dic);

    return true;
}

void addToNodeHashTable(node* hashTable[N], int index, node* newNode)
{
    if (hashTable[index] == NULL)
    {
        hashTable[index] = malloc(sizeof(node));
        hashTable[index]->next = newNode;
    }
    else
    {
        newNode->next = hashTable[index]->next;
        hashTable[index]->next = newNode;
    }
}

node* allocateNewNode()
{
    node* newNode = malloc(sizeof(node));
    if (newNode == NULL)
        exit(1);
    return newNode;
}
void populateNode(node* n, char* nword, node* nextNode)
{
    strcpy(n->word, nword);
    if (nextNode!= NULL)
        n->next = nextNode;
    else
        n->next = NULL;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return numWordsInDictionary;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i ++)
    {
        node* nodeToFree = table[i];
        while (nodeToFree != NULL)
        {
            node* nextNode = nodeToFree->next;
            free(nodeToFree);
            nodeToFree = nextNode;
        }
    }
    return true;
}
