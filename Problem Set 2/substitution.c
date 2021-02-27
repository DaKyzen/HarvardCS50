#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int getLetterPosition(char letter);
char encryptLetter(char letter, string key);
bool isLetter(char letter);
void encryptWord(string word, string key);
bool isAllLetters(string word);

int main(int argc, string argv[])
{
    // printf("%i", getLetterPosition('z'));
    // printf("%c", encryptLetter('b', key));

    if ((strlen(argv[1]) == 26) && isAllLetters(argv[1]))
    {
        string wordToEncrypt = get_string("plaintext: ");
        printf("ciphertext: ");
        encryptWord(wordToEncrypt, argv[1]);
    }
    else
    {
        printf("A key of 26 letters is required.\n");
        return 1;
    }
    return 0;
}

// Get alphabetical position of letter
int getLetterPosition(char letter)
{
    return (toupper(letter) - 65); // returns a as the 0th position and z as the 25th position
}

char encryptLetter(char letter, string key)
{
    if (isLetter(letter))
    {
        int letterPosition = getLetterPosition(letter);
        int keyPosition = getLetterPosition(key[letterPosition]); // Gets the alphabetical position of the character at array position key[letterPosition]
        int encryptedLetter = letter + (keyPosition - letterPosition);
        return encryptedLetter;
    }
    return letter;
}

void encryptWord(string word, string key)
{
    for (size_t i = 0; i < strlen(word); i++)
        printf("%c", encryptLetter(word[i], key));

    printf("\n");
}

bool isAllLetters(string word)
{
    for (size_t i = 0; i < strlen(word); i++)
    {
        if (!isLetter(word[i]))
            return false;
    }
    return true;
    
}

bool isLetter(char letter)
{
    if ((letter >= 65 && letter <= 90) || (letter >= 97 && letter <= 122))
        return true;
    return false;
}