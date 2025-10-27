#include "Inventory.hpp"
#include "CSVReader.hpp"
#include <iostream>

static std::string trim(const std::string& s)
 {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) ++a;
    while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

static void splitCategories(const std::string& raw, DynArray<std::string>& out)
 {
    out.clear();
    if (raw.empty()) return;

    std::string cur;
    for (size_t i = 0; i < raw.size(); ++i)
     {
        char c = raw[i];
        if (c == '|') {
            std::string t = trim(cur);
            if (!t.empty()) out.push_back(t);
            cur.clear();
        } else
         {
            cur.push_back(c);
        }
    }
    std::string t = trim(cur);
    if (!t.empty()) out.push_back(t);
}


size_t Inventory::loadCSV(const std::string& path, std::string& err) 
{
    CSVReader reader(path);
    if (!reader.good()) { err = "Failed to open file: " + path; return 0; }

    DynArray<std::string> row;
    if (!reader.readRow(row)) { err = "CSV empty."; return 0; }

    
    int idx_id = -1, idx_name = -1, idx_cat = -1;
    for (size_t i = 0; i < row.size(); ++i) 
    {
        std::string h = row[i];
        for (size_t j = 0; j < h.size(); ++j) h[j] = (char)tolower((unsigned char)h[j]);
        if (h.find("uniq") != std::string::npos) idx_id = (int)i;
        else if (h.find("product") != std::string::npos && h.find("name") != std::string::npos) idx_name = (int)i;
        else if (h.find("category") != std::string::npos) idx_cat = (int)i;
    }
    if (idx_id < 0 || idx_name < 0 || idx_cat < 0) 
    {
        err = "Header missing required columns.";
        return 0;
    }

    size_t count = 0;
    while (reader.readRow(row))
     {
        if ((int)row.size() <= idx_id || (int)row.size() <= idx_name || (int)row.size() <= idx_cat) continue;
        Product p;
        p.id = row[(size_t)idx_id];
        p.name = row[(size_t)idx_name];
        p.category_raw = row[(size_t)idx_cat];
        splitCategories(p.category_raw, p.categories);
        if (!p.id.empty()) 
        {
            addProduct(p);
            ++count;
        }
    }
    return count;
}
