

// #include <iostream>
// #include <iterator>
// #include <list>
// #include <unordered_map>
// #include <cassert>

// enum whatList
// {
//     IN,
//     OUT,
//     HOT
// };

// template <typename Data> 
// struct Node
// {
//     Data data;
//     whatList place;
// };

// int main()
// {
//     using ListIt = typename std::list<struct Node<int>>::iterator;

//     std::unordered_map<int, ListIt> map;

//     std::list <struct Node<int>> lst;

//     lst.push_front({12, IN});
//     std::list <struct Node<int>>::iterator It;
//     It = lst.begin();
//     lst.push_front({13, IN});
//     lst.push_front(It);
//     It = lst.begin();
//     map.insert({It->data, It});
//     auto hui = map.find(14);
//     map.erase(hui);
//     //auto mp = map.find(14);
//     //std::cout << mp->first << std::endl;
// }