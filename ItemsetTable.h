#ifndef ITEMSETTABLE_H
#define ITEMSETTABLE_H

#include <iostream>
#include <string>
#include <vector>

class Itemset;
class Dataset;

class ItemsetTable{
    
    std::vector<Itemset*> headList;
    int                   numOfHeads;
    friend class          Itemset;

public:
    ItemsetTable(int RHSNumOfHeads);
    void     addItemset(std::string RHSItemset);
    Itemset* delItemset(std::string RHSItemset);
    bool     subsetExist(std::string itemset, std::vector<int>& subset, int lenOfSubset);
    bool     allSubsetExist(std::string itemset, int lenOfSubset);
    void     genNextLayer(int layer);
    void     genHUG(int minUtil, Dataset& dataset);
    void     printItemsetTable();
};
#endif
