#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "matrix2.h"

TEST(Matrix, Common)
{
    Matrix<int, 3> matrix(0);
    int num = 10;

    auto k = matrix[1][2][0];
    EXPECT_EQ(k, 0);

    matrix[1][2][0] = num;
    EXPECT_EQ(matrix[1][2][0], num);

    matrix[1][2][2] = 5;
    EXPECT_EQ(matrix[1][2][2], 5);
    EXPECT_EQ(matrix.size(), 2);

    matrix[1][2][2] = 0;
    EXPECT_EQ(matrix.size(), 1);
}

TEST(Matrix, ComplexType) {
    std::string default_str = "default";
    Matrix<std::string, 3> matrix(default_str);

    std::string actual_str = "actual";

    std::string as_default_str = matrix[1][2][0];
    EXPECT_EQ(as_default_str, default_str);

    matrix[1][2][0] = actual_str;
    EXPECT_EQ((std::string)matrix[1][2][0], actual_str);

    matrix[1][2][2] = "actual1";
    EXPECT_EQ((std::string)matrix[1][2][2], "actual1");
    EXPECT_EQ(matrix.size(), 2);

    matrix[1][2][2] = default_str;
    EXPECT_EQ(matrix.size(), 1);
}