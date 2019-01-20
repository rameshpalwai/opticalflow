#include<iostream>
#include<stdint.h>
#include<chrono>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "census_transform.h"

using namespace cv;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, const char** argv){


// load images
    Mat img_bgr_1 = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    Mat img_bgr_2 = imread(argv[2],CV_LOAD_IMAGE_COLOR);


// Check if images has been loaded correctly
    if(img_bgr_1.empty()){

        cout<<"Error: Image_1 has not been loaded ..."<<endl;
        return 0;
    }

    if(img_bgr_2.empty()){

        cout<<"Error: Image_2 has not been loaded ..."<<endl;
        return 0;
    }


// display original color images
    namedWindow("Position 1",CV_WINDOW_AUTOSIZE);
    namedWindow("Position 2",CV_WINDOW_AUTOSIZE);

    imshow("Position 1",img_bgr_1);
    imshow("Position 2",img_bgr_2);



// convert color image to gray scale image
    Mat img_gray_1;
    cvtColor(img_bgr_1,img_gray_1,CV_BGR2GRAY);

    Mat img_gray_2;
    cvtColor(img_bgr_2,img_gray_2,CV_BGR2GRAY);


    cout<<"length of img array is "<<img_gray_1.size<<"\n";
    cout<<"no. of rows "<<img_gray_1.rows<<"\n";
    cout<<"no. of cols "<<img_gray_1.cols<<endl;



    CT frame1(img_gray_1.rows,img_gray_1.cols);
    CT frame2(img_gray_2.rows,img_gray_2.cols);

    uint16_t star_pattern_size = std::stoi(argv[3]);
    uint16_t cell_size = std::stoi(argv[4]);


 // start measuring time
 auto begin = std::chrono::high_resolution_clock::now(); 


    frame1.find_signature_vector_star(img_gray_1, star_pattern_size);
    frame2.find_signature_vector_star(img_gray_2, star_pattern_size);

    uint32_t* displacement = frame1.transition_towards_star(frame2, cell_size);


 // calculate elapsed time
 auto end = std::chrono::high_resolution_clock::now();    
 auto dur = end - begin;
 float ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
 
 cout << "Total time taken by CPU : " << ms <<" msec"<< endl;



//----------------------------------------- Draw Vectors -------------------------------------
    uint32_t x_cordinate, y_cordinate;

    for(uint16_t i_1=0; i_1 < img_bgr_1.rows; ++i_1){
    for(uint16_t j_1=0; j_1< img_bgr_1.cols; ++j_1){

        if(displacement[i_1*img_bgr_1.cols+j_1] != 0){

            x_cordinate = (displacement[i_1*img_bgr_1.cols+j_1] >> 16);
            y_cordinate = (displacement[i_1*img_bgr_1.cols+j_1] & 0x0000FFFF);

            line(img_bgr_1, Point(j_1,i_1), Point(x_cordinate,y_cordinate), Scalar(10,50,200));
        }

    }
    }
//---------------------------------------------------------------------------------------------


    namedWindow("Final",CV_WINDOW_AUTOSIZE);
    imshow("Final",img_bgr_1);

    waitKey(0);
    destroyAllWindows();

    delete displacement;
    return 0;
}
