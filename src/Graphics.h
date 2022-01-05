#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "TrafficObject.h"

class Graphics
{

public:
    void setBGFilename(std::string filename){ _bgFilename= filename;}
    void setTrafficOBjects(std::vector<std::shared_ptrLTrafficObject>> &trafficObjects) {_trafficObjects = trafficObjects;}
    void simulate();

private:
    std::vector<std::shared_ptr<TrafficObject>> _trafficObjects;
    std::string _bgFilename;
    std::string _windowName;
    std::vector<cv::Mat> _images;

};


#endif

