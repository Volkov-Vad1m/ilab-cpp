#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>

#define MIN_SIZE 1;
#define epsilon 10E-6;

struct Point 
{
    float x_, y_, z_;

    Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}
    Point() : x_(0), y_(0), z_(0) {}
    void Print()
    {
        std::cout << "( " << x_ << "; " << y_ << "; " << z_ << ")" << std::endl;
        return;
    };
    //....................................................
    //....................................................
};
//....................................................
//....................................................
struct Triangle
{
    Point pt[3];
    Point triangleCentre_; // центр треугольника

    Triangle(Point pt1, Point pt2, Point pt3)
    {
        triangleCentre_;
        pt[0] = pt1;
        pt[1] = pt2;
        pt[2] = pt3;
        for(int i = 0; i < 3; i++)
        {
            triangleCentre_.x_ += pt[i].x_ / 3;
            triangleCentre_.y_ += pt[i].y_ / 3;
            triangleCentre_.z_ += pt[i].z_ / 3;
        }
    };
};
class OctTree
{
private:
    OctNode* root_; // указатель на корень дерева
    std::vector<float> sizeSide_; // размер граней на каждом уровне
    std::vector<struct Triangle> queue_; // поступающие запросы

public:
    
};
//....................................................
//....................................................
class OctNode
{
public:
    OctNode* parent_; 
    OctNode* childs_[8];
    float region_; // размеры куба
    int blockNum_; //номер блока по отношению к parent, 
    int level_; // уровень относительно root

    struct Point centre_; // координаты центра куба
    std::vector<struct Triangle> objects_; // треугольники, попавшие в куб.

public:
    void get_blockNum() // записывает в blockNum номер, где находится узел относительно parent
    {
        blockNum_ = (centre_.x_ > parent_->centre_.x_) << 2 |
                    (centre_.y_ > parent_->centre_.y_) << 1 | 
                    (centre_.z_ > parent_->centre_.z_);
    };

    void insert_Triangle(Triangle tr, OctNode* cur)
    {
        int blockNum = (tr.triangleCentre_.x_ > cur->centre_.x_) << 2 |
                       (tr.triangleCentre_.y_ > cur->centre_.y_) << 1 | 
                       (tr.triangleCentre_.z_ > cur->centre_.z_); // номер блока, куда попал центр треугольника
        if(region_ < MIN_SIZE) 
            return;
        
        return;
        insert_Triangle(tr, cur->childs_[blockNum]);
    };
};