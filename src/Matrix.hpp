#ifndef MATRIX_H
#define MATRIX_H

#include "BufferMatrix.hpp"
#include <stdexcept>
#include <utility> 

template <typename T>
class Matrix : private BufferMatrix<T>
{
public:
    Matrix() : BufferMatrix<T>() {}

    Matrix(int r, int c) : BufferMatrix<T>(r, c) {}

    Matrix(const Matrix& other) : BufferMatrix<T>(other) {}

    Matrix(Matrix&& other) noexcept : BufferMatrix<T>(std::move(other)) {}

    ~Matrix() = default;

    void Set(int row, int col, const T& value)
    {
        if (row < 0 || row >= this->rows || col < 0 || col >= this->cols)
            throw std::out_of_range("Index out of range");
        this->buffer[col][row] = value;
    }

    T Get(int row, int col) const
    {
        if (row < 0 || row >= this->rows || col < 0 || col >= this->cols)
            throw std::out_of_range("Index out of range");
        return this->buffer[col][row];
    }

    int RowCount() const { return this->rows; }
    int ColCount() const { return this->cols; }

    void Clear()
    {
        for (int i = 0; i < this->cols; ++i)
            delete[] this->buffer[i];
        delete[] this->buffer;
        this->buffer = nullptr;
        this->rows = 0;
        this->cols = 0;
    }

    Matrix operator+(const Matrix& other) const
    {
        if (this->rows != other.rows || this->cols != other.cols)
            throw std::invalid_argument("Matrix sizes must be equal");

        Matrix result(this->rows, this->cols);
        for (int i = 0; i < this->cols; ++i)
        {
            for (int j = 0; j < this->rows; ++j)
            {
                result.buffer[i][j] = this->buffer[i][j] + other.buffer[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const
    {
        if (this->rows != other.rows || this->cols != other.cols)
            throw std::invalid_argument("Matrix sizes must be equal");

        Matrix result(this->rows, this->cols);
        for (int i = 0; i < this->cols; ++i)
        {
            for (int j = 0; j < this->rows; ++j)
            {
                result.buffer[i][j] = this->buffer[i][j] - other.buffer[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const
    {
        if (this->cols != other.rows)
            throw std::invalid_argument("Matrix sizes not suitable for multiplication");

        Matrix result(this->rows, other.cols);
        for (int i = 0; i < other.cols; ++i)
        {
            for (int j = 0; j < this->rows; ++j)
            {
                result.buffer[i][j] = T();
                for (int k = 0; k < this->cols; ++k)
                {
                    result.buffer[i][j] += this->buffer[k][j] * other.buffer[i][k];
                }
            }
        }
        return result;
    }

    Matrix operator*(const T& scalar) const
    {
        Matrix result(this->rows, this->cols);
        for (int i = 0; i < this->cols; ++i)
        {
            for (int j = 0; j < this->rows; ++j)
            {
                result.buffer[i][j] = this->buffer[i][j] * scalar;
            }
        }
        return result;
    }

    Matrix& operator=(const Matrix& other)
    {
        if (this != &other)
        {
            Matrix temp(other); 
            std::swap(this->rows, temp.rows);
            std::swap(this->cols, temp.cols);
            std::swap(this->buffer, temp.buffer);
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            this->buffer = other.buffer;
            this->cols = other.cols;
            this->rows = other.rows;
            other.buffer = nullptr;
            other.cols = 0;
            other.rows = 0;
        }
        return *this;
    }

    bool operator==(const Matrix& other) const
    {
        if(this->rows != other.rows || this->cols != other.cols)
            return false;
        for(int i = 0; i < this->cols; ++i)
        {
            for(int j = 0; j < this->rows; ++j)
            {
                if(this->buffer[i][j] != other.buffer[i][j])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const
    {
        if(this->rows != other.rows || this->cols != other.cols)
            return true;
        for(int i = 0; i < this->cols; ++i)
        {
            for(int j = 0; j < this->rows; ++j)
            {
                if (this->buffer[i][j] != other.buffer[i][j])
                    return true;
            }
        }
        return false;
    }

    bool IsEmpty() const
    {
        return this->cols == 0 || this->rows == 0 || this->buffer == nullptr;
    }

    bool IsSquare() const
    {
        return this->cols == this->rows;
    }

    void Fill(const T& value)
    {
        for (int i = 0; i < this->cols; ++i)
        {
            for (int j = 0; j < this->rows; ++j)
            {
                this->buffer[i][j] = value;
            }
        }
    }
};

#endif
