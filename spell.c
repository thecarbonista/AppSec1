//
// Created by jmc1367 on 9/25/19.
//

//Program should contain at least 3 functions
//1. load_dictionary - Function to load the list or words into the hash map
//2. check_word - Function to check if a word is correctly spelled
//3. check_words - Function to tie together functions 1 & 2.

#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//#define DICTIONARY "wordlist.txt"
//#define TESTDICT "test_wordlist.txt"

//Number of items in the hash table
//#define HASH_SIZE 2000 --> from dictionary.h
//typedef node* hashmap_t; --> from dictionary.h


//Function Prototypes

//1. load_dictionary - Function to load the list or words into the hash map
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    //Initialize all values in hash table to NULL.
    for(int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    //Open dict_file from path stored in dictionary.
    FILE *fp= fopen(dictionary_file , "r");

    //If dict_file is NULL:
    //        return false.
    if(!fp)
    {
        //printf("***The dictionary failed to load***");
        return false;
    }

    //Set buffer to hold characters based on previously defined LENGTH + 1
    char buffer[LENGTH + 1];

    //While word in dict_file is not EOF (end of file):
    while(fgets(buffer, sizeof buffer, fp) != NULL)
    {
        //Set hashmap_t new_node to a new node.
        node* new_node = malloc(sizeof(new_node));

        //Set new_node->next to NULL.
        new_node->next = NULL;

        //Set new_node->word equal to word.
        strcpy(new_node->word, buffer);

        //Set int bucket to hash_function(word).
        int bucket = hash_function(new_node->word);

        //if hashtable[bucket] is NULL:
        //            Set hashtable[bucket] to new_node.
        if(hashtable[bucket] == NULL)
        {
            hashtable[bucket] = new_node;
        }

            //else:
            //            Set new_node->next to hashtable[bucket].
            //            Set hashtable[bucket] to new_node.
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }

    }
    //Close the dictionary file
    fclose(fp);

    return true;
}

//2. check_word - Function to check if a word is correctly spelled
bool check_word(const char* word, hashmap_t hashtable[])
{
    //Create variable for word_length
    int word_length = strlen(word);

    if(word_length > LENGTH)
    {
        return true;
    }

    //Create array to hold lowercase word
    char lower_word[LENGTH+1];

    // Convert word to lowercase to accurately compare to hash table.
    for (int i = 0; i < word_length; i++)
    {
        // make uppercase into lowercase
        if(isupper(word[i]))
        {
            lower_word[i] = tolower(word[i]) ;
        }
            // Already lowercase
        else
        {
            lower_word[i] = word[i];
        }
    }
    lower_word[word_length] = '\0';

    //    Set int bucket to the output of hash_function(word).
    //    Set hashmap_t cursor equal to hashmap[bucket].
    int bucket = hash_function(lower_word);
    node* cursor = hashtable[bucket];

    //    While cursor is not NULL:
    while(cursor != NULL)
    {
        //        If word equals cursor->word:
        //            return True.
        if(lower_word == cursor->word)
        {
            return true;
        }
        //Set cursor to cursor->next
        cursor = cursor->next;
    }
    //return false
    return false;
}



//3. check_words - Function to tie together functions 1 & 2.
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
    // prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];

    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp)) {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0)) {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH) {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

            // ignore words with numbers (like MS Word can)
        else if (isdigit(c)) {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

            // we must have found a whole word
        else if (index > 0) {
            // terminate current word
            word[index] = '\0';

            // update counter
            words++;

            // check word's spelling

            bool misspelled = !check_word(word, hashtable);

            // print word if misspelled
            if (misspelled)
            {
                misspellings++;
            }

            // prepare for next word
            index = 0;
        }
    }

    return misspellings;
}



//int main()
//{
//    printf("It compiled\n\n");
//    return 0;
//}



