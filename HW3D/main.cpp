#include <iostream>
#include "tree.hpp"

int main() {
    Point arr[3];
    arr[0] = {0, 0, 0};
    arr[1] = {1, 1, 1};
    arr[2] = {-1, -1, -1};

    Triangle t1{arr[0], arr[1], arr[2]};

    // OctTree*  tree = new OctTree; 
    // tree->root_->region_.centre_.point_print();
    // tree->root_->insert_triangle(t1);
    // tree->root_->childs_[2]->region_.centre_.point_print();
    // std::cout << tree->root_->childs_[2]->blockNum_ << std::endl;
    Point pt = {99, 99, 102};
    Point l = {-2, -2, -2};
    Point r = {2, 2, 2};
    Box reg{l, r};
    
    OctNode* node = new OctNode{};
    OctNode* prt = new OctNode{};

 
   // std::cout << node->region_.left_.coord_[0] << std::endl;
    //std::cout << node->childs_[1] << std::endl;
    //OctNode* node1 = new OctNode{node, 0};
    
    //node1->region_.left_.point_print();
    //node1->region_.right_.point_print();
    node->insert_triangle(t1);
   // std::cout << node->childs_[0]->objects_[0].triangle_print() << std::endl;
   //node->region_.left_.point_print();
   //node->region_.right_.point_print();
    // std::cout << node->triangle_in_box(t1, node->region_) << std::endl;
   // std::cout << "point in octant " << pt.point_in_box(reg) << std::endl;
    std::cout << "dump start\n";
    //node->dump_tree();
    std::cout << "dump end\n";
    delete node;
    delete prt;
    return 0;
}

