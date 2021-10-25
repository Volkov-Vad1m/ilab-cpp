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
    float x_, y_, z_;

    Point(float x = 0, float y = 0, float z = 0) : x_(x), y_(y), z_(z) {}

    void point_print() {
        std::cout << "(" << x_ << "; " << y_ << "; " << z_ << ")" << std::endl;
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
        sizeSide_ = right_.x_ - left_.x_;
    }
    Box() {};

    void get_centre()
    {
        centre_.x_ = (left_.x_ + right_.x_) / 2; 
        centre_.y_ = (left_.y_ + right_.y_) / 2;
        centre_.z_ = (left_.z_ + right_.z_) / 2;
    }
};
//....................................................
//....................................................
bool Point::point_in_octant(Box &region) {
        return (x_ > region.left_.x_) && (x_ < region.right_.x_) &&
               (y_ > region.left_.y_) && (y_ < region.right_.y_) &&
               (z_ > region.left_.z_) && (z_ < region.right_.z_)  ;
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
            triangleCentre_.x_ += pt_[i].x_ / 3;
            triangleCentre_.y_ += pt_[i].y_ / 3;
            triangleCentre_.z_ += pt_[i].z_ / 3;
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
        blockNum_ = (region_.centre_.x_ > parent_->region_.centre_.x_) << 2 |
                    (region_.centre_.y_ > parent_->region_.centre_.y_) << 1 | 
                    (region_.centre_.z_ > parent_->region_.centre_.z_);
    };
    //..............................................
    //..............................................
    void get_region()
    {
        auto& prt = parent_->region_;
        if (blockNum_ & 7) {

            region_.left_.x_ = ( prt.left_.x_ + prt.right_.x_ ) / 2;
            region_.right_.x_ = prt.right_.x_;
        }
        else {
            region_.left_.x_ = region_.left_.x_;
            region_.right_.x_ = ( prt.left_.x_ + prt.right_.x_ ) / 2;
        }

        if (blockNum_ & 2) {

            region_.left_.y_ = ( prt.left_.y_ + prt.right_.y_ ) / 2;
            region_.right_.y_ = prt.right_.y_;
        }
        else {
            region_.left_.y_ = region_.left_.y_;
            region_.right_.y_ = ( prt.left_.y_ + prt.right_.y_ ) / 2;
        }

        if (blockNum_ & 1) {

            region_.left_.z_ = ( prt.left_.z_ + prt.right_.z_ ) / 2;
            region_.right_.z_ = prt.right_.z_;
        }
        else {
            region_.left_.z_ = region_.left_.z_;
            region_.right_.z_ = ( prt.left_.z_ + prt.right_.z_ ) / 2;
        }
        region_.sizeSide_ = region_.right_.x_ - region_.left_.x_; 
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

        int blockNum = (tr.triangleCentre_.x_ > region_.centre_.x_) << 2 |
                       (tr.triangleCentre_.y_ > region_.centre_.y_) << 1 | 
                       (tr.triangleCentre_.z_ > region_.centre_.z_); // номер блока, куда попал центр треугольника
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

