#ifndef STUDENT_MANAGEMENT_H
#define STUDENT_MANAGEMENT_H

#include "Student.h"

#include <optional>
#include <vector>

class StudentManager {
public:
    bool addStudent(const Student& student);
    void displayStudents() const;
    std::optional<Student> findStudentById(int id) const;
    std::optional<double> calculateAverageMarks() const;
    std::optional<Student> findTopStudent() const;

private:
    std::vector<Student> students_;
};

#endif