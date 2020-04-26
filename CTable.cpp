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

class CElement
{
public:
    enum ALIGN{ALIGN_LEFT, ALIGN_RIGHT};
    int m_Lines, m_Columns;

    CElement() = default;

    virtual ~CElement(){}

    CElement(const CElement & el)
    {
        m_Lines = el.m_Lines;
        m_Columns = el.m_Columns;
    }

    virtual string GetLine(int index, int lines, int width) const = 0;
};

class CEmpty : public CElement
{
public:
    CEmpty()
    {
        m_Lines = m_Columns = 0;
    }

    ~CEmpty() override = default;

    string GetLine(int index, int lines, int width) const override
    {
        return "";
    }
};

class CText : public CElement
{
public:
    vector<string> m_Text;
    ALIGN m_Align;

    CText(const string &text, ALIGN align)
    {
        m_Align = align;
        CountRowsCols(text);
    }

    CText(const CText &text)
     : CElement(text) {
        m_Lines = text.m_Lines;
        m_Columns = text.m_Columns;
        m_Align = text.m_Align;
        m_Text.clear();
        for (auto &i: text.m_Text)
        {
            m_Text.push_back(i);
        }
    }

    CText &operator =(const CText &text)
    {
        if (this == &text)
            return *this;

        m_Lines = text.m_Lines;
        m_Columns = text.m_Columns;
        m_Align = text.m_Align;
        m_Text.clear();
        for (auto &i: text.m_Text)
        {
            m_Text.push_back(i);
        }
        return *this;
    }

    ~CText() override = default;

    void SetText(const string &text)
    {
        m_Text.clear();
        CountRowsCols(text);
    }

    string GetLine(int index, int lines, int width) const override
    {
        if (index < m_Lines && index >= 0)
        {
            return m_Text[index];
        }
        return "";
    }

private:
    void CountRowsCols(const string& text)
    {
        m_Lines = 1;
        m_Columns = 0;

        int temp_cols = 0;
        string temp_string;

        for (char i: text)
        {
            if (i == '\n') {
                m_Lines++;
                if (temp_cols > m_Columns)
                    m_Columns = temp_cols;

                m_Text.push_back(temp_string);
                temp_string.clear();
                temp_cols = 0;
            }
            else {
                temp_string += i;
                temp_cols++;
            }
        }
        m_Text.push_back(temp_string);
        temp_string.clear();
        if (temp_cols > m_Columns)
            m_Columns = temp_cols;
    }
};

class CImage : public CElement
{
public:
    vector<string> m_Image;

    CImage()
    {
        m_Lines = m_Columns = 0;
    }

    CImage(const CImage &image)
            : CElement(image) {
        m_Lines = image.m_Lines;
        m_Columns = image.m_Columns;
        m_Image.clear();
        for (auto &i: image.m_Image)
        {
            m_Image.push_back(i);
        }
    }

    CImage & operator=(const CImage &image)
    {
        if (this == &image)
            return *this;

        m_Lines = image.m_Lines;
        m_Columns = image.m_Columns;
        m_Image.clear();
        for (auto &i: image.m_Image)
        {
            m_Image.push_back(i);
        }
        return *this;
    }

    ~CImage() override = default;

    string GetLine(int index, int lines, int width) const override
    {
        ostringstream oss;
        if (index < (lines + m_Lines) / 2 && index >= (lines - m_Lines) / 2)
        {
            for (int i = 0; i < (width - m_Columns) / 2; i++)
                oss << ' ';
            oss << m_Image[index - ((lines - m_Lines) / 2)];
        }
        return oss.str();
    }

    CImage AddRow(const string &string)
    {
        m_Image.push_back(string);
        m_Columns = string.length();
        m_Lines++;
        return *this;
    }
};

class CTable : public CElement
{
public:
    CTable(const int rows, const int cols)
    {
        m_Rows = rows;
        m_Cols = cols;
        m_Table = new CElement**[m_Rows];

        for (int i = 0; i < m_Rows; i++)
        {
            m_Table[i] = new CElement*[m_Cols];
            for (int j = 0; j < m_Cols; j++)
            {
                m_Table[i][j] = new CEmpty();
            }
        }
    }

    CTable(const CTable &table)
            : CElement(table)
    {
        m_Rows = table.m_Rows;
        m_Cols = table.m_Cols;
        m_Table = new CElement**[m_Rows];

        for (int i = 0; i < m_Rows; i++)
        {
            m_Table[i] = new CElement*[m_Cols];
            for (int j = 0; j < m_Cols; j++)
            {
                SetCellWithoutDelete(i, j, *table.m_Table[i][j]);
            }
        }
    }

    CTable &operator=(const CTable &table)
    {
        if (this == &table)
            return *this;

        for (int i = 0; i < m_Rows; i++)
        {
            for (int j = 0 ; j < m_Cols; j++)
                delete m_Table[i][j];
            delete [] m_Table[i];
        }
        delete [] m_Table;

        m_Rows = table.m_Rows;
        m_Cols = table.m_Cols;
        m_Table = new CElement**[m_Rows];

        for (int i = 0; i < m_Rows; i++)
        {
            m_Table[i] = new CElement*[m_Cols];
            for (int j = 0; j < m_Cols; j++)
            {
                SetCellWithoutDelete(i, j, *table.m_Table[i][j]);
            }
        }
        return *this;
    }

    ~CTable() override
    {
        for (int i = 0; i < m_Rows; i++)
        {
            for (int j = 0; j < m_Cols; j++)
                delete m_Table[i][j];
            delete[] m_Table[i];
        }
        delete [] m_Table;
    }

    void SetCell(const int row, const int col, const CElement &item)
    {
        if (m_Table[row][col] != &item) {
            delete m_Table[row][col];
            SetCellWithoutDelete(row, col, item);
        }
    }

    void SetCellWithoutDelete(const int row, const int col, const CElement &item)
    {
        if (typeid(item) == typeid(CImage))
        {
            m_Table[row][col] = new CImage(dynamic_cast<CImage &>(const_cast<CElement &>(item)));
        }
        else if (typeid(item) == typeid(CText))
        {
            m_Table[row][col] = new CText(dynamic_cast<CText &>(const_cast<CElement &>(item)));
        }
        else if (typeid(item) == typeid(CEmpty))
        {
            m_Table[row][col] = new CEmpty(dynamic_cast<CEmpty &>(const_cast<CElement &>(item)));
        }
        else if (typeid(item) == typeid(CTable))
        {
            m_Table[row][col] = new CTable(dynamic_cast<CTable &>(const_cast<CElement &>(item)));
        }
    }

    void SetCell(const int row, const int col, const CEmpty &item)
    {
        delete m_Table[row][col];
        m_Table[row][col] = new CEmpty(item);
    }

    void SetCell(const int row, const int col, const CText &item)
    {
        delete m_Table[row][col];
        m_Table[row][col] = new CText(item);
    }

    void SetCell(const int row, const int col, const CImage &item)
    {
        delete m_Table[row][col];
        m_Table[row][col] = new CImage(item);
    }

    void SetCell(const int row, const int col, const CTable &item)
    {
        CElement *temp = m_Table[row][col];
        m_Table[row][col] = new CTable(item);
        delete temp;
    }

    CElement &GetCell(const int row, const int col) const
    {
        return *m_Table[row][col];
    }

    void Update()
    {
        string temp_string = Render();
        string temp;
        m_Content.clear();

        for (auto i : temp_string)
        {
            if (i == '\n')
            {
                m_Content.push_back(temp);
                temp.erase();
            } else{
                temp += i;
            }
        }
        m_Lines = m_Content.size();
        if (m_Lines)
            m_Columns = m_Content[0].length();
    }

    string GetLine(int index, int line, int width) const override
    {
        if (index >= 0 && index < m_Lines)
            return m_Content[index];
        return "";
    }

    friend ostream &operator<< (ostream &os, CTable &table)
    {
        table.Update();
        for (auto &i : table.m_Content)
            os << i << endl;
        return os;
    }

private:
    CElement *** m_Table;
    int m_Rows, m_Cols;
    vector<string> m_Content;

    void FindWidths(int * widths) const
    {
        for (int i = 0; i < m_Cols; i++)
        {
            widths[i] = 0;
            for (int j = 0; j < m_Rows; j++)
            {
                if (typeid(*m_Table[j][i]) == typeid(CTable))
                    ((CTable*) m_Table[j][i])->Update();
                if (widths[i] < m_Table[j][i]->m_Columns)
                {
                    widths[i] = m_Table[j][i]->m_Columns;
                }
            }
        }
    }

    void FindHeights(int * heights) const
    {
        for (int i = 0; i < m_Rows; i++)
        {
            heights[i] = 0;
            for (int j = 0; j < m_Cols; j++)
            {
                if (heights[i] < m_Table[i][j]->m_Lines)
                {
                    heights[i] = m_Table[i][j]->m_Lines;
                }
            }
        }
    }

    string DrawLine(const int * widths) const
    {
        ostringstream oss;

        for (int i = 0; i < m_Cols; i++)
        {
            oss << '+';
            for (int j = 0; j < widths[i]; j++)
            {
                oss << '-';
            }
        }
        oss << '+' << endl;
        return oss.str();
    }

    string DrawContent(const int * widths, const int height, const int index) const
    {
        ostringstream oss;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < m_Cols; j++)
            {
                oss << '|' << setw(widths[j])
                    << (typeid(*m_Table[index][j]) == typeid(CText) ?
                        dynamic_cast<CText *>(m_Table[index][j])->m_Align == CText::ALIGN_LEFT ? left : right : left)
                    << m_Table[index][j]->GetLine(i, height, widths[j]);
            }
            oss << '|' << endl;
        }
        return oss.str();
    }

    string Render() const
    {
        ostringstream oss, line;
        int *widths = new int[m_Cols];
        int *heights = new int[m_Rows];

        FindWidths(widths);
        FindHeights(heights);

        line << DrawLine(widths);
        for (int i = 0; i < m_Rows; i++) {
            oss << line.str();
            oss << DrawContent(widths, heights[i], i);
        }
        oss << line.str();

        delete [] widths;
        delete [] heights;
        return oss.str();
    }
};
