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

};

};

///////////////////////////
template <typename KeyT, typename Data>
bool CacheHit (caches::Cache_2Q<KeyT, Data> &Cache, Data request)
{    
    
    auto find = Cache.Hash.find(request);
    
    if( find == Cache.Hash.end() ) // did not find
    {
        std::cout << "не нашли" << std::endl;

        struct Node <Data> newPage;  // создаём новую страницу, которая будет помещена в кэш IN
        newPage.data = request;
        newPage.place = IN;

        if(Cache.In.isfull()) // если IN полон, вытесняем из IN последний элемент и отправляем его в OUT
        {
            std::cout << "мы в IN full" << std::endl;
    
            auto backIn = Cache.In.List.end(); // последний элемент в IN
            backIn--;
            Cache.In.List.pop_back(); // достаем его из листа IN
        
            Cache.Hash.erase(backIn->data); // удаляем из хэша ( по ключу )
            if(Cache.Out.isfull()) // если Out полон
            {
                std::cout << "OUT full" << std::endl;
                auto backOut = Cache.Out.List.end(); // последний элемент в OUT
                backOut--;
                // отправляем его в Ад
                Cache.Out.List.pop_back();
                Cache.Hash.erase(backOut->data);
                // 
            }
            std::cout << "huy" << std::endl;
            Cache.In.List.push_front(newPage); // добавляем новый элемент в In
            auto hashIns = Cache.In.List.begin();
            Cache.Hash.insert({hashIns->data, hashIns} );
            // последний элемент из In переносится в OUT
            backIn->place = OUT;
            Cache.Out.List.push_front({backIn->data, backIn->place});
            Cache.Hash.insert({backIn->data, backIn});
            
            
        }
        
        else
        {
            std::cout << "IN не full" << std::endl;

            Cache.In.List.push_front(newPage);
            Cache.Hash.insert({newPage.data, Cache.In.List.begin()});
        }
        
        return false;
    }


    else // нашли
    {
        if(find->second->place == IN)  // ready
        {
            std::cout << "нашли в IN" << std::endl;

            return true;
        }
            
        if(find->second->place == OUT)
        {
            std::cout << "нашли в OUT" << std::endl;
            if(Cache.Hot.isfull()) 
            {
                auto backHot = Cache.Hot.List.end(); // вытесняем из Hot последний элемент
                backHot--;
                Cache.Hash.erase(backHot->data); // удаляем из таблицы последний элемент из HOT 
    
            }  
            // вставляем в HOT наш find
            find->second->place = HOT;
            Cache.Hot.List.push_front( {find->second->data, find->second->place} ); // вставляем в список
            Cache.Hash.insert({find->second->data, find->second}); // или это не надо 
            return true;
        }

        if(find->second->place = HOT) // ready
        {
            std::cout << "нашли в HOT" << std::endl;
            Cache.Hot.List.splice(Cache.Hot.List.begin(), Cache.Hot.List, find->second);
            return true;
        }
        return true;
            
    }

}
 
