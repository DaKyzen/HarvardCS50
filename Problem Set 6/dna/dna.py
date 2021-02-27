'''
1. First command line argument is the name of the CSV file. Second command line is the name of the text file containing the DNA sequence to identify
2. Program should print an error message if incorrect command line inputted
3. Open CSV file into memory.
    First row = name
    Second row = STR sequences
4. Open DNA sequence into memory
5. Compute the longest run of the consecutive repeats of the STR in the DNA sequence to identify a match in the CSV file
6. If no match print "No match"
'''

from sys import argv
import csv

def main():
    if (len(argv) < 3):
        print("Usage: python dna.py data.csv sequence.txt")

    else:
        personDictReader = getPersonDictReader()
        dnaSequence = getDNASequence()

        isFound = False
        for personData in personDictReader:
            if (isMatchingSequence(personData, dnaSequence)):
                isFound = True
                print(personData["name"])
        if (not isFound):
            print("No match")


def isMatchingSequence(personData, dnaSequence):
    for key in personData:
        if key != "name":
            if str(countLongestSTRSequence(key, dnaSequence)) != personData[key]:
                return False
    return True

def getPersonDictReader():
    csvFilename = argv[1]
    personFile = open(csvFilename, "r")
    dictReader =  csv.DictReader(personFile)
    return dictReader

def getDNASequence():
    dnaFilename = argv[2]
    with open(dnaFilename, "r") as file:
        sequence = file.read().replace("\n", "")
        
    return sequence

def countLongestSTRSequence(STR, dnaSequence):
    count = 0
    while dnaSequence.count(STR + STR * count) > 0:
        count += 1
    return count

main()

