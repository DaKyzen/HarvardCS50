#include <cs50.h>
#include <stdio.h>

void printTriangle(char a, char b, int num);
void printCharacter(char a, int num);
int askNumber(void);

int main()
{
    int number = askNumber();
    printTriangle(' ', '#', number);
}

int askNumber()
{
    int number = 0;
    do {
        number = get_int("Enter size of triangle: ");
    } while(number < 1 || number > 8);

    return number;
}

void printCharacter(char a, int num)
{
    for (int i = 0; i < num; i++)
        printf("%c", a);
}

void printTriangle(char a, char b, int num) // a = first character, b = second character
{
    int first, second;
    for(first = num - 1, second = 1; first >= 0; first--, second++)
    {
        printCharacter(a, first);
        printCharacter(b, second);
        printf("  ");
        printCharacter(b, second);
        printCharacter(a, first);
        printf("\n");
    }
        
}
