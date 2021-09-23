
#include "cache.hpp"

template <typename KeyT, typename Data>
bool CacheHit (caches::Cache_2Q<KeyT, Data> Cache, Data request)
{    

    auto find = Cache.Hash.find(request);

    if( find == Cache.Hash.end() ) // did not find
    {

        struct Node <Data> newPage;  // создаём новую страницу, которая будет помещена в кэш IN
        newPage.data = request;
        newPage.place = IN;

        if(Cache.In.isfull()) // если IN полон, вытесняем из IN последний элемент и отправляем его в OUT
        {
            auto backIn = Cache.In.List.back(); // последний элемент в IN
            Cache.In.List.pop_back(); // достаем его из листа IN

            Cache.Hash.erase(backIn->data); // удаляем из хэша ( по ключу )

            if(Cache.Out.isfull()) // если Out полон
            {
                auto backOut = Cache.Out.List.back(); // последний элемент в OUT
                // отправляем его в Ад
                Cache.Out.List.pop_back();
                Cache.Hash.erase(backOut->data);
                // 
            }

            Cache.In.List.push_front(newPage); // добавляем новый элемент в In
            auto hashIns = Cache.In.List.begin();
            Cache.Hash.insert({hashIns->data, hashIns} );
            // последний элемент из In переносится в OUT
            backIn->place = OUT;
            Cache.Out.List.push_front({backIn->data, backIn->place});
            Cache.Hash.insert({backIn->data, backIn});
                // /
        }

        else
        {
            Cache.In.List.push_front(newPage);
            Cache.Hash.insert({request, Cache.In.List.begin()});
        }
    }


    else // нашли
    {
        if(find->second->place == IN)  // ready
        {
            return true;
        }
            
        if(find->second->place == OUT)
        {
            if(Cache.Hot.isfull()) 
            {
                auto backHot = Cache.Hot.List.back(); // вытесняем из Hot последний элемент
                Cache.Hash.erase(backHot->data); // удаляем из таблицы последний элемент из HOT 
    
            }  
            // вставляем в HOT наш find
            find->second->place = HOT;
            Cache.Hot.List.push_front( {find->second->data, find->second->place} ); // вставляем в список
            Cache.Hash.insert({find->second->data, find->second}); // или это не надо 

        }

        if(find->second->place = HOT) // ready
        {
            Cache.Hot.List.splice(Cache.List.Hot.begin(), Cache.List.Hot, find->second);
            return true;
        }
            
    }

};