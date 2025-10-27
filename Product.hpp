#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>
#include "DynArray.hpp"

struct Product
 {
    std::string id;            
    std::string name;          
    std::string category_raw; 
    DynArray<std::string> categories; 
};

#endif 
