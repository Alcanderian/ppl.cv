/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include "ppl/cv/cuda/zeros.h"

#include <tuple>
#include <sstream>

#include "opencv2/opencv.hpp"
#include "gtest/gtest.h"

#include "utility.hpp"
#include "infrastructure.hpp"

using namespace ppl::cv;
using namespace ppl::cv::cuda;

using Parameters = std::tuple<cv::Size>;
inline std::string convertToString(const Parameters& parameters) {
  std::ostringstream formatted;

  cv::Size size = std::get<0>(parameters);
  formatted << size.width << "x";
  formatted << size.height;

  return formatted.str();
}

template <typename T, int channels>
class PplCvCudaZerosTest : public ::testing::TestWithParam<Parameters> {
 public:
  PplCvCudaZerosTest() {
    const Parameters& parameters = GetParam();
    size = std::get<0>(parameters);
  }

  ~PplCvCudaZerosTest() {
  }

  bool apply();

 private:
  cv::Size size;
};

template <typename T, int channels>
bool PplCvCudaZerosTest<T, channels>::apply() {
  cv::Mat dst;
  dst = createSourceImage(size.height, size.width,
                          CV_MAKETYPE(cv::DataType<T>::depth, channels));
  cv::Mat cv_dst(size.height, size.width,
                 CV_MAKETYPE(cv::DataType<T>::depth, channels));
  cv::cuda::GpuMat gpu_dst(dst);

  int dst_size = size.height * size.width * channels * sizeof(T);
  T* output = (T*)malloc(dst_size);
  T* gpu_output;
  cudaMalloc((void**)&gpu_output, dst_size);

  cv_dst = cv::Mat::zeros(size.height, size.width,
                          CV_MAKETYPE(cv::DataType<T>::depth, channels));

  Zeros<T, channels>(0, gpu_dst.rows, gpu_dst.cols, gpu_dst.step / sizeof(T),
                     (T*)gpu_dst.data);
  gpu_dst.download(dst);

  Zeros<T, channels>(0, size.height, size.width, size.width * channels,
                     gpu_output);
  cudaMemcpy(output, gpu_output, dst_size, cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  float epsilon;
  if (sizeof(T) == 1) {
    epsilon = EPSILON_1F;
  }
  else {
    epsilon = EPSILON_E6;
  }
  bool identity0 = checkMatricesIdentity<T>(cv_dst, dst, epsilon);
  bool identity1 = checkMatArrayIdentity<T>(cv_dst, output, epsilon);

  free(output);
  cudaFree(gpu_output);

  return (identity0 && identity1);
}

#define UNITTEST(T, channels)                                                  \
using PplCvCudaZerosTest ## T ## channels = PplCvCudaZerosTest<T, channels>;   \
TEST_P(PplCvCudaZerosTest ## T ## channels, Standard) {                        \
  bool identity = this->apply();                                               \
  EXPECT_TRUE(identity);                                                       \
}                                                                              \
                                                                               \
INSTANTIATE_TEST_CASE_P(IsEqual, PplCvCudaZerosTest ## T ## channels,          \
  ::testing::Values(cv::Size{321, 240}, cv::Size{642, 480},                    \
                    cv::Size{1283, 720}, cv::Size{1934, 1080},                 \
                    cv::Size{320, 240}, cv::Size{640, 480},                    \
                    cv::Size{1280, 720}, cv::Size{1920, 1080}),                \
  [](const testing::TestParamInfo<                                             \
      PplCvCudaZerosTest ## T ## channels::ParamType>& info) {                 \
    return convertToString(info.param);                                        \
  }                                                                            \
);

UNITTEST(uchar, 1)
UNITTEST(uchar, 3)
UNITTEST(uchar, 4)
UNITTEST(float, 1)
UNITTEST(float, 3)
UNITTEST(float, 4)
