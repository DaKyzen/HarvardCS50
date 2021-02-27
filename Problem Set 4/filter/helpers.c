#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int calculateAverage(double blue, double green, double red);
void changeToGrayscale(RGBTRIPLE* pixel, int average);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int average = calculateAverage(image[h][w].rgbtBlue, image[h][w].rgbtGreen, image[h][w].rgbtRed);
            changeToGrayscale(&image[h][w], average);
        }
    }

    return;
}
int calculateAverage(double blue, double green, double red)
{
    double average = round((blue + green + red) / 3);
    if (average > 255)
        average = 255;

    return average;
}
void changeToGrayscale(RGBTRIPLE* pixel, int average)
{
    pixel->rgbtBlue = average;
    pixel->rgbtGreen = average;
    pixel->rgbtRed = average;
}

/*==============================================*/

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            RGBTRIPLE tempPixel = image[h][w];
            image[h][w] = image[h][width - 1 - w];
            image[h][width - 1 - w] = tempPixel;
        }
    }
    return;
}

/*==============================================*/

RGBTRIPLE getAverageColour(RGBTRIPLE* pixelBoxArray);
RGBTRIPLE* createPixelBoxArray(int height, int width, RGBTRIPLE image[height][width], int currentRow, int currentCol, int level, int size);
void blurPixelBoxArray(int height, int width, RGBTRIPLE image[height][width], int currentRow, int currentCol, RGBTRIPLE* pixelBoxArray, int size);
double getValidAverageColour(double sumColourValue, int divisor);
RGBTRIPLE* create2DimensionalRGBArray(int height, int width);
int getPixelBoxSize(int height, int width, int currentRow, int currentCol);
void copyEditedImageToOriginalImage(int height, int width, RGBTRIPLE originalImage[height][width], RGBTRIPLE editedImage[height][width]);
bool isValidPixelCoordinate(int height, int width, int row, int col);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imageCopy[height][width];

    for (int currentRow = 0; currentRow < height; currentRow++)
    {
        for (int currentCol = 0; currentCol < width; currentCol++)
        {
            int pixelBoxSize = getPixelBoxSize(height, width, currentRow, currentCol);

            if (pixelBoxSize > 0)
            {
                RGBTRIPLE* pixelBoxArray = createPixelBoxArray(height, width, image, currentRow, currentCol, 1, pixelBoxSize);
                blurPixelBoxArray(height, width, imageCopy, currentRow, currentCol, pixelBoxArray, pixelBoxSize);
                free(pixelBoxArray);
            }
        }
    }
    copyEditedImageToOriginalImage(height, width, image, imageCopy);

    return;
}
void copyEditedImageToOriginalImage(int height, int width, RGBTRIPLE originalImage[height][width], RGBTRIPLE editedImage[height][width])
{
    for (int currentRow = 0; currentRow < height; currentRow++)
    {
        for (int currentCol = 0; currentCol < width; currentCol++)
        {
            originalImage[currentRow][currentCol] = editedImage[currentRow][currentCol];
        }
    }
}
RGBTRIPLE* createPixelBoxArray(int height, int width, RGBTRIPLE image[height][width], int currentRow, int currentCol, int level, int pixelBoxSize)
{
    RGBTRIPLE* pixelBoxArray = malloc(pixelBoxSize * sizeof(pixelBoxArray));
    if (pixelBoxArray == NULL)
        printf("Error. Allocation unsuccessful.");

    int rowStep = -level;
    int colStep = -level;
    int pixelBoxIndex = 0;

    for (int i = 0; i < 9; i++)
    {
        int row = currentRow + rowStep;
        int col = currentCol + colStep;

        if (isValidPixelCoordinate(height, width, row, col))
        {
            pixelBoxArray[pixelBoxIndex] = image[row][col];
            pixelBoxIndex ++;
        }

        if (colStep < level)
            colStep ++;
        else
        {
            colStep = -level;
            rowStep ++;
        }
    }
    return pixelBoxArray;
}
bool isValidPixelCoordinate(int height, int width, int row, int col)
{
    bool isValidCol = (row >= 0 && row < height);
    bool isValidRow = (col >= 0 && col < width);

    return (isValidCol && isValidRow);
}
int getPixelBoxSize(int height, int width, int currentRow, int currentCol)
{
    int size = 0;
    bool isVerticalEdge = (currentRow == 0 || currentRow == height - 1);
    bool isHorizontalEdge = (currentCol == 0 || currentCol == width - 1);
    bool isEdge = (isVerticalEdge || isHorizontalEdge);
    bool isCorner = (isVerticalEdge && isHorizontalEdge);
    bool isEdgeAndNotCorner = (isEdge && !isCorner);

    if (!isVerticalEdge && !isHorizontalEdge)
        size = 9;
    else if (isEdgeAndNotCorner)
        size = 6;
    else if (isCorner)
        size = 4;

    return size;
}
void blurPixelBoxArray(int height, int width, RGBTRIPLE image[height][width], int currentRow, int currentCol, RGBTRIPLE* pixelBoxArray, int pixelBoxSize)
{
    double sumBlue = 0;
    double sumGreen = 0;
    double sumRed = 0;

    for (int i = 0; i < pixelBoxSize; i++)
    {
        sumBlue += pixelBoxArray[i].rgbtBlue;
        sumGreen += pixelBoxArray[i].rgbtGreen;
        sumRed += pixelBoxArray[i].rgbtRed;
    }
    double avgBlue = getValidAverageColour(sumBlue, pixelBoxSize);
    double avgGreen = getValidAverageColour(sumGreen, pixelBoxSize);
    double avgRed = getValidAverageColour(sumRed, pixelBoxSize);

    image[currentRow][currentCol].rgbtBlue = avgBlue;
    image[currentRow][currentCol].rgbtGreen = avgGreen;
    image[currentRow][currentCol].rgbtRed = avgRed;
}
double getValidAverageColour(double sumColourValue, int divisor)
{
    if (divisor > 0)
    {
        double average = round(sumColourValue / divisor);
        if (average > 255)
            average = 255;
        else if (average < 0)
            average = 0;
        return average;
    }
    else
        return 0;

}

/*=======================================*/


void populatePixelBox(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE** pixelBox, int currentRow, int currentCol);
RGBTRIPLE** createPixelBox(int size);
RGBTRIPLE* createPixel(int red, int green, int blue);
void calculateGxOfPixelBox(RGBTRIPLE** pixelBox);
RGBTRIPLE* applySobelOperatorToPixel(RGBTRIPLE** pixelBox);
double combineGxAndGy(double val1, double val2);
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imageCopy[height][width];

    for (int currentRow = 0; currentRow < height; currentRow++)
    {
        for (int currentCol = 0; currentCol < width; currentCol++)
        {
            RGBTRIPLE** pixelBox = createPixelBox(3);
            populatePixelBox(height, width, image, pixelBox, currentRow, currentCol);
            RGBTRIPLE* pixel = applySobelOperatorToPixel(pixelBox);
            imageCopy[currentRow][currentCol] = *pixel;

            free(pixelBox);
            free(pixel);
        }
    }
    copyEditedImageToOriginalImage(height, width, image, imageCopy);
    return;
}

void populatePixelBox(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE** pixelBox, int currentRow, int currentCol)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            int translatedRow = currentRow + (row - 1);
            int translatedCol = currentCol + (col - 1);
            if (isValidPixelCoordinate(height, width, translatedRow, translatedCol))
                pixelBox[row][col] = image[translatedRow][translatedCol];
            else
            {
                RGBTRIPLE* blackPixel = createPixel(0, 0, 0);
                pixelBox[row][col] = *blackPixel;
                free(blackPixel);
            }
        }
    }
}
RGBTRIPLE** createPixelBox(int size)
{
    RGBTRIPLE** pixelBox = malloc(size * sizeof(RGBTRIPLE*));
    for (int i = 0; i < 3; i++)
    {
        pixelBox[i] = malloc(size * sizeof(RGBTRIPLE));
    }

    if (pixelBox == NULL)
        printf("Error. Allocation unsuccessful.");

    return pixelBox;
}
RGBTRIPLE* createPixel(int red, int green, int blue)
{
    RGBTRIPLE* pixel = malloc(sizeof(pixel));
    pixel->rgbtRed = red;
    pixel->rgbtGreen = green;
    pixel->rgbtBlue = blue;

    return pixel;
}

RGBTRIPLE* applySobelOperatorToPixel(RGBTRIPLE** pixelBox)
{
    int gx[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int gy[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    double gxBlue = 0;
    double gyBlue = 0;
    double gxGreen = 0;
    double gyGreen = 0;
    double gxRed = 0;
    double gyRed = 0;

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            gxBlue += pixelBox[row][col].rgbtBlue * gx[row][col];
            gyBlue += pixelBox[row][col].rgbtBlue * gy[row][col];
            gxGreen += pixelBox[row][col].rgbtGreen * gx[row][col];
            gyGreen += pixelBox[row][col].rgbtGreen * gy[row][col];
            gxRed += pixelBox[row][col].rgbtRed * gx[row][col];
            gyRed += pixelBox[row][col].rgbtRed * gy[row][col];
        }
    }

    RGBTRIPLE* pixel = malloc(sizeof(RGBTRIPLE));
    pixel->rgbtBlue = combineGxAndGy(gxBlue, gyBlue);
    pixel->rgbtGreen = combineGxAndGy(gxGreen, gyGreen);
    pixel->rgbtRed =combineGxAndGy(gxRed, gyRed);
    return pixel;
}
double combineGxAndGy(double val1, double val2)
{
    double squaredVal1 = val1 * val1;
    double squaredVal2 = val2 * val2;
    double squareRoot = sqrt(squaredVal1 + squaredVal2);
    double combinedValue = round(squareRoot);
    if (combinedValue > 255)
        combinedValue = 255;
    else if (combinedValue < 0)
        combinedValue = 0;
    return combinedValue;
}













