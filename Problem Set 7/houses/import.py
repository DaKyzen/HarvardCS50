import csv
from sys import argv
from cs50 import SQL

db = SQL("sqlite:///students.db")
db.execute("DELETE FROM students")

def main():
    if (len(argv) < 2):
        print("Usage: python import.py characters.csv")

    else:
        studentReader = getStudentReader()
        for students in studentReader:
            names = getSplitNameAsDict(students["name"])
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                        names["firstName"], names["middleName"], names["lastName"], students["house"], students["birth"])

def getStudentReader():
    csvFilename = argv[1]
    studentFile = open(csvFilename, "r")
    studentReader =  csv.DictReader(studentFile)
    return studentReader

def getSplitNameAsDict(name):
    splitName = name.split()
    if len(splitName) == 2:
        return dict(firstName = splitName[0], middleName = None, lastName = splitName[1])
    elif len(splitName) >= 3:
        middleNameList = [splitName[index] for index in range(1, len(splitName) - 1)]
        middleNameString = ""
        middleNameString = middleNameString.join(middleNameList)
        return dict(firstName = splitName[0], middleName = middleNameString, lastName = splitName[len(splitName) - 1])

main()


