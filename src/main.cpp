#include "StudentManagement.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace {

template <typename Number>
std::optional<Number> readNumber(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return std::nullopt;
        }

        std::istringstream input(line);
        Number value{};
        char extraCharacter;
        if (input >> value && !(input >> extraCharacter)) {
            return value;
        }

        std::cout << "Invalid input. Please enter a valid number.\n";
    }
}

std::optional<std::string> readName() {
    while (true) {
        std::cout << "Enter student name: ";
        std::string name;
        if (!std::getline(std::cin, name)) {
            return std::nullopt;
        }

        if (name.find_first_not_of(" \t\r\n") != std::string::npos) {
            return name;
        }

        std::cout << "Name cannot be empty.\n";
    }
}

bool addStudent(StudentManager& manager) {
    const auto id = readNumber<int>("Enter student ID: ");
    if (!id) {
        return false;
    }
    if (*id <= 0) {
        std::cout << "Student ID must be positive.\n";
        return true;
    }

    const auto name = readName();
    if (!name) {
        return false;
    }

    const auto marks = readNumber<double>("Enter marks (0-100): ");
    if (!marks) {
        return false;
    }
    if (!std::isfinite(*marks) || *marks < 0.0 || *marks > 100.0) {
        std::cout << "Marks must be between 0 and 100.\n";
        return true;
    }

    if (manager.addStudent(Student(*id, *name, *marks))) {
        std::cout << "Student added successfully.\n";
    } else {
        std::cout << "A student with that ID already exists.\n";
    }
    return true;
}

void displayStudent(const Student& student) {
    std::cout << "ID: " << student.getId()
              << "\nName: " << student.getName()
              << "\nMarks: " << std::fixed << std::setprecision(2)
              << student.getMarks() << '\n';
}

}  // namespace

int main() {
    StudentManager manager;

    while (true) {
        std::cout << "\nStudent Management System\n"
                  << "1. Add student\n"
                  << "2. Display students\n"
                  << "3. Find student by ID\n"
                  << "4. Calculate average marks\n"
                  << "5. Find top student\n"
                  << "6. Filter students by marks range\n"
                  << "0. Exit\n";

        const auto choice = readNumber<int>("Choose an option: ");
        if (!choice) {
            std::cout << "\nInput ended. Exiting.\n";
            break;
        }

        switch (*choice) {
        case 1:
            if (!addStudent(manager)) {
                std::cout << "\nInput ended. Exiting.\n";
                return 0;
            }
            break;
        case 2:
            manager.displayStudents();
            break;
        case 3: {
            const auto id = readNumber<int>("Enter student ID to find: ");
            if (!id) {
                std::cout << "\nInput ended. Exiting.\n";
                return 0;
            }
            const auto student = manager.findStudentById(*id);
            if (student) {
                displayStudent(*student);
            } else {
                std::cout << "Student not found.\n";
            }
            break;
        }
        case 4: {
            const auto average = manager.calculateAverageMarks();
            if (average) {
                std::cout << "Average marks: " << std::fixed
                          << std::setprecision(2) << *average << '\n';
            } else {
                std::cout << "No students available to calculate an average.\n";
            }
            break;
        }
        case 5: {
            const auto topStudent = manager.findTopStudent();
            if (topStudent) {
                std::cout << "Top student:\n";
                displayStudent(*topStudent);
            } else {
                std::cout << "No students available to find a top student.\n";
            }
            break;
        }
        case 6: {
            const auto minimumMarks =
                readNumber<double>("Enter minimum marks (0-100): ");
            if (!minimumMarks) {
                std::cout << "\nInput ended. Exiting.\n";
                return 0;
            }

            const auto maximumMarks =
                readNumber<double>("Enter maximum marks (0-100): ");
            if (!maximumMarks) {
                std::cout << "\nInput ended. Exiting.\n";
                return 0;
            }

            const auto matchingStudents =
                manager.findStudentsByMarksRange(*minimumMarks, *maximumMarks);
            if (!matchingStudents) {
                std::cout << "Invalid marks range. Enter values from 0 to 100 "
                             "with minimum no greater than maximum.\n";
            } else if (matchingStudents->empty()) {
                std::cout << "No students found in that marks range.\n";
            } else {
                std::cout << "Students in marks range:\n";
                for (const Student& student : *matchingStudents) {
                    displayStudent(student);
                }
            }
            break;
        }
        case 0:
            std::cout << "Goodbye.\n";
            return 0;
        default:
            std::cout << "Invalid option. Please choose from the menu.\n";
            break;
        }
    }

    return 0;
}