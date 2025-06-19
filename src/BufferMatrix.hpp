#ifndef BUFFERMATRIX_H
#define BUFFERMATRIX_H

template <typename T>
class BufferMatrix
{
protected:
    T** buffer;
    int rows, cols;

public:
    BufferMatrix() : buffer(nullptr), rows(0), cols(0) {}

    BufferMatrix(int r, int c) : rows(r), cols(c)
    {
        buffer = new T*[cols];
        for (int i = 0; i < cols; ++i)
        {
            buffer[i] = new T[rows];
        }
    }

    BufferMatrix(const BufferMatrix& other) : rows(other.rows), cols(other.cols)
    {
        buffer = new T*[cols];
        for (int i = 0; i < cols; ++i)
        {
            buffer[i] = new T[rows];
            for (int j = 0; j < rows; ++j)
                buffer[i][j] = other.buffer[i][j];
        }
    }

    BufferMatrix(BufferMatrix&& other) noexcept
        : buffer(other.buffer), rows(other.rows), cols(other.cols)
    {
        other.buffer = nullptr;
        other.rows = 0;
        other.cols = 0;
    }

    BufferMatrix& operator=(const BufferMatrix& other)
    {
        if (this != &other)
        {
            if (buffer)
            {
                for (int i = 0; i < cols; ++i)
                    delete[] buffer[i];
                delete[] buffer;
            }

            rows = other.rows;
            cols = other.cols;
            buffer = new T*[cols];
            for (int i = 0; i < cols; ++i)
            {
                buffer[i] = new T[rows];
                for (int j = 0; j < rows; ++j)
                    buffer[i][j] = other.buffer[i][j];
            }
        }
        return *this;
    }


    BufferMatrix& operator=(BufferMatrix&& other) noexcept
    {
        if (this != &other)
        {
            if (buffer)
            {
                for (int i = 0; i < cols; ++i)
                    delete[] buffer[i];
                delete[] buffer;
            }

            buffer = other.buffer;
            rows = other.rows;
            cols = other.cols;

            other.buffer = nullptr;
            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }


    ~BufferMatrix()
    {
        if (buffer)
        {
            for (int i = 0; i < cols; ++i)
            {
                delete[] buffer[i];
            }
            delete[] buffer;
        }
    }

};


#endif
