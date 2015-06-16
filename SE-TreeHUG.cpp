#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <string>
#include <cstring>

#include "Dataset.h"
#include "ItemsetTable.h"

int main (int argc, char** argv){
    
    std::fstream datasetFile(argv[1], std::ios::in);
    std::fstream unitProfitFile(argv[2], std::ios::in);
    
    std::map<std::string, int> unitProfit;

    /* Preprocess */
        /* unit profit file */
    std::string tempItem;
    int         tempUnitProfit;

    while( unitProfitFile >> tempItem >> tempUnitProfit )
        unitProfit.insert( std::pair<std::string, int>(tempItem, tempUnitProfit) );

        /* dataset file */
    Dataset dataset;
    std::string tempBuff;
    int transac = 1;

    while( getline(datasetFile, tempBuff) ){
        
        char* tempBuffCstr;
        char* tempBuffCstrPtr;
        int tempTransacUtil;

        tempBuffCstr = new char [tempBuff.length() + 1];
        std::strcpy(tempBuffCstr, tempBuff.c_str());

        /* parse Transaction Util */
        tempBuffCstrPtr = std::strtok(tempBuffCstr, " ");
        tempTransacUtil = atoi(tempBuffCstrPtr);
        dataset.addTransacUtil(transac, tempTransacUtil);

        while( tempBuffCstrPtr = std::strtok(NULL, ",") ){
            
            /* parse item */
            std::string tempItem(tempBuffCstrPtr);

            /* parse item's quantity
               util(item) = quantity(item) * unitProfit(item)
            */
            tempBuffCstrPtr = std::strtok(NULL, " ");
            int tempUtil = atoi(tempBuffCstrPtr) * unitProfit[tempItem];

            dataset.addItem(transac, tempItem, tempUtil);
        }

        transac++;
    }
    
    /* print the original Dataset */
    dataset.printDataset();
    std::cout << "===========================================" << std::endl;

    /* generate HUG */
    ItemsetTable itemsetTable( unitProfit.size() );
 
    for(std::map<std::string, int>::iterator It = unitProfit.begin() ;
        It != unitProfit.end() ;
        It++){
        
        std::vector<std::string> tempItem;
        tempItem.push_back(It->first);
        itemsetTable.addItemset(tempItem);
    }
    
    int minUtil = atoi(argv[3]);

    itemsetTable.genHUG(minUtil, dataset);
}
