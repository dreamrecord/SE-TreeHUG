#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <string>
#include "Items.h"

class Items;

class Dataset{
    
    std::vector<Items*> headList;
    int                 numOfHeads;

public:
    Dataset();
    void addTransacUtil(int transac, int RHSTransacUtil);
    void addItem(int transac, std::string item, int util);
    bool existInTransac(std::string itemset, int transac);
    int  retNumOfTransac();
    int  retTransacUtil(int transac);
    int  retItemUtilInTransac(std::string item, int transac);
    void printDataset();
};
#endif
