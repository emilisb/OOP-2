//
//  StudentCollection.cpp
//  Uzduotis 2
//
//  Created by Emilis Baliukonis on 21/02/2019.
//  Copyright © 2019 Emilis Baliukonis. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include "StudentCollection.hpp"
#include "Console/Table.hpp"
#include "Console/Row.hpp"
#include "Console/Console.hpp"

using std::cout;
using std::endl;

void StudentCollection::printFileHeader(std::ofstream &file) {
    file << "Vardas Pavarde ND1 ND2 ND3 ND4 ND5 Egzaminas" << '\n';
}

void StudentCollection::printStudentToFile(std::ofstream &file, const Student &student) {
    file << student.firstName << " " << student.lastName << " ";
    for (int i = 0; i < 5; i++) {
        file << student.homeworkResults.at(i) << " ";
    }
    file << student.examResult << '\n';
}

void StudentCollection::writeStudentsByTypeToFile(string badStudentsFilename, string goodStudentsFilename) {
    std::ofstream badStudentsFile(badStudentsFilename);
    std::ofstream goodStudentsFile(goodStudentsFilename);
    
    printFileHeader(badStudentsFile);
    printFileHeader(goodStudentsFile);
    
    for (const auto &student : students) {
        if (student.isGood) {
            printStudentToFile(goodStudentsFile, student);
        } else {
            printStudentToFile(badStudentsFile, student);
        }
    }
}

void StudentCollection::writeGroupedStudentsToFile(string badStudentsFilename, string goodStudentsFilename) {
    std::ofstream badStudentsFile(badStudentsFilename);
    std::ofstream goodStudentsFile(goodStudentsFilename);
    
    printFileHeader(badStudentsFile);
    printFileHeader(goodStudentsFile);
    
    for (const auto &student : badStudents) {
        printStudentToFile(badStudentsFile, student);
    }
    
    for (const auto &student : goodStudents) {
        printStudentToFile(goodStudentsFile, student);
    }
}

void StudentCollection::writeMovedBadStudentsToFile(string badStudentsFilename, string goodStudentsFilename) {
    std::ofstream badStudentsFile(badStudentsFilename);
    std::ofstream goodStudentsFile(goodStudentsFilename);
    
    printFileHeader(badStudentsFile);
    printFileHeader(goodStudentsFile);
    
    for (const auto &student : badStudents) {
        printStudentToFile(badStudentsFile, student);
    }
    
    for (const auto &student : students) {
        printStudentToFile(goodStudentsFile, student);
    }
}

void StudentCollection::generateRandomFile(string filename, int numOfStudents) {
    Student student;
    std::ofstream file(filename);
    printFileHeader(file);
    
    for (int i = 0; i < numOfStudents; i++) {
        student = getRandomStudent(5, i);
        printStudentToFile(file, student);
    }
    
    file.close();
}

void StudentCollection::loadFromFile(string filename, int numHomeworkResults) {
    std::ifstream file(filename);
    if (!file.fail()) {
        string tempLine;
        std::getline(file, tempLine); // ignore first line
        
        while (!file.eof()) {
            Student student;
            file >> student.firstName >> student.lastName;
            
            if (student.firstName == "") {
                break;
            }
            
            int result;
            for (int i = 0; i < numHomeworkResults; i++) {
                file >> result;
                student.homeworkResults.push_back(result);
            }
            
            file >> student.examResult;
            
            students.push_back(student);
        }
    } else {
        std::cout << "Failo skaitymo klaida!" << std::endl;
    }
    
    file.close();
}

Student StudentCollection::getRandomStudent(int numOfHomework, int id) {
    Student student;
    student.firstName = "Vardas" + std::to_string(id);
    student.lastName = "Pavarde" + std::to_string(id);
    
    if (!numOfHomework) {
        numOfHomework = randomGenerator.getNumber(1, 10);
    }
    
    for (int i = 0; i < numOfHomework; i++) {
        student.homeworkResults.push_back(randomGenerator.getNumber(1, 10));
    }
    
    student.examResult = randomGenerator.getNumber(1, 10);
    
    return student;
}

Student StudentCollection::getStudentFromInput() {
    Student student;
    unsigned int homeworkResult;
    
    student.firstName = Console::getStringWithQuestion("Vardas (arba /q jeigu norite baigti vesti duomenis):");
    if (student.firstName != EXIT_COMMAND) {
        student.lastName = Console::getStringWithQuestion("Pavardė:");
        bool fillingHomework = true;
        
        while (fillingHomework) {
            homeworkResult = Console::getIntegerWithQuestion("Namų darbų rez. (arba 0 jei norite baigti vesti duomenis):");
            if (homeworkResult != 0) {
                student.homeworkResults.push_back(homeworkResult);
            } else {
                fillingHomework = false;
            }
        }
        
        student.examResult = Console::getIntegerWithQuestion("Egzamino rez:");
    }
    
    return student;
}

void StudentCollection::loadFromConsole(bool useRandom) {
    if (useRandom) {
        const int numStudents = randomGenerator.getNumber(3, 10);
        
        for (int i = 0; i < numStudents; i++) {
            Student student = getRandomStudent(5, i);
            students.push_back(student);
        }
    } else {
        bool filling = true;
        
        while (filling) {
            Student student = getStudentFromInput();
            
            if (student.firstName == EXIT_COMMAND) {
                filling = false;
            } else {
                students.push_back(student);
            }
        }
    }
}

void StudentCollection::printResults() {
    Table table(3);
    table.setSeparatorRow(1);
    table.addRow( { "Vardas", "Pavarde", getFinalResultLabel() } );
    
    for (auto &student : students) {
        table.addRow( { student.firstName, student.lastName, student.getFinalResult() } );
    }
    
    table.print();
}

void StudentCollection::sortByName() {
#ifdef _CONTAINER_DEQUE_
    students.sort();
#else
    std::sort(students.begin(), students.end());
#endif
}

void StudentCollection::calculateMedian() {
    for (auto &student : students) {
        student.calculateMedian();
    }
}

void StudentCollection::calculateAverage() {
    for (auto &student : students) {
        student.calculateAverage();
    }
}

void StudentCollection::calculateFinal() {
    switch (finalResultMode) {
        case 'v':
            calculateAverage();
            break;
        case 'm':
            calculateMedian();
            break;
    }
}

string StudentCollection::getFinalResultLabel() {
    const string modeLabel = (finalResultMode == 'v' ? "Vid." : "Med.");
    return "Galutinis (" + modeLabel + ")";
}

void StudentCollection::setTypeByFinalResult() {
    for (auto &student : students) {
        student.setGoodStatus();
    }
}

void StudentCollection::pushByType() {
    for (const auto &student : students) {
        if (student.finalResult >= 5) {
            goodStudents.push_back(student);
        } else {
            badStudents.push_back(student);
        }
    }
}

void StudentCollection::moveBadStudents() {
    auto it = std::stable_partition(students.begin(), students.end(), [](Student student) {
        return student.finalResult < 5;
    });
    
    std::move(students.begin(), it, std::back_inserter(badStudents));
    students.erase(students.begin(), it);
}
