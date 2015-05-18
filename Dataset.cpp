#include "Dataset.h"

/* Dataset functions implementation */
Dataset::Dataset(){
    
    numOfHeads = 0;
}

void Dataset::addTransacUtil(int transac, int RHSTransacUtil){
    
    /* if added transaction is out of bound
       need to resize the vector
    */
    if( transac > numOfHeads ){

        headList.resize(transac+1, NULL);
        numOfHeads = transac;
    }
   
    headList[transac] = new Items();
    headList[transac] -> transacUtil = RHSTransacUtil;
}

void Dataset::addItem(int transac, std::string item, int util){
    
    headList[transac] -> items.insert( std::pair<std::string, int>(item, util) );
}

bool Dataset::existInTransac(std::string itemset, int transac){
    
    if(headList[transac] -> items.count(itemset) == 1)
        return true;

    return false;
}

int Dataset::retNumOfTransac(){
    
    return numOfHeads;
}

int Dataset::retTransacUtil(int transac){
    
    return headList[transac] -> transacUtil;
}

int Dataset::retItemUtilInTransac(std::string item, int transac){
    
    return headList[transac] -> items[item];
}

void Dataset::printDataset(){
    
    for(int i=1 ; i<=numOfHeads ; i++)
        headList[i] -> printItems();
}
