#ifndef CENSUS_TRANSFORM_H
#define CENSUS_TRANSFORM_H

#include<iostream>
#include<stdint.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<vector>


class CT{

private:

    uint16_t n_rows;
    uint16_t n_cols;
    uint8_t* signature_vector;
    uint16_t n_bytes;

public:

    CT(uint16_t rows, uint16_t cols);
    ~CT();


    void find_signature_vector_star(cv::Mat& img, uint16_t N);

    uint32_t* transition_towards_star(CT& frame_2, uint16_t Cell_size);

};

#endif // CT_H

