#ifndef MATRIX_H
#define MATRIX_H

#include "BufferMatrix.hpp"
#include <stdexcept>

template <typename T>
class Matrix : private BufferMatrix<T>
{
private:
    class RowProxy {
    private:
        T* data;
        int maxRow;
    public:
        RowProxy(T* rowData, int rowCount) : data(rowData), maxRow(rowCount) {}

        T& operator[](int row) {
            if (row < 0 || row >= maxRow)
                throw std::out_of_range("Invalid row index");
            return data[row];
        }
    };

    class ConstRowProxy 
    {
    private:
        const T* data;
        int maxRow;
    
    public:
        ConstRowProxy(const T* rowData, int rowCount) : data(rowData), maxRow(rowCount) {}

        const T& operator[](int row) const {
            if (row < 0 || row >= maxRow)
                throw std::out_of_range("Invalid row index");
            return data[row];
        }
    };
    
public:
    Matrix() : BufferMatrix<T>() {}

    Matrix(int r, int c) : BufferMatrix<T>(r, c) {}

    Matrix(const Matrix& other) : BufferMatrix<T>(other) {}

    Matrix(Matrix&& other) noexcept : BufferMatrix<T>(std::move(other)) {}

    ~Matrix() = default;

    Matrix& operator=(const Matrix& other)
    {
        if (this != &other)
        {
            BufferMatrix<T>::operator=(other);
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept
    {
        if (this != &other)
        {
            BufferMatrix<T>::operator=(std::move(other));
        }
        return *this;
    }

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

    void Resize(int newRows, int newCols)
    {
        if (newRows < 0 || newCols < 0)
            throw std::out_of_range("Negative size");

        T** NewBuffer = new T*[newCols];
        int i = 0;
        try {
            for (; i < newCols; ++i)
            {
                NewBuffer[i] = new T[newRows];
                for (int j = 0; j < newRows; ++j)
                {
                    if (i < this->cols && j < this->rows)
                        NewBuffer[i][j] = this->buffer[i][j];
                    else
                        NewBuffer[i][j] = T();
                }
            }
        } catch (...) {
            for (int k = 0; k < i; ++k)
                delete[] NewBuffer[k];
            delete[] NewBuffer;
            throw;
        }

        Clear();
        this->buffer = NewBuffer;
        this->cols = newCols;
        this->rows = newRows;
    }

    void AddColumn(const T* colData)
    {
        T** NewBuffer = new T*[this->cols + 1];
        int i = 0;
        try {
            for (; i < this->cols; ++i)
            {
                NewBuffer[i] = new T[this->rows];
                for (int j = 0; j < this->rows; ++j)
                    NewBuffer[i][j] = this->buffer[i][j];
            }

            NewBuffer[this->cols] = new T[this->rows];
            for (int j = 0; j < this->rows; ++j)
                NewBuffer[this->cols][j] = colData[j];
        } catch (...) {
            for (int k = 0; k < i; ++k)
                delete[] NewBuffer[k];
            delete[] NewBuffer;
            throw;
        }

        Clear();
        this->buffer = NewBuffer;
        ++this->cols;
    }

    void RemoveColumn(int colIndex)
    {
        if (colIndex < 0 || colIndex >= this->cols)
            throw std::out_of_range("Index out of range");

        T** NewBuffer = new T*[this->cols - 1];
        int ni = 0;
        int i = 0;
        try {
            for (; i < this->cols; ++i)
            {
                if (i == colIndex) continue;

                NewBuffer[ni] = new T[this->rows];
                for (int j = 0; j < this->rows; ++j)
                    NewBuffer[ni][j] = this->buffer[i][j];
                ++ni;
            }
        } catch (...) {
            for (int k = 0; k < ni; ++k)
                delete[] NewBuffer[k];
            delete[] NewBuffer;
            throw;
        }

        Clear();
        this->buffer = NewBuffer;
        --this->cols;
    }

    Matrix operator+(const Matrix& other) const
    {
        if (this->rows != other.rows || this->cols != other.cols)
            throw std::invalid_argument("Matrix sizes must be equal");

        Matrix result(this->rows, this->cols);
        for (int col = 0; col < this->cols; ++col)
        {
            for (int row = 0; row < this->rows; ++row)
            {
                result.buffer[col][row] = this->buffer[col][row] + other.buffer[col][row];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const
    {
        if (this->rows != other.rows || this->cols != other.cols)
            throw std::invalid_argument("Matrix sizes must be equal");

        Matrix result(this->rows, this->cols);
        for (int col = 0; col < this->cols; ++col)
        {
            for (int row = 0; row < this->rows; ++row)
            {
                result.buffer[col][row] = this->buffer[col][row] - other.buffer[col][row];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const
    {
        if (this->cols != other.rows)
            throw std::invalid_argument("Matrix sizes not suitable for multiplication");

        Matrix result(this->rows, other.cols);

        for (int col = 0; col < other.cols; ++col)
        {
            for (int row = 0; row < this->rows; ++row)
            {
                result.buffer[col][row] = T();
                for (int k = 0; k < this->cols; ++k)
                {
                    result.buffer[col][row] += this->buffer[k][row] * other.buffer[col][k];
                }
            }
        }
        return result;
    }

    Matrix operator*(const T& scalar) const
    {
        Matrix result(this->rows, this->cols);
        for (int col = 0; col < this->cols; ++col)
        {
            for (int row = 0; row < this->rows; ++row)
            {
                result.buffer[col][row] = this->buffer[col][row] * scalar;
            }
        }
        return result;
    }

    RowProxy operator[](int col)
    {
        if (col < 0 || col >= this->cols)
            throw std::out_of_range("Invalid column index");
        return RowProxy(this->buffer[col], this->rows);
    }

    ConstRowProxy operator[](int col) const
    {
        if (col < 0 || col >= this->cols)
            throw std::out_of_range("Invalid column index");
        return ConstRowProxy(this->buffer[col], this->rows);
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
        for (int col = 0; col < this->cols; ++col)
        {
            for (int row = 0; row < this->rows; ++row)
            {
                this->buffer[col][row] = value;
            }
        }
    }
};

#endif
