#include <gtest/gtest.h>
#include "Matrix.hpp"

TEST(MatrixTest, DefaultConstructorCreatesEmptyMatrix)
{
    Matrix<int> m;
    EXPECT_TRUE(m.IsEmpty());
    EXPECT_EQ(m.RowCount(), 0);
    EXPECT_EQ(m.ColCount(), 0);
}

TEST(MatrixTest, ParamConstructorCreatesCorrectSize)
{
    Matrix<int> m(3, 4);
    EXPECT_EQ(m.RowCount(), 3);
    EXPECT_EQ(m.ColCount(), 4);
}

TEST(MatrixTest, SetAndGetWorkCorrectly)
{
    Matrix<int> m(2, 2);
    m.Set(0, 0, 42);
    m.Set(1, 1, 99);
    EXPECT_EQ(m.Get(0, 0), 42);
    EXPECT_EQ(m.Get(1, 1), 99);
}

TEST(MatrixTest, CopyConstructorCopiesCorrectly)
{
    Matrix<int> m1(2, 2);
    m1.Set(0, 0, 5);
    Matrix<int> m2 = m1;
    EXPECT_EQ(m2.Get(0, 0), 5);
}

TEST(MatrixTest, AssignmentOperatorCopiesCorrectly)
{
    Matrix<int> m1(2, 2);
    m1.Set(0, 1, 7);
    Matrix<int> m2;
    m2 = m1;
    EXPECT_EQ(m2.Get(0, 1), 7);
}

TEST(MatrixTest, AddMatrices)
{
    Matrix<int> a(2, 2);
    Matrix<int> b(2, 2);

    a.Set(0, 0, 1); a.Set(0, 1, 2);
    a.Set(1, 0, 3); a.Set(1, 1, 4);

    b.Set(0, 0, 10); b.Set(0, 1, 20);
    b.Set(1, 0, 30); b.Set(1, 1, 40);

    Matrix<int> c = a + b;

    EXPECT_EQ(c.Get(0, 0), 11);
    EXPECT_EQ(c.Get(0, 1), 22);
    EXPECT_EQ(c.Get(1, 0), 33);
    EXPECT_EQ(c.Get(1, 1), 44);
}

TEST(MatrixTest, SubMatrices)
{
    Matrix<int> a(2, 2);
    Matrix<int> b(2, 2);

    a.Set(0, 0, 15); a.Set(0, 1, 25);
    a.Set(1, 0, 35); a.Set(1, 1, 45);

    b.Set(0, 0, 5); b.Set(0, 1, 10);
    b.Set(1, 0, 20); b.Set(1, 1, 30);

    Matrix<int> c = a - b;

    EXPECT_EQ(c.Get(0, 0), 10);
    EXPECT_EQ(c.Get(0, 1), 15);
    EXPECT_EQ(c.Get(1, 0), 15);
    EXPECT_EQ(c.Get(1, 1), 15);
}

TEST(MatrixTest, MultiplyMatrixWithScalar)
{
    Matrix<int> a(2, 2);
    a.Set(0, 0, 1); a.Set(0, 1, 2);
    a.Set(1, 0, 3); a.Set(1, 1, 4);

    Matrix<int> b = a * 2;

    EXPECT_EQ(b.Get(0, 0), 2);
    EXPECT_EQ(b.Get(0, 1), 4);
    EXPECT_EQ(b.Get(1, 0), 6);
    EXPECT_EQ(b.Get(1, 1), 8);
}
