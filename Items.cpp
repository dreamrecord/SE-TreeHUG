#include "Items.h"

/* Items functions implementation */
Items::Items(){
    
    transacUtil = 0;
}

void Items::printItems(){
    
    std::cout << transacUtil << " | ";

    for(std::map<std::string, int>::iterator It = items.begin() ;
        It != items.end() ;
        It++){

        std::cout << It->first << "," << It->second << " ";
    }

    std::cout << std::endl;
}
