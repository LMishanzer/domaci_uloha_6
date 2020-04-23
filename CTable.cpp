#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CEmpty
{
public:
    CEmpty()
    {
        m_Rows = m_Cols = 0;
    }

    friend ostream &operator<<(ostream &os, const CEmpty &empty)
    {
        os << empty.m_Rows << ", " << empty.m_Cols;
        return os;
    }

protected:
    int m_Rows, m_Cols;
};

class CText : public CEmpty
{
public:
    enum ALIGN{ALIGN_LEFT, ALIGN_RIGHT};

    CText(string  text, const ALIGN align)
    : m_Text(std::move(text)), m_Align(align)
    {
        CountRowsCols();
    }

    void SetText(string &text)
    {
        m_Text = text;
        CountRowsCols();
    }

    friend ostream &operator<<(ostream &os, const CText &text)
    {
        os << text.m_Rows << ", " << text.m_Cols << ": " << text.m_Text;
        return os;
    }

private:
    string m_Text;
    ALIGN m_Align;

    void CountRowsCols()
    {
        m_Rows = 1;
        m_Cols = 0;

        int temp_cols = 0;

        for (char i: m_Text)
        {
            if (i == '\n') {
                m_Rows++;
                if (temp_cols > m_Cols)
                    m_Cols = temp_cols;

                temp_cols = 0;
            }
            temp_cols++;
        }
        if (temp_cols > m_Cols)
            m_Cols = temp_cols;
    }
};

class CImage : public CEmpty
{
public:
    CImage()
    = default;

    CImage &AddRow(const string &str)
    {
        if (!m_Rows)
            m_Cols = str.length();
        m_Image.push_back(str);
        m_Rows++;
        return *this;
    }

    friend ostream &operator<<(ostream &os, const CImage &image)
    {
        os << image.m_Rows << ", " << image.m_Cols << ": \n";
        for (long unsigned int i = 0; i < image.m_Image.size() - 1; i++)
        {
            cout << image.m_Image[i] << '\n';
        }
        cout << image.m_Image[image.m_Image.size() - 1];
        return os;
    }

private:
    vector<string> m_Image;

};

class CTable
{
public:
    CTable(const int rows, const int cols)
    {
        m_Rows = rows;
        m_Cols = cols;
        m_Table.resize(m_Rows);
        for (auto &i: m_Table)
        {
            for (int j = 0; j < m_Cols; j++)
            {
                i.emplace_back();
            }
        }
    }

    CEmpty &GetCell(const int row, const int col)
    {
        return *m_Table[row][col];
    }

    void SetCell(const int row, const int col, CEmpty & item)
    {
        m_Table[row][col] = (CText *) &m_Empty;
    }

    void SetCell(const int row, const int col, CText & item)
    {

    }

    void SetCell(const int row, const int col, CImage & item)
    {

    }

    friend ostream& operator<<(ostream &os, const CTable &table)
    {

        return os;
    }

private:
    int m_Rows, m_Cols;
    CEmpty m_Empty;
    set<CText> m_Texts;
    set<CImage> m_Images;
    vector<vector<CText *>> m_Table;
};
