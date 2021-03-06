/**
* @file Types.h
* @brief Data types used on QBill.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez Martinez
* @date 2/18/2014
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#pragma once
#ifndef _TYPES

#include <vector>

typedef char uint8;
typedef short unsigned int uint16;
typedef unsigned int uint32;
typedef int int32;

#ifndef NULL
#define NULL 0
#endif

/**
* @class Coord
* @brief Coordinate storage.
**/
template <class T> class Coord
{
public:
    T m_x, m_y;
    Coord();
    Coord(T x, T y);
    Coord& operator=(const Coord& other);
    bool operator==(const Coord& other);
    bool operator!=(const Coord& other);
};

template <class T> Coord<T>::Coord()
{
    m_x = 0;
    m_y = 0;
}
template <class T> Coord<T>::Coord(T x, T y)
{
    m_x = x;
    m_y = y;
}
template <class T> Coord<T>& Coord<T>::operator=(const Coord<T>& other)
{
    m_x = other.m_x;
    m_y = other.m_y;
    return *this;
}
template <class T> bool Coord<T>::operator==(const Coord<T>& other)
{
    if(other.m_x == m_x && other.m_y == m_y) return true;
    else return false;
}
template <class T> bool Coord<T>::operator!=(const Coord<T>& other)
{
    if(other.m_x == m_x && other.m_y == m_y) return false;
    else return true;
}


/**
* @class Matrix
* @brief Basic matrix storage.
**/
template <class T> class Matrix
{
    T** m_matrix;
    unsigned int m_ncols;
    unsigned int m_nrows;

    T CalcDeterminant(T **inM, unsigned int size);
    void FreeMem();
public:

    ///@brief Default constructor.
    Matrix();

    ///@brief Copy constructor.
    Matrix(const Matrix<T> &other);

    ///@brief Creates empty matrix.
    Matrix(unsigned int rows, unsigned int cols);

    ///@brief Copy matrix from raw data.
    Matrix(unsigned int rows, unsigned int cols, T** input);

    ~Matrix();

    // ## Set values. ##

    ///@brief Creates empty matrix.
    bool SetMatrix(unsigned int rows, unsigned int cols);

    ///@brief Copy matrix from raw data.
    bool SetMatrix(unsigned int rows, unsigned int cols, T** input);

    ///@brief Copy matrix.
    bool SetMatrix(Matrix<T> *input);

    ///@brief Safely set element.
    void SetElement(unsigned int i, unsigned int j, T val);


    // ## Get values. ##
    T** GetMatrix();
    std::vector<T> GetRow(unsigned int row);
    unsigned int GetNumCols();
    unsigned int GetNumRows();

    ///@brief Safely access element.
    T At(unsigned int i, unsigned int j);
    T*& operator[](const unsigned int pos);
};

template <class T> Matrix<T>::Matrix()
{
    m_ncols = 0;
    m_nrows = 0;
    m_matrix = NULL;
}
template <class T> Matrix<T>::Matrix(const Matrix<T> &other)
{
    m_matrix = NULL;
    this->SetMatrix(other.m_nrows, other.m_ncols, other.m_matrix);
}
template <class T> Matrix<T>::Matrix(unsigned int rows, unsigned int cols, T** inputMatrix)
{
    m_matrix = NULL;
    m_ncols = 0;
    m_nrows = 0;
    this->SetMatrix(rows, cols, inputMatrix);
}
template <class T> Matrix<T>::Matrix(unsigned int rows, unsigned int cols)
{
    m_matrix = NULL;
    m_ncols = 0;
    m_nrows = 0;
    this->SetMatrix(rows, cols);
}
template <class T> Matrix<T>::~Matrix()
{
    this->FreeMem();
}
template <class T> void Matrix<T>::FreeMem()
{
    if(m_matrix != NULL)
    {
        for(unsigned int i=0; i<m_nrows; i++)
        {
            if(m_matrix[i] != NULL) delete[] m_matrix[i];
        }
        delete[] m_matrix;
    }
}
template <class T> bool Matrix<T>::SetMatrix(Matrix<T> *matrix)
{
    return this->SetMatrix(matrix->GetNumRows(), matrix->GetNumCols(), matrix->GetMatrix());
}
template <class T> bool Matrix<T>::SetMatrix(unsigned int rows, unsigned int cols, T** inputMatrix)
{
    this->FreeMem();
    m_nrows = rows;
    m_ncols = cols;
    try
    {
        m_matrix = new T*[m_nrows];
        for(unsigned int i=0; i<m_nrows; i++)
        {
            m_matrix[i] = new T[m_ncols];
        }
    }
    catch(std::bad_alloc& ba)
    {
        return false;
    }

    // Assign values.
    if(inputMatrix != NULL)
    {
        for(unsigned int i=0; i<m_nrows; i++)
        {
            for(unsigned int j=0; j<m_ncols; j++)
            {
                m_matrix[i][j] = inputMatrix[i][j];
            }
        }
    }
    else
    {
        for(unsigned int i=0; i<m_nrows; i++)
        {
            for(unsigned int j=0; j<m_ncols; j++)
            {
                m_matrix[i][j] = 0;
            }
        }
    }
    return true;
}
template <class T> bool Matrix<T>::SetMatrix(unsigned int rows, unsigned int cols)
{
    this->FreeMem();
    m_nrows = rows;
    m_ncols = cols;
    try
    {
        m_matrix = new T*[m_nrows];
        for(unsigned int i=0; i<m_nrows; i++)
        {
            m_matrix[i] = new T[m_ncols];
        }
    }
    catch(std::bad_alloc& ba)
    {
        return false;
    }
    return true;
}
template <class T> T** Matrix<T>::GetMatrix()
{
    return m_matrix;
}
template <class T> void Matrix<T>::SetElement(unsigned int i, unsigned int j, T val)
{
    if((i<m_nrows && i>=0) && (j<m_ncols && j>=0))
    {
        m_matrix[i][j] = val;
    }
}
template <class T> T Matrix<T>::At(unsigned int i, unsigned int j)
{
    if((i<m_nrows && i>=0) && (j<m_ncols && j>=0))
    {
        return m_matrix[i][j];
    }
    else return 0;
}
template <class T> std::vector<T> Matrix<T>::GetRow(unsigned int fil)
{
    std::vector<T> out;
    for(int j=0; j<m_ncols; j++)
    {
        out.push_back(m_matrix[fil][j]);
    }
    return out;
}
template <class T> unsigned int Matrix<T>::GetNumCols()
{
    return m_ncols;
}
template <class T> unsigned int Matrix<T>::GetNumRows()
{
    return m_nrows;
}

// Operators.
template <class T> T*& Matrix<T>::operator[](const unsigned int pos)
{
    return m_matrix[pos];
}

#endif
