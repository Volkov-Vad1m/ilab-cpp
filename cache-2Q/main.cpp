#include "cache.hpp"

int main()
{
    size_t sz;
    size_t numRequest;
    std::cin >> sz >> numRequest;
    
    int caacheHit;
    caches::Cache_2Q<int, int> ch{sz}; 

    int request;

    for(int i = 0; i < numRequest; i++)
    {
        std::cin >> request;
        if( CacheHit(ch, request) )
            caacheHit++;
    } 
   
    std::cout << caacheHit << std::endl;
    
    return 0;
}