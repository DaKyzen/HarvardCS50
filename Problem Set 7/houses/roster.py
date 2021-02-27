import csv
from sys import argv
from cs50 import SQL

db = SQL("sqlite:///students.db")


def main():
    if (len(argv) < 2):
        print("Usage: python roster.py [HOUSE NAME]")

    else:
        house = argv[1]
        studentRecords = getStudentsByHouseAsListOfDict(house)

        if (house == None):
            print("Invalid house name")
        else:
            for record in studentRecords:
                formattedRecord = formatStudentRecord(record)
                print(formattedRecord)

def getStudentsByHouseAsListOfDict(house):
    if house.title() not in ['Slytherin', 'Gryffindor', 'Hufflepuff', 'Ravenclaw']:
        return None
    students = db.execute("SELECT * FROM students WHERE students.house = (?) ORDER BY students.last, students.first", house.title())
    return students

def formatStudentRecord(record):
    middle = record["middle"] if record["middle"] != None else ""

    formattedString = "{firstName} {middleName} {lastName}, born {birth}".format(
        firstName = record["first"],
        middleName = middle,
        lastName = record["last"],
        birth = record["birth"])

    formattedString = formattedString.split()

    formattedString = " ".join(formattedString)
    return formattedString

main()