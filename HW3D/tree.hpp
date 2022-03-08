#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>
#include "geometry.hpp"

const float MIN_SIZE = 1;
#define epsilon 10E-6

//....................................................
//....................................................
class OctNode
{
public:
    OctNode* parent_; 
    OctNode* childs_[8] {};
    int blockNum_; //номер блока по отношению к parent, 
    Box region_; // размеры куба
    //int level_; // уровень относительно root
    std::vector<struct Triangle> objects_; // треугольники, попавшие в куб.   
public:
    OctNode() {
        std::cout << "default" << std::endl;
        region_ = { {-2, -2, -2} , {2, 2, 2}};
    }
    OctNode(OctNode* prnt, int block) : parent_(prnt), blockNum_(block) { // block - в каком октанте относительно parent {{correctly}}
        get_region();   
        prnt->childs_[block] = this; 
    }
    ~OctNode() {
        for(int i = 0; i < 8; i++) {
            delete childs_[i];
        }
    }
    
    //..............................................
    //записывает в blockNum номер, где находится узел относительно parent
    void get_blockNum() // не знаю, надо ли это вообще
    { 
        blockNum_ = (region_.centre_.coord_[0] > parent_->region_.centre_.coord_[0]) << 2 |
                    (region_.centre_.coord_[1] > parent_->region_.centre_.coord_[1]) << 1 | 
                    (region_.centre_.coord_[2] > parent_->region_.centre_.coord_[2]);
    };
    //..............................................
    // метод определяет region, зная, какой это блок относительно parent_
    void get_region() // {{correctly}}
    {
        int bit_mask[3] = {4, 2, 1}; // 100b, 010b, 001b.
        auto& prt = parent_->region_;

        for(int i = 0; i < 3; i++) { // порядок x, y ,z

            if(blockNum_ & bit_mask[i]) {
                region_.left_.coord_[i] = ( prt.left_.coord_[i] + prt.right_.coord_[i] ) / 2;
                region_.right_.coord_[i] = prt.right_.coord_[i];
            }

            else {
                region_.left_.coord_[i] = prt.left_.coord_[i];
                region_.right_.coord_[i] = ( prt.left_.coord_[i] + prt.right_.coord_[i] ) / 2;
            }
        }

        region_.sizeSide_ = region_.right_.coord_[0] - region_.left_.coord_[0]; 
        region_.get_centre();
    }
    //..............................................
    //..............................................
     bool triangle_in_box(Triangle tr, Box region) { // {{correctly}}
        bool result = 1;
        for (int i = 0; i < 3; i++) {
            result = result && tr.pt_[i].point_in_box(region);
        }
        return result;
    };
    //..............................................
    //..............................................
    void insert_triangle(Triangle tr) {
        std::cout << "start\n" ;
        region_.left_.point_print();
        region_.right_.point_print();

        if(region_.sizeSide_ < MIN_SIZE ) // если размер области меньше минимальной
        {
            std::cout << "маленький член\n" ;
            objects_.push_back(tr);
            return; 
        }

        int blockNum = (tr.triangleCentre_.coord_[0] > region_.centre_.coord_[0]) << 2 |
                       (tr.triangleCentre_.coord_[1] > region_.centre_.coord_[1]) << 1 | 
                       (tr.triangleCentre_.coord_[2] > region_.centre_.coord_[2]); // номер блока, куда попал центр треугольника
                       std::cout << blockNum << "номер блока" << std::endl;
        // мы будем вставлять именно в этот октант, проверяя что все 3 точки полностью попадают в него.
        // если это не так, прекращаем вставку треугольника
        if (childs_[blockNum] == nullptr) {// мы в самом низу дерева
            childs_[blockNum] = new OctNode{this, blockNum}; // хуйня 
            std::cout << "не создана нода\n";
            childs_[blockNum]->region_.left_.point_print();
            childs_[blockNum]->region_.right_.point_print();
            std::cout << "\n\n";

        }

        if( triangle_in_box(tr, childs_[blockNum]->region_) ) { // если треугольник полностью попал в octant, продолжаем вставку // вместо region_ childs[blocknum] region
            std::cout << "вставляем" <<std::endl;
            childs_[blockNum]->insert_triangle(tr);
            return;
        }
        else {
            std::cout << "хуй\n";
            objects_.push_back(tr);
            return; 
        }
    };
    //..............................................
    //..............................................
    void dump_tree() {
        using VecIt = typename std::vector<Triangle>::iterator; 
        std::cout << "box" << std::endl;
        region_.left_.point_print();
        region_.right_.point_print();
        std::cout << "~~~~~" << std::endl;
        for(VecIt It = objects_.begin(); It != objects_.end(); ++It) {
            It->triangle_print();
            std::cout << "---" << std::endl;
        }
        std::cout << "end iteration" << std::endl;

        for (int i = 0; i < 8; ++i) {
            if(!childs_[i])
                continue;
            std::cout << " номер " << i << std::endl;
            childs_[i]->dump_tree();
        }
    }
};

class OctTree {
public:
    OctNode* root_; // указатель на корень дерева
    std::vector<struct Triangle> queue_; // поступающие запросы

    OctTree()
    {
        root_ = new OctNode;
    }
    ~OctTree()
    {
        delete root_;
    }
};
