#include <cassert>
#include <string>
#include "DynArray.hpp"
#include "HashMap.hpp"


void testDynArray()
 {
    DynArray<int> a;
    a.push_back(1);
    a.push_back(2);
    assert(a.size() == 2);

    assert(a[0] == 1 && a[1] == 2);

    
    for (int i = 0; i < 100; ++i) a.push_back(i);
    assert(a.size() == 102);
    
    assert(a[50] == 48);
}

void testHashMap()
 {
    HashMap<std::string, int> m;
    m.insert("a", 1);
    
    m.insert("b", 2);
    m.insert("c", 3);

    assert(m.size() == 3);
    assert(*m.find("a") == 1);
    assert(m.contains("b"));
    assert(!m.contains("z"));

    m.insert("a", 42); 
    assert(*m.find("a") == 42);

    
    for (int i = 0; i < 1000; ++i) m.insert("k"+std::to_string(i), i);
    assert(m.contains("k123"));
    assert(*m.find("k999") == 999);
}

int main()
 {
    testDynArray();
    testHashMap();
    return 0;
}
