/*
    Coleman-Liau Index
    index = 0.0588 * L - 0.296 * S - 15.8
    L = Average number of letters per 100 words
    S = Average number of sentences per 100 words
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


int getNumLetters(string sentence);
int getNumWords(string sentence);
int getNumSentences(string sentence);
bool isLetter(char letter);
int roundToNearestTen(float num);

int main()
{
    // printf("%i letter(s)\n", getNumLetters(text));
    // printf("%i word(s)\n", getNumWords(text));
    // printf("%i sentence(s)\n", getNumSentences(text));
    // printf("%i", roundToNearestTen(10.9));

    string text = get_string("Text: ");
    float numLetters = getNumLetters(text);
    float numWords = getNumWords(text);
    float numSentences = getNumSentences(text);

    float l = (numLetters / numWords) * 100;
    float s = (numSentences / numWords) * 100;


    float index = (0.0588 * l) - (0.296 * s) - 15.8;
    int grade = roundToNearestTen(index);

    if (grade < 1)
        printf("Before Grade 1\n");
    else if (grade > 16)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", grade);
}

bool isLetter(char letter)
{
    if ((letter >= 65 && letter <= 90) || (letter >= 97 && letter <= 122))
        return true;
    return false;
}

int getNumLetters(string sentence)
{
    int count = 0;
    for (size_t i = 0, n = strlen(sentence); i < n; i++)
    {
        if(isLetter(sentence[i]))
            count ++;
    }

    return count;
}
int getNumWords(string sentence)
{
    int count = 0;
    for (size_t i = 0, n = strlen(sentence); i < n; i++)
    {
        if(sentence[i] == ' ')
            count ++;
    }
    return count + 1; //To count the last word after the last space e.g. "...along with him."
}
int getNumSentences(string sentence)
{
    int count = 0;
    for (size_t i = 0, n = strlen(sentence); i < n; i++)
    {
        if(sentence[i] == '.' || sentence[i] == '!' || sentence[i] == '?')
            count ++;
    }
    return count;
}
int roundToNearestTen(float num)
{
    int temp = floor(num * 10);
    if ((temp % 10) >= 5)
        temp += (10 - temp % 10);
    else
        temp -= temp % 10;

    return (int)(temp/10);
}







