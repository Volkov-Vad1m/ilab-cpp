#include <iostream>
#include "hw3d.hpp"

int main()
{
    Point arr[3];
    arr[0] = {- 1};
    arr[1] = {1, 1};
    arr[2] = {0, 0, -1};
    for(int i = 0; i<3; i++)
        arr[i].point_print();

    Triangle t1{arr[0], arr[1], arr[2]};

    OctTree*  tree = new OctTree; 
    tree->root_->region_.centre_.point_print();
    tree->root_->insert_triangle(t1);
    tree->root_->childs_[2]->region_.centre_.point_print();
    std::cout << tree->root_->childs_[2]->blockNum_ << std::endl;

}

