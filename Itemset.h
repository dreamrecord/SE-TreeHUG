#ifndef ITEMSET_H
#define ITEMSET_H

#include <iostream>
#include <string>
#include <map>
#include <set>

class Dataset;
class ItemsetTable;

class Itemset{
    
    std::string        itemset;
    int                utility;
    int                support;
    int                TWU;
    std::set<int>      idList;
    std::map<int, int> utilList;
    Itemset*           prev;
    Itemset*           next;
    friend class       ItemsetTable;

public:
    Itemset(std::string RHSItemset);
    void genIDList(Dataset& dataset, ItemsetTable& itemsetTable);
    void genUtilList(Dataset& dataset, ItemsetTable& itemsetTable);
    bool isSubset(std::string subset);
    bool isGenerator(ItemsetTable& itemsetTable);
    bool isHUI(int minUtil);
    void computeTWU(Dataset& dataset);
    int  retTWU();
    void printIDList();
    void printItemsetDetails();
};
#endif
