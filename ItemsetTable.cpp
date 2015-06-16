#include "Itemset.h"
#include "ItemsetTable.h"
#include "Dataset.h"

/* ItemsetTable functions implementation */
ItemsetTable::ItemsetTable(int RHSNumOfHeads){
    
    headList.resize(RHSNumOfHeads+1 , NULL);
    numOfHeads = RHSNumOfHeads;
}

void ItemsetTable::addItemset(std::vector<std::string>& RHSItemset){
   
    int itemsetLen = RHSItemset.size();
    
    /* add the new Itemset to the end of the list */
    Itemset* newItemset = new Itemset(RHSItemset);
    Itemset* itemsetCursor = headList[itemsetLen];
    
    if( !itemsetCursor ){

        /* empty list */
        headList[itemsetLen] = newItemset;
        newItemset->prev = newItemset;
    }
    else{
        
        /* nonempty list
           run to the second last node
        */
        while( itemsetCursor->next )
            itemsetCursor = itemsetCursor->next;
        
        itemsetCursor->next = newItemset;
        newItemset->prev = itemsetCursor;
    }
}

Itemset* ItemsetTable::delItemset(std::vector<std::string>& RHSItemset){
    
    /* the return itemset
       if empty list return NULL
    */
    Itemset* retItemset = NULL;
    int itemsetLen = RHSItemset.size();
    Itemset* itemsetCursor = headList[itemsetLen];
    
    /* find the itemset */
    while(itemsetCursor){
        
        if(itemsetCursor->itemset == RHSItemset)
            break;

        itemsetCursor = itemsetCursor->next;
    }
    
    if(itemsetCursor->prev == itemsetCursor){
        
        /* is first node */
        if(itemsetCursor->next){

            /* following node exists */
            itemsetCursor->next->prev = itemsetCursor->next;
            headList[itemsetLen] = itemsetCursor->next;
            retItemset = itemsetCursor->next;
        }
        else
            headList[itemsetLen] = NULL;
    }
        /* is last node */
    else if( !(itemsetCursor->next) )
        itemsetCursor->prev->next = NULL;
    else{
        
        /* node is in the middle */
        itemsetCursor->prev->next = itemsetCursor->next;
        itemsetCursor->next->prev = itemsetCursor->prev;
        retItemset = itemsetCursor->next;
    }

    delete itemsetCursor;
    return retItemset;
}

bool ItemsetTable::subsetExist(std::vector<std::string>& itemset, std::vector<int>& subset, int lenOfSubset){
    
    /* check if subset exist in the previous layer */
    std::vector<std::string> realItemset;

    for(int i=lenOfSubset-1 ; i>=0 ; i--)
        realItemset.push_back( itemset[ subset[i]-1 ] );
    
    Itemset* itemsetCursor = headList[lenOfSubset];
    
    while(itemsetCursor){
        
        if(itemsetCursor->itemset == realItemset)
            return true;

        itemsetCursor = itemsetCursor -> next;
    }

    return false;
}

bool ItemsetTable::allSubsetExist(std::vector<std::string>& itemset, int lenOfSubset){
    
    /* check if all Len(itemset)-1 subsets of itemset exist */
    std::vector<int> subset;

    for(int i=lenOfSubset ; i>0 ; i--)
        subset.push_back( i );
    
    if( !(subsetExist(itemset, subset, lenOfSubset)) )
        return false;

    for(int i=0 ; i<lenOfSubset ; ){
        
        if(subset[i] + i < itemset.size()){

            subset[i] ++;

            for(int j=i-1 ; j>=0 ; j--)
                subset[j] = subset[j+1] + 1;

            i=0;

            if( !(subsetExist(itemset, subset, lenOfSubset)) )
                return false;
        }
        else
            i++;
    }

    return true;
}

void ItemsetTable::genNextLayer(int layer){
    
    /* generate layer+1 of layer
       
       ABC + ABD = ABCD
    */
    Itemset* itemsetCursor1 = headList[layer];

    while(itemsetCursor1){
        
        std::vector<std::string> realItemset( itemsetCursor1->itemset.begin(),
                                              itemsetCursor1->itemset.end()-1 );
        std::vector<std::string> realItemsetCopy = realItemset;

        Itemset* itemsetCursor2 = itemsetCursor1->next;

        while(itemsetCursor2){

            realItemset = realItemsetCopy;

            /* the 2 itemset share the same prefix */
            if( itemsetCursor2->isSubset(realItemset) ){
                
                realItemset.push_back(itemsetCursor1->itemset[ layer-1 ]);
                realItemset.push_back(itemsetCursor2->itemset[ layer-1 ]);

                /* if layer == 1 : simply add the new itemset
                   if layer >  1 : if   all subsets exist : add the new itemset
                                   else : skip
                */
                if(layer == 1 || allSubsetExist(realItemset, layer))
                    addItemset(realItemset);
            }

            itemsetCursor2 = itemsetCursor2 -> next;
        }

        itemsetCursor1 = itemsetCursor1 -> next;
    }
}

void ItemsetTable::genHUG(int minUtil, Dataset& dataset){
    
    /* generate all High Util Generators */
    for(int i=1 ; i<=numOfHeads ; i++){
        
        Itemset* itemsetCursor = headList[i];

        while(itemsetCursor){
            
            /* generate ID List (compute support) */
            itemsetCursor->genIDList(dataset, *this);

            /* for a itemset X :
               (if X is a generator => subset(X) is a generator) implies
               (if subset(X) not a generator => X not a generator)
               
               => prune those itemsets X that are not generators
            */
            if( !( itemsetCursor->isGenerator(*this) ) ){

                itemsetCursor = delItemset(itemsetCursor->itemset);
                continue;
            }

            /* compute TWU from ID List */
            itemsetCursor->computeTWU(dataset);
            
            /* for a itemset X :
               if TWU(X) < min util 
               => X is not a HUI
               => delete itemset X
            */
            if(itemsetCursor->retTWU() < minUtil){

                itemsetCursor = delItemset(itemsetCursor->itemset);
                continue;
            }

            /* generate util list */
            itemsetCursor->genUtilList(dataset, *this);
            
            /* check if it is HUI and print result */
            if( itemsetCursor->isHUI(minUtil) )
                itemsetCursor -> printItemsetDetails();

            itemsetCursor = itemsetCursor->next;
        }
        
        /* only have to generate next layer
           when layer < # of items
        */
        if( i < numOfHeads )
            genNextLayer(i);
    }
}

void ItemsetTable::printItemsetTable(){
    
    for(int i=1 ; i<=numOfHeads ; i++){
        
        Itemset* itemsetCursor = headList[i];

        while(itemsetCursor){
            
            itemsetCursor -> printItemsetDetails();
      
            itemsetCursor = itemsetCursor->next;
        }
    }
}

