#pragma once

#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>


struct Box;

struct Point {
    float coord_[3]; // (x, y ,z)

    Point(float x = 0, float y = 0, float z = 0) : coord_{x, y , z} {};
    void point_print() { // correctly
        std::cout << "(" << coord_[0] << "; " << coord_[1] << "; " << coord_[2] << ")" << std::endl;
        return;
    };
    bool point_in_box(Box &region); // {{correctly}}
};

struct Box {
    Point left_; Point right_; 
    Point centre_; // координаты центра куба
    float sizeSide_;

    Box(Point l, Point r) : left_(l), right_(r) {
        get_centre();
        sizeSide_ = right_.coord_[0] - left_.coord_[0];
    }
    Box() {};
    //....................................................
    // this method finds the coordinates of the centre of the box 
    void get_centre() { // {{correctly}}
        for(int i = 0; i < 3; i++)
            centre_.coord_[i] = (left_.coord_[i] + right_.coord_[i]) / 2;
    }
};
//....................................................
//....................................................
bool Point::point_in_box(Box &region) { //{{correctly}}
    bool result = true;
    for(int i = 0; i < 3; i++)
        result = result && ( ( coord_[i] > region.left_.coord_[i]) && (coord_[i] < region.right_.coord_[i]) ); 
    return result;
}

struct Triangle {
    Point pt_[3];
    Point triangleCentre_; // центр треугольника

    Triangle(Point pt1, Point pt2, Point pt3) {
        pt_[0] = pt1; 
        pt_[1] = pt2; 
        pt_[2] = pt3;
        // вычесляем центр треугольника
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                triangleCentre_.coord_[j] += pt_[i].coord_[j] / 3;
            }
        }
    };
    //....................................................
    //....................................................
    void triangle_print() { // {{correctly}}
        for(int i = 0; i < 3; i++) {
            pt_[i].point_print();
        }
    };
    //....................................................
    //....................................................

};

struct Plane {
    float coeff_[4];

    Plane(Point a, Point b, Point c) {
        coeff_[0] = a.coord_[1] * (b.coord_[2] - c.coord_[2]) + 
                    b.coord_[1] * (c.coord_[2] - a.coord_[2]) +
                    c.coord_[1] * (a.coord_[2] - b.coord_[2]);
        
        coeff_[1] = a.coord_[2] * (b.coord_[0] - c.coord_[0]) + 
                    b.coord_[2] * (c.coord_[0] - a.coord_[0]) +
                    c.coord_[2] * (a.coord_[0] - b.coord_[0]);
        
        coeff_[2] = a.coord_[0] * (b.coord_[1] - c.coord_[1]) + 
                    b.coord_[0] * (c.coord_[1] - a.coord_[1]) +
                    c.coord_[0] * (a.coord_[1] - b.coord_[1]);
            
        coeff_[4] = 
        
    }
};