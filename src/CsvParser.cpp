#include "CsvParser.h"
using namespace std;

// CsvParser.
CsvWriter::CsvWriter()
{
    m_isopen = false;
    m_delimiter = ", ";
}
CsvWriter::CsvWriter(string filename)
{
    m_file.open(filename.c_str());
    m_isopen = m_file.is_open();
    m_delimiter = ", ";
}
CsvWriter::~CsvWriter()
{
    m_file.close();
}

// File operations.
void CsvWriter::Open(string filename)
{
    m_file.open(filename.c_str());
    m_isopen = m_file.is_open();
}
void CsvWriter::Close()
{
    m_file.close();
}
bool CsvWriter::IsOpen()
{
    return m_isopen;
}

// Csv operations.
void CsvWriter::SetDelimiter(string delimiter)
{
    m_delimiter = delimiter;
}
void CsvWriter::Push(std::vector<string> data)
{
    for(unsigned int i=0; i<data.size(); i++)
    {
        if(i != data.size()-1)
            m_file << data[i] << m_delimiter;
        else
            m_file << data[i];
    }
    m_file << endl;
}
void CsvWriter::Push(string d1, string d2, string d3, string d4)
{
    if(d1 != "") m_file << d1 << m_delimiter;
    if(d2 != "") m_file << d2 << m_delimiter;
    if(d3 != "") m_file << d3 << m_delimiter;
    if(d4 != "") m_file << d4;
    m_file << endl;
}


