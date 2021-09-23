
#include "cache.hpp"


int main()
{
    size_t sz;
    size_t numRequest;
    std::cin >> sz >> numRequest;
    
    caches::Cache_2Q<int, int> ch{sz}; 

    ch.In.List.push_front({1, IN});
    ch.In.List.push_front({1, IN});
    std::cout << ch.In.isfull() << std::endl;




}