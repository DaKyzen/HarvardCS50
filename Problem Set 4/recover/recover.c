#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;
int fileNum = 0;

bool isStartOfNewJPEG(BYTE* bytes);
int calculateNumBlocksToRead(FILE* file, int blockSize);
FILE* createAndOpenJPEGFile(int fileNameNum);
BYTE* getArrayOfNBytes(size_t numElements);
void populateByteArrayWithImgBlock(BYTE* byteArray, int blockSize, FILE* fileToReadFrom);
long calculateFileSize(FILE* file);
void closeFile(FILE* file);
char* generateJPEGFileNameNum();

int main(int argc, char *argv[])
{
    if (argc != 2)
        printf("Usage: ./recover image\n");

    char* fileNameToRecover = argv[1];
    FILE* fileToRecover = fopen(fileNameToRecover, "r");
    if (fileToRecover == NULL)
        return 1;
    FILE* imgToRecover = NULL;

    int numBlocksToRead = calculateNumBlocksToRead(fileToRecover, BLOCK_SIZE);

    BYTE* byteArray = getArrayOfNBytes(BLOCK_SIZE);

    for (int i = 0; i < numBlocksToRead; i++)
    {
        populateByteArrayWithImgBlock(byteArray, BLOCK_SIZE, fileToRecover);

        if (isStartOfNewJPEG(byteArray))
        {
            closeFile(imgToRecover);
            imgToRecover = createAndOpenJPEGFile(fileNum);
            fwrite(byteArray, sizeof(BYTE), BLOCK_SIZE, imgToRecover);
        }
        else if (imgToRecover != NULL)
        {
            fwrite(byteArray, sizeof(BYTE), BLOCK_SIZE, imgToRecover);
        }
    }

    free(byteArray);
}

char* generateJPEGFileNameNum()
{
    char* fileName = malloc(sizeof(char) * 8);
    sprintf(fileName, "%03i.jpg", fileNum);
    fileNum++;
    return fileName;
}

void closeFile(FILE* file)
{
    if (file != NULL)
        fclose(file);
}

void populateByteArrayWithImgBlock(BYTE* byteArray, int blockSize, FILE* fileToReadFrom)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
        fread(&byteArray[i], sizeof(BYTE), sizeof(BYTE), fileToReadFrom);
}

BYTE* getArrayOfNBytes(size_t numElements)
{
    BYTE* bytes = malloc(numElements * sizeof(BYTE));
    return bytes;
}

long calculateFileSize(FILE* file)
{
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

int calculateNumBlocksToRead(FILE* file, int blockSize)
{
    long fileSizeInBytes = calculateFileSize(file);
    int numBlocksToRead = fileSizeInBytes / BLOCK_SIZE;
    return numBlocksToRead;
}

FILE* createAndOpenJPEGFile(int fileNameNum)
{
    char* fileName = generateJPEGFileNameNum();
    FILE* img = fopen(fileName, "w");
    free(fileName);
    return img;
}

bool isStartOfNewJPEG(BYTE* bytes)
{
    bool isValidFirstByte = (bytes[0] == 0xff);
    bool isValidSecondByte = (bytes[1] == 0xd8);
    bool isValidThirdByte = (bytes[2] == 0xff);
    bool isValidFourthByte = ((bytes[3] & 0xf0) == 0xe0);

    bool isStartOfJPEG = isValidFirstByte && isValidSecondByte && isValidThirdByte && isValidFourthByte;

    return isStartOfJPEG;
}
