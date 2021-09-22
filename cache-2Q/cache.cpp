
#include "cache.hpp"

template <typename KeyT, typename Data>
int CacheHit (caches::Cache_2Q<KeyT, Data> &Cache, size_t numRequest)
{
    int numHit = 0;
    Data request;
    for(int i = 0; i < numRequest; i++)
    {
        std::cin >> request;
        assert(std::cin.good());

        auto find = Cache.Hash.find(request);

        if(find == Cache.Hash.end()) // did not find
        {

            struct Node <Data> newPage;
            newPage.data = request;
            newPage.place = IN;

            if(Cache.In.isfull())
            {
                auto backIn = Cache.In.List.back(); // последний элемент в IN
                Cache.In.List.pop_back(); // достаем его из листа IN
                Cache.Hash.erase(backIn); // удаляем из хэша

                if(Cache.Out.isfull()) // если Out полон
                {
                    auto backOut = Cache.Out.List.back(); // последний элемент в OUT
                    // отправляем его в Ад
                    Cache.Out.List.pop_back();
                    Cache.Hash.erase(backOut);
                }

                Cache.In.List.push_front(newPage); // добавляем новый элемент в In
                auto hashIns = Cache.In.List.begin();
                Cache.Hash.insert(hashIns);
                // последний элемент из In переносится в OUT
                backIn.place = OUT;
                Cache.Out.List.push_front(backIn);
                Cache.Hash.insert({backIn.data, backIn});
                
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
                numHit++;
                continue;
            }
            
            if(find->second->place == OUT)
            {
                if(Cache.Hot.isfull())
                {
                    auto backHot = Cache.Hot.List.back();
                    auto inhash = Cache.Hash.find(backHot->data);
                    // put code
                }
            }

            if(find->second->place = HOT) // ready
            {
                numHit++;
                Cache.Hot.List.splice(Cache.List.Hot.begin(), Cache.List.Hot, find->second);
                continue;
            }
            
        }



    }
};