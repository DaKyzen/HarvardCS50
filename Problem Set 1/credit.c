#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int addDigits(int num);

// step - how many numbers to skip
// start - how many digits from the last to start adding
int addEveryNDigits(int num, int step, int start);
int getNDigit(long num, int pos); // pos starting from 0
int countDigits(long num);
bool luhnAlg(long num);
bool checkValidLength(long num);
string checkValidStart(long num);

int main()
{
    long input = get_long("Number: ");
    string type = checkValidStart(input);
    if ((strcmp(type, "INVALID") != 0) && checkValidLength(input) && luhnAlg(input))
        printf("%s\n", type);
    else
        printf("INVALID\n");

    return 0;
}

// Adds each digit of a number together. E.g. if 23 is passed, 5 is returned
int addDigits(int num)
{
    if(num < 10)
        return (num % 10);
    return (num % 10) + addDigits(num / 10);
}
// num is modified
int countDigits(long num)
{
    int numDigits = 0;
    while(num >= 1)
    {
        numDigits ++;
        num = num / 10;
    }
    return numDigits;
}
int getNDigit(long num, int pos)
{
    int numDigits = countDigits(num);
    for (size_t i = numDigits - 1; i > pos; i--)
        num /= 10;
        
    return num % 10;
}
bool luhnAlg(long num)
{
    int multiplied = 0;
    int notMultiplied = 0;
    int size = countDigits(num);

    // Multiply every other number and add its digits together
    for (int i = size - 2; i >= 0; i -= 2)
    {
        int temp = getNDigit(num, i) * 2;
        multiplied += addDigits(temp);
    }

    // Sum of digits not multiplied
    for (int i = size - 1; i >= 0; i -= 2)
        notMultiplied += getNDigit(num, i);
    
    int total = multiplied + notMultiplied;

    return !(total % 10); // If 0, returns true
    
}
bool checkValidLength(long num)
{
    int length = countDigits(num);
    return(length == 13 || length == 15 || length == 16);
}
string checkValidStart(long num)
{
    /* Get first two digits */
    long divisor = pow(10, floor(log10(num))) / 10; // Get number to divide num by to get two digits
    int firstTwoDigits = floor(num / divisor);  

    string type = "";

    if (getNDigit(num, 0) == 4)
        type = "VISA";
    else if (firstTwoDigits == 34 || firstTwoDigits == 37)
        type = "AMEX";
    else if (firstTwoDigits >= 51 && firstTwoDigits <= 55)
        type = "MASTERCARD";
    else
        type = "INVALID";

    return type;
}