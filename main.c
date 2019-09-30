#include "dictionary.h"
#include "spell.c"
#include "dictionary.c"


int main()
{

    hashmap_t hashtable[HASH_SIZE];

    load_dictionary("wordlist.txt", hashtable);

    char*misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test_wordlist.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);

    for (int i = 0; i < num_misspelled; i++)
    {
        printf("%s \n", misspelled[i]);

    }

    return 1;
}