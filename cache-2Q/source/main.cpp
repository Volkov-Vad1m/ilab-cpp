#include "../headers/cache.hpp"

int main()
{
    size_t sz;
    size_t numRequest;
    std::cin >> sz >> numRequest;
    
    int hits = 0;
    caches::Cache_2Q<int, int> ch{sz}; 
    
    int request;
    
    for(int i = 0; i < numRequest; i++)
    {
        std::cin >> request;
        if( CacheHit <int, int> (ch, request) )
        {
            hits += 1;
        }
    } 
   
    std::cout << hits << std::endl;
    
    return 0;
}