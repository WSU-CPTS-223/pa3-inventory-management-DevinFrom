
/*******************************************************************************************
* Programmer: Devin From
* Class: CptS 223, Fall 2025
* Programming Assignment: PA3
* Date: October 26, 2025
* Description: This program Will allow users to sort and find products that are on Amazon's
* catalog
*******************************************************************************************/
#include <iostream>
#include <string>
#include <sstream>
#include "Inventory.hpp"

static void printHelp() 
{
    std::cout << "Commands:\n"
              << "  find \n"
              << "  listInventory \n"
              << "  help\n"
              << "  exit\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_csv>\n";
        return 1;
    }
    Inventory inv;
    std::string err;
    size_t loaded = inv.loadCSV(argv[1], err);
    if (loaded == 0 && !err.empty()) {
        std::cerr << "Error : " << err << "\n";
        return 1;
    }
    std::cout << "Loaded " << loaded << " products.\n";
    std::cout << "Type 'help' for commands.\n";

    std::string line;
    while (true) 
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        std::istringstream iss(line);
        std::string cmd;
        if (!(iss >> cmd)) continue;

        if (cmd == "find")
         {
            std::string id; iss >> id;
            if (id.empty()) { std::cout << "Usage: find <inventoryid>\n"; continue; }
            const Product* p = inv.findById(id);
            if (!p) 
            {
                std::cout << "Product not found\n";
            } else
             {
                std::cout << "Uniq Id: " << p->id << "\n"
                          << "Product Name: " << p->name << "\n"
                          << "Category: " << (p->category_raw.empty() ? "NA" : p->category_raw) << "\n";
            }
        } else if (cmd == "listInventory") {
            std::string cat;
            std::getline(iss, cat);
            if (!cat.empty() && cat[0] == ' ') cat.erase(0,1);
            if (cat.empty()) { std::cout << "Usage: listInventory \n"; continue; }

            DynArray<int> indices;
            if (!inv.byCategory(cat, indices))
             {
                std::cout << "Invalid Category\n";
            } else {
                for (size_t i = 0; i < indices.size(); ++i) 
                {
                    const Product& p = inv.productAt((size_t)indices[i]);
                    std::cout << p.id << " | " << p.name << "\n";
                }
            }
        } else if (cmd == "help") 
        {
            printHelp();
        } else if (cmd == "quit" || cmd == "exit")
         {
            break;
        } else 
        {
            std::cout << "Unknown command\n";
        }
    }
    return 0;
}
