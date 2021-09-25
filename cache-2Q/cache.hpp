#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>

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

namespace caches {

template <typename Data> 
struct QueueMap {

    size_t sizeList; // размер листа
    std::list<struct Node<Data>> List; // сам лист 
    
    // constructor 
    QueueMap(size_t sz) : sizeList(sz) {}

    bool isfull()
    {
        return (sizeList == List.size());
    }

};

template <typename KeyT, typename Data>
struct Cache_2Q {

    size_t sizeCache;

    
    using ListIt = typename std::list<struct Node<Data>>::iterator;
    std::unordered_map<KeyT, ListIt> Hash;

    Cache_2Q(size_t sz) : sizeCache(sz) {}
    
    QueueMap <Data> In {sizeCache / 5};
    QueueMap <Data> Out {sizeCache - sizeCache / 5 - sizeCache / 5};
    QueueMap <Data> Hot {sizeCache / 5};


    void Erase (Data data) // удаляет из кэша по данным
    {
        auto delPage = Hash.find(data);
        auto delList = delPage->second;

        Hash.erase(delPage);

        if(delList->place == IN)
        {
            In.List.erase(delList);
            return;
        }

        if(delList->place == OUT)
        {
            Out.List.erase(delList);
            return;
        }

        if(delList->place == HOT)
        {
            Hot.List.erase(delList);
            return;
        }
    };
};

};

///////////////////////////
template <typename KeyT, typename Data>
bool CacheHit (caches::Cache_2Q<KeyT, Data> &Cache, Data request)
{    
    
    auto find = Cache.Hash.find(request);
    
    if( find == Cache.Hash.end() ) // did not find
    {
        struct Node <Data> newPage;  // создаём новую страницу, которая будет помещена в кэш IN
        newPage.data = request;
        newPage.place = IN;

        if(Cache.In.isfull()) // если IN полон, вытесняем из IN последний элемент и отправляем его в OUT
        {

            auto backIn = Cache.Hash.find(Cache.In.List.back().data); // последний элемент в IN backIn->second //+
        
            if(Cache.Out.isfull()) // если Out полон
            {
                Cache.Erase(Cache.Out.List.back().data);
            }
            // последний элемент из In переносится в OUT
            backIn->second->place = OUT;
            Cache.Out.List.splice(Cache.Out.List.begin(), Cache.In.List, backIn->second);
            

            Cache.In.List.push_front(newPage); 
            auto hashIns = Cache.In.List.begin();
            Cache.Hash.insert({hashIns->data, hashIns} );      
                        //list.splice(куда вставляем, из какого листа вставляем, что вставляем)
        }
        
        else 
        {
            Cache.In.List.push_front(newPage);
            Cache.Hash.insert({newPage.data, Cache.In.List.begin()});
        }
        
        return false;
    }


    else // нашли
    {
        if(find->second->place == IN)
        {
            return true;
        }
            
        if(find->second->place == OUT) 
        {
            
            if(Cache.Hot.isfull()) 
            {
                Cache.Erase(Cache.Hot.List.back().data);
            } 
            find->second->place = HOT;
            Cache.Hot.List.splice(Cache.Hot.List.begin(), Cache.Out.List, find->second);
            return true;
        }

        if(find->second->place = HOT) // ready
        {
            Cache.Hot.List.splice(Cache.Hot.List.begin(), Cache.Hot.List, find->second);
            return true;
        }
    }
    return false;

}
 
