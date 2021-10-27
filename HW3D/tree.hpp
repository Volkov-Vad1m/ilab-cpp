#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>

const float MIN_SIZE = 1;

#define epsilon 10E-6

class OctNode;

struct Box;
struct Point;

struct Point {
    float coord_[3]; // (x, y ,z)

    Point(float x = 0, float y = 0, float z = 0) : coord_{x, y , z} {};

    void point_print() {
        std::cout << "(" << coord_[0] << "; " << coord_[1] << "; " << coord_[2] << ")" << std::endl;
        return;
    };

    bool point_in_octant(Box &region);
    //....................................................
    //....................................................
};

struct Box {
    Point left_;
    Point right_;
    Point centre_; // координаты центра куба
    float sizeSide_;

    Box(Point l, Point r) : left_(l), right_(r) {
        get_centre();
        sizeSide_ = right_.coord_[0] - left_.coord_[0];
    }
    Box() {};

    void get_centre() {
        for(int i = 0; i < 3; i++)
            centre_.coord_[i] = (left_.coord_[i] + right_.coord_[i]) / 2;
    }
};
//....................................................
//....................................................
bool Point::point_in_octant(Box &region) {
    bool result = true;
    for(int i = 0; i < 3; i++)
        result = result && ( ( coord_[i] > region.left_.coord_[i]) && (coord_[i] < region.right_.coord_[i]) ); 
    return result;
}
//....................................................
//....................................................
struct Triangle {
    Point pt_[3];
    Point triangleCentre_; // центр треугольника

    Triangle(Point pt1, Point pt2, Point pt3) {
        pt_[0] = pt1; 
        pt_[1] = pt2; 
        pt_[2] = pt3;
        // вычесляем центр треугольника
        triangleCentre_;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                triangleCentre_.coord_[j] += pt_[i].coord_[j] / 3;
            }
        }
    };

    void triangle_print() {
        for(int i = 0; i < 3; i++) {
            pt_[i].point_print();
        }
    };
};

//....................................................
//....................................................
class OctNode
{
public:
    OctNode* parent_; 
    OctNode* childs_[8];
    int blockNum_; //номер блока по отношению к parent, 
    Box region_; // размеры куба
    //int level_; // уровень относительно root
    std::vector<struct Triangle> objects_; // треугольники, попавшие в куб.   
public:
    OctNode() {
        region_ = { {-100, -100, -100} , {100, 100, 100}};
    }
    OctNode(OctNode* prnt, int block) : parent_(prnt), blockNum_(block) {
        
        get_region();
        //get_blockNum();    
    }
    //..............................................
    //..............................................
    void get_blockNum() // записывает в blockNum номер, где находится узел относительно parent
    { 
        blockNum_ = (region_.centre_.coord_[0] > parent_->region_.centre_.coord_[0]) << 2 |
                    (region_.centre_.coord_[1] > parent_->region_.centre_.coord_[1]) << 1 | 
                    (region_.centre_.coord_[2] > parent_->region_.centre_.coord_[2]);
    };
    //..............................................
    //..............................................
    void get_region()
    {
        int bit_mask[3] = {7, 2, 1}; // 100b, 010b, 001b.
        auto& prt = parent_->region_;

        for(int i = 0; i < 3; i++) { // порядок x, y ,z

            if(blockNum_ & bit_mask[i]) {
                region_.left_.coord_[i] = ( prt.left_.coord_[i] + prt.right_.coord_[i] ) / 2;
                region_.right_.coord_[i] = prt.right_.coord_[i];
            }

            else {
                region_.left_.coord_[i] = region_.left_.coord_[i];
                region_.right_.coord_[i] = ( prt.left_.coord_[i] + prt.right_.coord_[i] ) / 2;
            }
        }

        region_.sizeSide_ = region_.right_.coord_[0] - region_.left_.coord_[0]; 
        region_.get_centre();
    }
    //..............................................
    //..............................................
    void insert_triangle(Triangle tr) {
    
        if(region_.sizeSide_ < MIN_SIZE ) // если размер области меньше минимальной
        {
            objects_.push_back(tr);
            return; 
        }

        int blockNum = (tr.triangleCentre_.coord_[0] > region_.centre_.coord_[0]) << 2 |
                       (tr.triangleCentre_.coord_[1] > region_.centre_.coord_[1]) << 1 | 
                       (tr.triangleCentre_.coord_[2] > region_.centre_.coord_[2]); // номер блока, куда попал центр треугольника
                       std::cout << blockNum << std::endl;
        // мы будем вставлять именно в этот октант, проверяя что все 3 точки полностью попадают в него.
        // если это не так, прекращаем вставку треугольника
        if( triagle_in_octant(tr, region_) ) { // если треугольник полностью попал в octant, продолжаем вставку
            if (childs_[blockNum] == nullptr) {// мы в самом низу дерева
                childs_[blockNum] = new OctNode{this, blockNum}; // хуйня 
                 std::cout << "eewew\n";
            }
            childs_[blockNum]->insert_triangle(tr);
           
        }
        else
            objects_.push_back(tr);
        return;
    };
    //..............................................
    //..............................................
    bool triagle_in_octant(Triangle tr, Box region) {
        bool result = 1;
        for(int i = 0; i < 3; i++) {
            result = result && tr.pt_[i].point_in_octant(region);
    }
    return result;
    };

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

