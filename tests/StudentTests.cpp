#include <gtest/gtest.h>

#include "Student.h"

#include <cmath>
#include <limits>
#include <string>

TEST(StudentTest, ConstructorStoresTypicalValues) {
    const Student student(42, "Avery", 87.5);

    EXPECT_EQ(42, student.getId());
    ASSERT_EQ(std::string("Avery"), student.getName());
    EXPECT_EQ(87.5, student.getMarks());
}

TEST(StudentTest, GettersReturnValuesForInvalidInputsUnchanged) {
    // Student does not validate IDs, names, or marks.
    const Student student(-7, "", -1.0);

    EXPECT_EQ(-7, student.getId());
    EXPECT_EQ(std::string(), student.getName());
    EXPECT_EQ(-1.0, student.getMarks());
}

TEST(StudentTest, GetIdReturnsIntegerLimits) {
    const Student minimumStudent(
        (std::numeric_limits<int>::min)(), "Minimum", 50.0);
    const Student maximumStudent(
        (std::numeric_limits<int>::max)(), "Maximum", 50.0);

    EXPECT_EQ((std::numeric_limits<int>::min)(), minimumStudent.getId());
    EXPECT_EQ((std::numeric_limits<int>::max)(), maximumStudent.getId());
}

TEST(StudentTest, GetMarksReturnsRangeBoundaries) {
    const Student zeroMarksStudent(1, "Zero", 0.0);
    const Student maximumMarksStudent(2, "Maximum", 100.0);

    EXPECT_EQ(0.0, zeroMarksStudent.getMarks());
    EXPECT_EQ(100.0, maximumMarksStudent.getMarks());
}

TEST(StudentTest, GetNameReturnsLongNameUnchanged) {
    const std::string longName(4096, 'S');
    const Student student(1, longName, 75.0);

    ASSERT_EQ(longName.size(), student.getName().size());
    EXPECT_EQ(longName, student.getName());
}

TEST(StudentTest, GetMarksPreservesSpecialFloatingPointValues) {
    const double notANumber =
        std::numeric_limits<double>::quiet_NaN();
    const double infinity =
        std::numeric_limits<double>::infinity();
    const Student nanStudent(1, "NaN", notANumber);
    const Student infinityStudent(2, "Infinity", infinity);

    EXPECT_TRUE(std::isnan(nanStudent.getMarks()));
    EXPECT_FALSE(std::isnan(infinityStudent.getMarks()));
    EXPECT_TRUE(std::isinf(infinityStudent.getMarks()));
}
