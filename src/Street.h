#ifndef STREET_H
#define STREET_H

#include "TrafficObject.h"

class Intersection;

class Street: public TrafficObject, public std::enable_shared_from_this<Street>
{
    Street();

    double getLength() { return _length;}
    void setIntersection(std::shared_ptr<Intersection> in);
    void setOutIntersection(std::shared_ptr<Intersection> out);
    std::shared_ptr<Intersection> getOutIntersection(){ return _interOut};
    std::shared_ptr<Intersection> getInIntersection(){ return _interIn};

    std::shared_ptr<Street> get_shared_this(){ return shared_from_this();}
    



    private:
    double _length;
    std::shared_ptr<Intersection> _interIn, _interOut;

};
