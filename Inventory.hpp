#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "DynArray.hpp"
#include "HashMap.hpp"
#include "Product.hpp"
#include <string>
#include <cctype>

class Inventory 
{
public:
    Inventory() : idIndex(), categoryIndex() 
    {}

    void addProduct(const Product& p) {
        products.push_back(p);
        idIndex.insert(p.id, (int)(products.size() - 1));
        if (p.categories.size() == 0) {
            addToCategory("NA", (int)(products.size() - 1));
        } else 
        {
            for (size_t i = 0; i < p.categories.size(); ++i)
             {
                addToCategory(p.categories[i], (int)(products.size() - 1));
            }
        }
    }

    const Product* findById(const std::string& id) const 
    {
        const int* idx = idIndex.find(id);
        if (!idx) return nullptr;
        return &products[(size_t)*idx];
    }

    bool byCategory(const std::string& categoryQuery, DynArray<int>& out) const
     {
        const DynArray<int>* arr = categoryIndex.find(normalize(categoryQuery));
        if (!arr) return false;
        out = *arr; // copy
        return true;
    }

    size_t productCount() const { return products.size(); }
    const Product& productAt(size_t i) const { return products[i]; }

    
    size_t loadCSV(const std::string& path, std::string& err);

private:
    DynArray<Product> products;
    HashMap<std::string, int> idIndex;
    HashMap<std::string, DynArray<int> > categoryIndex;

    static std::string normalize(const std::string& s) 
    {
        std::string r = s;
        for (size_t i = 0; i < r.size(); ++i) r[i] = (char)std::tolower((unsigned char)r[i]);
        return r;
    }

    void addToCategory(const std::string& cat, int prodIndex) 
    {
        std::string key = normalize(cat);
        DynArray<int>* vec = categoryIndex.find(key);
        if (!vec) {
            DynArray<int> nv;
            nv.push_back(prodIndex);
            categoryIndex.insert(key, nv);
        } else {
            vec->push_back(prodIndex);
        }
    }
};

#endif 
