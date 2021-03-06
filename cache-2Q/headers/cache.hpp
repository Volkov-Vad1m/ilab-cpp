#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>
//.....................................................
//.....................................................
enum whatList
{
    IN,
    OUT,
    HOT
};

template <typename Data> 
struct Node
{
    Data data;
    whatList place;
};
//.....................................................
//.....................................................
namespace caches {

template <typename Data> 
struct QueueMap {

    size_t sizeList;
    std::list<struct Node<Data>> List; 
    
    QueueMap(size_t sz) : sizeList(sz) {}

    bool isfull()
    {
        return (sizeList == List.size());
    }

};
//.....................................................
//.....................................................
template <typename KeyT, typename Data>
struct Cache_2Q {

    size_t sizeCache;

    using ListIt = typename std::list<struct Node<Data>>::iterator;
    using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;

    std::unordered_map<KeyT, ListIt> Hash;

    

    Cache_2Q(size_t sz) : sizeCache(sz) {}
    
    QueueMap <Data> In {sizeCache - (3 * (sizeCache / 5)) - (sizeCache / 5)};
    QueueMap <Data> Out { 3 * (sizeCache / 5 ) };
    QueueMap <Data> Hot {sizeCache / 5};

    //.....................................................
    //.....................................................
    void Erase (KeyT key) 
    {
        auto delPage = Hash.find(key);
        if(delPage == Hash.end()) 
            return;

        auto delList = delPage->second;
        Hash.erase(delPage);

        switch (delList->place)
        {
        case IN:
            In.List.erase(delList);
            break;
        case OUT:
            Out.List.erase(delList);
            break;
        case HOT:
            Hot.List.erase(delList);
            break;
        }

        return;
    };
    //.....................................................
    //.....................................................
    void Request_notFound(HashIt &find, Data request)
    {
        struct Node <Data> newPage; 
        newPage.data = request; 
        newPage.place = IN;

        if(In.isfull()) 
        {
            auto backIn = Hash.find(In.List.back().data); 
            if(Out.sizeList == 0)
            {
                Hash.erase(backIn);
                In.List.pop_back(); // ?? ????
                In.List.push_front(newPage); 
                auto hashIns = In.List.begin();
                Hash.insert({hashIns->data, hashIns} );     
                return; 
            }

            if(Out.isfull())  // ????????
            {
                Erase(Out.List.back().data);
            }
            backIn->second->place = OUT;
            Out.List.splice(Out.List.begin(), In.List, backIn->second);
            

            In.List.push_front(newPage); 
            auto hashIns = In.List.begin();
            Hash.insert({hashIns->data, hashIns} );     
            return; 
        }
        
        else 
        {
            In.List.push_front(newPage);
            Hash.insert({newPage.data, In.List.begin()});
            return;
        }
    };
    //.....................................................
    //.....................................................
    void Request_Found(HashIt &find)
    {
        if(find->second->place == IN)
        {
            return;
        }
            
        if(find->second->place == OUT) 
        {
                
            if(Hot.isfull()) 
            {
                Erase(Hot.List.back().data);
            } 
            find->second->place = HOT;
            Hot.List.splice(Hot.List.begin(), Out.List, find->second);
            return;
        }

        if(find->second->place = HOT) 
        {
            Hot.List.splice(Hot.List.begin(), Hot.List, find->second);
            return;
        }
    };
    //.....................................................
    //.....................................................
    bool CacheHit (KeyT key, Data (*slow_get_page)(KeyT))
    {    
        auto find = Hash.find(key);
        
        if( find == Hash.end() ) 
        {

            Request_notFound(find, slow_get_page(key));
            return false;   
        }

        else 
        {   
            Request_Found(find);
            return true;
        }
    };
    
}; // end Cache_2Q

}; // end namespace caches

