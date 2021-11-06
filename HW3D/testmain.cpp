#include "tree.hpp"

int main()
{
    int bit_mask[3] = {4, 2, 1}; // 100b, 010b, 001b.
    int blockNum_ = 7;
    Point l = {0, 0, 0};
    Point r = {2, 2, 2};
    Box prt{l, r};
    Box region_ {};

    for(int i = 0; i < 3; i++) { // порядок x, y ,z
        std::cout << (blockNum_ & bit_mask[i]) << std::endl;

            if(blockNum_ & bit_mask[i]) {
                region_.left_.coord_[i] = ( prt.left_.coord_[i] + prt.right_.coord_[i] ) / 2;
                region_.right_.coord_[i] = prt.right_.coord_[i];
            }

            else {
                region_.left_.coord_[i] = prt.left_.coord_[i];
                region_.right_.coord_[i] = ( prt.left_.coord_[i] + prt.right_.coord_[i] ) / 2;
            }
    }
    
    region_.left_.point_print();
    region_.right_.point_print();

}
