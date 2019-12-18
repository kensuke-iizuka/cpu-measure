#include <iostream>
#include <array>
#include <chrono>

#include "weights/conv2_w_0.h"
#include "weights/conv2_b_0.h"

const int OUTCH = 256;
const int OUTH = 27;
const int OUTW = 27;
const int INCH = 96;
const int INH = 27;
const int INW = 27;
const int K = 5;
const int PAD = 2;
const int ST = 1;
const int GROUP = 2;
const int PINH = INH+2*PAD;
const int PINW = INW+2*PAD;

template <class data_T> 
// void part_conv(std::array<data_T, INCH*PINH*PINW/GROUP> input,
//           std::array<data_T, OUTH*OUTW> output,
//           data_T weight[INCH][K][K],
//           data_T bias)
void part_conv(data_T input[INCH][PINH][PINW],
               data_T output[OUTH][OUTW],
               data_T weight[INCH][K][K],
               data_T bias)
{
  for (size_t y = 0; y < OUTH; y++) {
    for (size_t x = 0; x < OUTW; x++) {
      output[y][x] = bias;
      for (size_t ich = 0; ich < INCH/GROUP; ich++) {
        for (size_t ky = 0; ky < K; ky++) {
          for (size_t kx = 0; kx < K; kx++) {
            // output[y*OUTW+x] += weight[ich][ky][kx] * input[ich][ST*y+ky][ST*x+kx];
            output[y][x] += weight[ich][ky][kx] * input[ich][ST*y+ky][ST*x+kx];
          }
        }
      }
    }
  }
}


int main()
{
  // std::array<std::array<float, INCH*INH*INW/GROUP>, GROUP> input;
  // std::array<std::array<float, INCH*(PINH*PINW)/GROUP>, GROUP> pad_input;
  // std::array<std::array<float, OUTH*OUTW>, OUTCH> output;

  float input[GROUP][INCH/GROUP][INH][INW];
  float pad_input[GROUP][INCH/GROUP][PINH][PINW];
  float output[OUTCH][OUTH][OUTW];
  // for(auto it = std::begin(); it != std::end(arr); ++it){
  //   std::cout << *it << std::endl;
  // }

  // Initialize and Padding input data
  for (size_t g=0; g<GROUP; g++) {
    for (size_t icht=0; icht<INCH/GROUP; icht++) {
      for (size_t y=0; y<PINH; y++) {
        for (size_t x=0; x<PINW; x++) {
          if (y<PAD || y>PINH-PAD-1) {
            // input[g][icht*PINH*PINW+y*PINW+x] = 0.0f;
            pad_input[g][icht][y][x] = 0.0f;
          }
          else if (x<PAD || x>PINW-PAD-1) {
            pad_input[g][icht][y][x] = 0.0f;
          } else {
            pad_input[g][icht][y][x] = 1.0f;
          }
        }
      }
    }
  }
  

  auto start = std::chrono::system_clock::now();
  // Group1
  for (size_t och1=0; och1<OUTCH/GROUP; och1++) {
    part_conv<float>(pad_input[0], output[och1], conv2_w_0[och1], conv2_b_0[och1]);
  }

  // Group2
  for (size_t och2=OUTCH/GROUP; och2<OUTCH; och2++) {
    part_conv<float>(pad_input[1], output[och2], conv2_w_0[och2], conv2_b_0[och2]);
  }

  auto end = std::chrono::system_clock::now();
  double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  std::cout << "Elapsed time: " << elapsed << "[ms]" << std::endl;

  // print conv2 result
  // for (size_t och=0; och<OUTCH; och++) {
  //   for (size_t y=0; y<OUTH; y++) {
  //     for (size_t x=0; x<OUTW; x++) {
  //     std::cout << output[och][y][x] << std::endl;
  //     }
  //   }
  // }

  return 0;
}
