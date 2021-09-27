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
template <typename Data>
using ListIt = typename std::list<struct Node<Data>>::iterator;
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

    std::unordered_map<KeyT, ListIt<Data>> Hash;

    Cache_2Q(size_t sz) : sizeCache(sz) {}
    
    QueueMap <Data> In {sizeCache / 5};
    QueueMap <Data> Out {sizeCache - sizeCache / 5 - sizeCache / 5};
    QueueMap <Data> Hot {sizeCache / 5};


    void Erase (Data data) 
    {
        auto delPage = Hash.find(data);
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
};

};
//.....................................................
//.....................................................
template <typename KeyT, typename Data> 
void Request_notFound(caches::Cache_2Q<KeyT, Data> &Cache, typename std::unordered_map<KeyT, ListIt<Data>>::iterator &find, Data request)
{
    struct Node <Data> newPage; 
    newPage.data = request; 
    newPage.place = IN;

    if(Cache.In.isfull()) 
    {

        auto backIn = Cache.Hash.find(Cache.In.List.back().data); 
        
        if(Cache.Out.isfull()) 
        {
            Cache.Erase(Cache.Out.List.back().data);
        }

        backIn->second->place = OUT;
        Cache.Out.List.splice(Cache.Out.List.begin(), Cache.In.List, backIn->second);
            
        Cache.In.List.push_front(newPage); 
        auto hashIns = Cache.In.List.begin();
        Cache.Hash.insert({hashIns->data, hashIns} );      
    }
        
    else 
    {
        Cache.In.List.push_front(newPage);
        Cache.Hash.insert({newPage.data, Cache.In.List.begin()});
    }
};
//.....................................................
//.....................................................
template <typename KeyT, typename Data> 
void Request_Found(caches::Cache_2Q<KeyT, Data> &Cache, typename std::unordered_map<KeyT, ListIt<Data>>::iterator &find)
{
    if(find->second->place == IN)
    {
        return;
    }
            
    if(find->second->place == OUT) 
    {
            
        if(Cache.Hot.isfull()) 
        {
            Cache.Erase(Cache.Hot.List.back().data);
        } 
        find->second->place = HOT;
        Cache.Hot.List.splice(Cache.Hot.List.begin(), Cache.Out.List, find->second);
        return;
    }

    if(find->second->place = HOT) 
    {
        Cache.Hot.List.splice(Cache.Hot.List.begin(), Cache.Hot.List, find->second);
        return;
    }
};
//.....................................................
//.....................................................
template <typename KeyT, typename Data>
bool CacheHit (caches::Cache_2Q<KeyT, Data> &Cache, Data request)
{    
    
    auto find = Cache.Hash.find(request);
    
    if( find == Cache.Hash.end() ) 
    {
        Request_notFound(Cache, find, request);
        return false;   
    }

    else 
    {
        Request_Found(Cache, find);
        return true;
    }
};
 
