#include <gtest/gtest.h>

#include "StudentManagement.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {

class CoutCapture {
public:
    CoutCapture() : previousBuffer_(std::cout.rdbuf(output_.rdbuf())) {}

    ~CoutCapture() {
        std::cout.rdbuf(previousBuffer_);
    }

    std::string str() const {
        return output_.str();
    }

private:
    std::ostringstream output_;
    std::streambuf* previousBuffer_;
};

}  // namespace

TEST(StudentManagerTest, AddsValidStudent) {
    StudentManager manager;
    const Student student(1, "Avery", 87.5);

    EXPECT_TRUE(manager.addStudent(student));

    const auto addedStudent = manager.findStudentById(1);
    ASSERT_TRUE(addedStudent.has_value());
    EXPECT_EQ("Avery", addedStudent->getName());
    EXPECT_EQ(87.5, addedStudent->getMarks());
}

TEST(StudentManagerTest, RejectsDuplicateStudentId) {
    StudentManager manager;
    const Student firstStudent(1, "Avery", 87.5);
    const Student duplicateStudent(1, "Jordan", 92.0);

    ASSERT_TRUE(manager.addStudent(firstStudent));
    EXPECT_FALSE(manager.addStudent(duplicateStudent));

    const auto storedStudent = manager.findStudentById(1);
    ASSERT_TRUE(storedStudent.has_value());
    EXPECT_EQ("Avery", storedStudent->getName());
}

TEST(StudentManagerTest, FindsExistingStudentById) {
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Morgan", 73.0)));

    const auto student = manager.findStudentById(7);

    ASSERT_TRUE(student.has_value());
    EXPECT_EQ(7, student->getId());
    EXPECT_EQ("Morgan", student->getName());
    EXPECT_EQ(73.0, student->getMarks());
}

TEST(StudentManagerTest, ReturnsEmptyWhenStudentIdDoesNotExist) {
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(7, "Morgan", 73.0)));

    EXPECT_FALSE(manager.findStudentById(99).has_value());
}

TEST(StudentManagerTest, RejectsNonPositiveStudentId) {
    StudentManager manager;

    EXPECT_FALSE(manager.addStudent(Student(0, "Avery", 87.5)));
    EXPECT_FALSE(manager.addStudent(Student(-1, "Jordan", 92.0)));
    EXPECT_FALSE(manager.findStudentById(0).has_value());
}

TEST(StudentManagerTest, RejectsEmptyOrWhitespaceOnlyName) {
    StudentManager manager;

    EXPECT_FALSE(manager.addStudent(Student(1, "", 87.5)));
    EXPECT_FALSE(manager.addStudent(Student(2, " \t\r\n", 87.5)));
}

TEST(StudentManagerTest, RejectsMarksOutsideInclusiveRange) {
    StudentManager manager;

    EXPECT_FALSE(manager.addStudent(Student(1, "Below", -0.01)));
    EXPECT_FALSE(manager.addStudent(Student(2, "Above", 100.01)));
}

TEST(StudentManagerTest, RejectsNonFiniteMarks) {
    StudentManager manager;
    const double notANumber =
        std::numeric_limits<double>::quiet_NaN();
    const double infinity =
        std::numeric_limits<double>::infinity();

    EXPECT_FALSE(manager.addStudent(Student(1, "NaN", notANumber)));
    EXPECT_FALSE(manager.addStudent(Student(2, "Infinity", infinity)));
    EXPECT_FALSE(manager.addStudent(Student(3, "Negative infinity", -infinity)));
}

TEST(StudentManagerTest, AcceptsMarksAtInclusiveBoundaries) {
    StudentManager manager;

    EXPECT_TRUE(manager.addStudent(Student(1, "Zero", 0.0)));
    EXPECT_TRUE(manager.addStudent(Student(2, "Maximum", 100.0)));
}

TEST(StudentManagerTest, EmptyCollectionHasNoAverageOrTopStudent) {
    StudentManager manager;

    EXPECT_FALSE(manager.calculateAverageMarks().has_value());
    EXPECT_FALSE(manager.findTopStudent().has_value());
    EXPECT_FALSE(manager.findStudentById(1).has_value());
}

TEST(StudentManagerTest, CalculatesAverageForAddedStudents) {
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(1, "Avery", 80.0)));
    ASSERT_TRUE(manager.addStudent(Student(2, "Jordan", 100.0)));

    const auto average = manager.calculateAverageMarks();

    ASSERT_TRUE(average.has_value());
    EXPECT_DOUBLE_EQ(90.0, *average);
}

TEST(StudentManagerTest, FindsStudentWithHighestMarks) {
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(1, "Avery", 80.0)));
    ASSERT_TRUE(manager.addStudent(Student(2, "Jordan", 100.0)));
    ASSERT_TRUE(manager.addStudent(Student(3, "Morgan", 95.0)));

    const auto topStudent = manager.findTopStudent();

    ASSERT_TRUE(topStudent.has_value());
    EXPECT_EQ(2, topStudent->getId());
    EXPECT_EQ(100.0, topStudent->getMarks());
}

TEST(StudentManagerTest, DisplaysMessageForEmptyCollection) {
    StudentManager manager;
    CoutCapture capture;

    manager.displayStudents();

    EXPECT_EQ("No students to display.\n", capture.str());
}

TEST(StudentManagerTest, DisplaysAddedStudentDetails) {
    StudentManager manager;
    ASSERT_TRUE(manager.addStudent(Student(1, "Avery", 87.5)));
    CoutCapture capture;

    manager.displayStudents();

    EXPECT_NE(std::string::npos, capture.str().find("Avery"));
    EXPECT_NE(std::string::npos, capture.str().find("87.50"));
}

// Update and delete tests are omitted: StudentManager has no such public APIs.
