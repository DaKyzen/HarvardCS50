def main():
    height = getInt("Height")
    printDoubleHalfPyramid(height)

def getInt(label):
    while True:
        num = input(f"{label}: ")
        if (num.isdigit()):
            return int(num)

def printDoubleHalfPyramid(height):
    numBlocks = 1;
    numSpaces = height - numBlocks;
    separator = " "
    block = "#"
    while (numBlocks <= height):
        separators = separator * numSpaces
        blocks = block * numBlocks

        print(separators + blocks + separator + blocks + separators)

        numBlocks += 1
        numSpaces -= 1

main()
