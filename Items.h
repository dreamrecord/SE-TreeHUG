#ifndef ITEMS_H
#define ITEMS_H

#include <iostream>
#include <string>
#include <map>

class Items{
    
    std::map<std::string, int> items;
    int                        transacUtil;
    friend class               Dataset;

public:
    Items();
    void printItems();
};
#endif
