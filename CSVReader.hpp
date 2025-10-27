#ifndef CSVREADER_HPP
#define CSVREADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include "DynArray.hpp"


class CSVReader {
public:
    explicit CSVReader(const std::string& path) : in(path.c_str()), good_(false)
     {
        good_ = in.good();
    }

    bool good() const { return good_; }
    bool eof() const { return in.eof(); }

   
    bool readRow(DynArray<std::string>& out) 
    {
        out.clear();
        if (!in.good()) return false;
        std::string line;
        if (!std::getline(in, line)) return false;
        parseCSVLine(line, out);
        return true;
    }

private:
    static void parseCSVLine(const std::string& line, DynArray<std::string>& out) 
    {

        std::string cur;
        bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '\"') {
                if (inQuotes && i + 1 < line.size() && line[i + 1] == '\"')
                 {
                    cur.push_back('\"'); ++i; 
                } 
                else
                 {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes)
             {
                out.push_back(cur);
                cur.clear();
            } else
             {
                cur.push_back(c);
            }
        }
        out.push_back(cur);
    }

    std::ifstream in;
    bool good_;
};

#endif 
