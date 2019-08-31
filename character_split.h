#ifndef CHARACTER_SPLIT_H
#define CHARACTER_SPLIT_H

#include <opencv2/opencv.hpp>

class character_split
{
public:
    character_split();
    std::vector<cv::Mat> chara_split(cv::Mat srcImg);
};

#endif // CHARACTER_SPLIT_H
