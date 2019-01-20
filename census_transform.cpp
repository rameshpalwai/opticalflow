#include "census_transform.h"

CT::CT(uint16_t rows, uint16_t cols){

    n_rows = rows;
    n_cols = cols;

}


CT::~CT(){

    delete signature_vector;

}



void CT::find_signature_vector_star(cv::Mat& img, uint16_t N){

    n_bytes = (N-1)>>1;
    signature_vector = new uint8_t[(n_rows*n_cols)*n_bytes];

 // initialize with zero
    for(unsigned int i=0; i < (n_rows*n_cols)*n_bytes; ++i){

        signature_vector[i] = 0;

    }


    uint8_t center, neighbour, bit, layer;
    uint16_t count, internal;

// iterate trough every pixel of gray scale image
    for(uint16_t i=0; i < n_rows; ++i){
        for(uint16_t j=0; j< n_cols; ++j){


         // get the value of center pixel of census transform window and reset count
            center = img.at<uint8_t>(i,j);
            count = 0;
            bit = 0;


         // iterate through pixels in census transform window in star pattern
        //-----------------------------------------------------------------------------------------------------------
            for(uint16_t ii= (i-n_bytes); ii <= (i+n_bytes); ++ii){

                if(ii<n_rows){
                    layer = abs(i-ii);

                    if( layer != 0){
                    for(uint16_t jj= (j-layer); jj <= (j+n_bytes); jj+=layer){
                     // eliminate indices that are out of bound
                        if( jj < n_cols ){
                            // compare with neighbour pixels. Set bit if value of center pixel is smaller
                               neighbour = img.at<uint8_t>(ii,jj);
                               if((center+40) < neighbour){
                                   internal = count>>3; // divide by 8
                                   signature_vector[i*(n_cols*n_bytes) + (j*n_bytes) + internal] = (signature_vector[i*(n_cols*n_bytes) + (j*n_bytes) + internal] | (1 << bit));
                               }
                        }
                        count++;
                        (bit<7) ? ++bit : bit=0;
                    }
                    }
                    else{
                    for(uint16_t jj= (j-n_bytes); jj<(j+n_bytes); ++jj){
                     // eliminate indices that are out of bound and the selfindex
                        if( (jj < n_cols) && ((ii != i)||(jj != j)) ){
                            // compare with neighbour pixels. Set bit if value of center pixel is smaller
                               neighbour = img.at<uint8_t>(ii,jj);
                               if((center+40) < neighbour){
                                   internal = count>>3; // divide by 8
                                   signature_vector[i*(n_cols*n_bytes) + (j*n_bytes) + internal] = (signature_vector[i*(n_cols*n_bytes) + (j*n_bytes) + internal] | (1 << bit));
                               }
                        }
                        count++;
                        (bit<7) ? ++bit : bit=0;
                    }
                    }

            }
            }
        //--------------------------------------------------------------------------------------------------------------
        }
    }

}







uint32_t* CT::transition_towards_star(CT& frame_2, uint16_t Cell_size){


   // uint32_t nopair_count=0, morepairs=0, exact_pairs=0;


  //define vector to carry x- and y- coordinates of displaced pixel
    uint32_t* displacement_vec = new uint32_t[n_rows*n_cols];
    for(unsigned int i=0; i < (n_rows*n_cols); ++i){

        displacement_vec[i] = 0;

    }

  //set cell size
    uint16_t n = Cell_size>>1;
    std::cout<<"Actual Cell size is: "<<(2*n+1)<<" X "<<(2*n+1)<<std::endl;

    uint16_t u_nibble=0, l_nibble=0;
    uint8_t hamming_distance = 255;
    int n_pairs;
    uint16_t I_1,I_2,J_1,J_2,N_cols,count;

    for(uint16_t i_1=0; i_1 < n_rows; ++i_1){
    for(uint16_t j_1=0; j_1< n_cols; ++j_1){

        n_pairs = 0;

     // iterate through all pixels in NxN window
        for(uint16_t i_2= (i_1-n); i_2 <= (i_1+n); ++i_2){
        for(uint16_t j_2= (j_1-n); j_2 <= (j_1+n); ++j_2){

         // eliminate indices that are out of bound
            if( i_2 < n_rows && j_2 < n_cols ){

                I_1 = i_1;
                I_2 = i_2;
                J_1 = j_1*n_bytes;
                J_2 = j_2*n_bytes;
       		
                N_cols = n_cols*n_bytes;
                count = 0;

                for(uint8_t internal=0; internal<n_bytes; ++internal){
              //find hamming distance of pixels of frame_1 w.r.t pixels of frame_2
                //hamming_distance = this->signature_vector[i_1*n_cols+j_1] ^ frame_2.signature_vector[i_2*n_cols+j_2];
                hamming_distance = this->signature_vector[I_1*N_cols+J_1+internal] ^ frame_2.signature_vector[I_2*N_cols+J_2+internal];
                if(hamming_distance == 0){ count++; }
		else {break;}
                }

              //select pairs having zero hamming distance
                if(count == n_bytes){

                    u_nibble = j_2; // x-cordinate of displaced pixel
                    l_nibble = i_2; // y-cordinate of displaced pixel
                    n_pairs++;
                }

            }

        }
        }


       /* if(n_pairs == 0){
            nopair_count++;
        }
        if(n_pairs > 1){
            morepairs++;
        }
        if(n_pairs == 1){
            exact_pairs++;
        }*/

      //save displacement of pixels forming single pair
        if(n_pairs == 1){

            // eliminate self index indicating zero displacement
               if( (u_nibble != j_1) || (l_nibble != i_1) ){
               displacement_vec[i_1*n_cols+j_1] = u_nibble;
               displacement_vec[i_1*n_cols+j_1] = (displacement_vec[i_1*n_cols+j_1] << 16) | (l_nibble);
               }
        }

    }
    }

   /* std::cout<<"No. of no pairs : "<<nopair_count<<std::endl;
    std::cout<<"No. of more than 1 pair : "<<morepairs<<std::endl;
    std::cout<<"No. of exactly 1 pair : "<<exact_pairs<<std::endl;*/
    
    return displacement_vec;

}
    
