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

#include "ppl/cv/cuda/boxfilter.h"

#include <tuple>
#include <sstream>

#include "opencv2/opencv.hpp"
#include "gtest/gtest.h"

#include "infrastructure.hpp"

using namespace ppl::cv;
using namespace ppl::cv::cuda;

using Parameters = std::tuple<int, int, bool, BorderType, cv::Size>;
inline std::string convertToStringBoxFilter(const Parameters& parameters) {
  std::ostringstream formatted;

  int ksize_x = std::get<0>(parameters);
  formatted << "Ksize_x" << ksize_x << "_";

  int ksize_y = std::get<1>(parameters);
  formatted << "Ksize_y" << ksize_y << "_";

  bool normalize = std::get<2>(parameters);
  formatted << "Normalize" << normalize << "_";

  BorderType border_type = (BorderType)std::get<3>(parameters);
  if (border_type == BORDER_TYPE_REPLICATE) {
    formatted << "BORDER_REPLICATE" << "_";
  }
  else if (border_type == BORDER_TYPE_REFLECT) {
    formatted << "BORDER_REFLECT" << "_";
  }
  else if (border_type == BORDER_TYPE_REFLECT_101) {
    formatted << "BORDER_REFLECT_101" << "_";
  }
  else {  // border_type == BORDER_TYPE_DEFAULT
    formatted << "BORDER_DEFAULT" << "_";
  }

  cv::Size size = std::get<4>(parameters);
  formatted << size.width << "x";
  formatted << size.height;

  return formatted.str();
}

template <typename T, int channels>
class PplCvCudaBoxFilterTest : public ::testing::TestWithParam<Parameters> {
 public:
  PplCvCudaBoxFilterTest() {
    const Parameters& parameters = GetParam();
    ksize_x     = std::get<0>(parameters);
    ksize_y     = std::get<1>(parameters);
    normalize   = std::get<2>(parameters);
    border_type = std::get<3>(parameters);
    size        = std::get<4>(parameters);
  }

  ~PplCvCudaBoxFilterTest() {
  }

  bool apply();

 private:
  int ksize_x;
  int ksize_y;
  bool normalize;
  BorderType border_type;
  cv::Size size;
};

template <typename T, int channels>
bool PplCvCudaBoxFilterTest<T, channels>::apply() {
  cv::Mat src;
  src = createSourceImage(size.height, size.width,
                          CV_MAKETYPE(cv::DataType<T>::depth, channels));
  cv::Mat dst(size.height, size.width,
              CV_MAKETYPE(cv::DataType<T>::depth, channels));
  cv::Mat cv_dst(size.height, size.width,
                 CV_MAKETYPE(cv::DataType<T>::depth, channels));
  cv::cuda::GpuMat gpu_src(src);
  cv::cuda::GpuMat gpu_dst(dst);

  int src_size = size.height * size.width * channels * sizeof(T);
  T* input  = (T*)malloc(src_size);
  T* output = (T*)malloc(src_size);
  T* gpu_input;
  T* gpu_output;
  cudaMalloc((void**)&gpu_input, src_size);
  cudaMalloc((void**)&gpu_output, src_size);
  copyMatToArray(src, input);
  cudaMemcpy(gpu_input, input, src_size, cudaMemcpyHostToDevice);

  cv::BorderTypes cv_border = cv::BORDER_DEFAULT;
  if (border_type == BORDER_TYPE_REPLICATE) {
    cv_border = cv::BORDER_REPLICATE;
  }
  else if (border_type == BORDER_TYPE_REFLECT) {
    cv_border = cv::BORDER_REFLECT;
  }
  else if (border_type == BORDER_TYPE_REFLECT_101) {
    cv_border = cv::BORDER_REFLECT_101;
  }
  else {
  }
  cv::boxFilter(src, cv_dst, cv_dst.depth(), cv::Size(ksize_x, ksize_y),
                cv::Point(-1, -1), normalize, cv_border);

  BoxFilter<T, channels>(0, gpu_src.rows, gpu_src.cols,
      gpu_src.step / sizeof(T), (T*)gpu_src.data, ksize_x, ksize_y, normalize,
      gpu_dst.step / sizeof(T), (T*)gpu_dst.data, border_type);
  gpu_dst.download(dst);

  BoxFilter<T, channels>(0, size.height, size.width, size.width * channels,
      gpu_input, ksize_x, ksize_y, normalize, size.width * channels, gpu_output,
      border_type);
  cudaMemcpy(output, gpu_output, src_size, cudaMemcpyDeviceToHost);

  float epsilon;
  if (sizeof(T) == 1) {
    epsilon = EPSILON_1F;
  }
  else {
    epsilon = EPSILON_E1;
  }
  bool identity0 = checkMatricesIdentity<T>(cv_dst, dst, epsilon);
  bool identity1 = checkMatArrayIdentity<T>(cv_dst, output, epsilon);

  free(input);
  free(output);
  cudaFree(gpu_input);
  cudaFree(gpu_output);

  return (identity0 && identity1);
}

#define UNITTEST(T, channels)                                                  \
using PplCvCudaBoxFilterTest ## T ## channels =                                \
        PplCvCudaBoxFilterTest<T, channels>;                                   \
TEST_P(PplCvCudaBoxFilterTest ## T ## channels, Standard) {                    \
  bool identity = this->apply();                                               \
  EXPECT_TRUE(identity);                                                       \
}                                                                              \
                                                                               \
INSTANTIATE_TEST_CASE_P(IsEqual, PplCvCudaBoxFilterTest ## T ## channels,      \
  ::testing::Combine(                                                          \
    ::testing::Values(1, 5, 17, 24, 43),                                       \
    ::testing::Values(1, 4, 17, 31, 44),                                       \
    ::testing::Values(true, false),                                            \
    ::testing::Values(BORDER_TYPE_REPLICATE, BORDER_TYPE_REFLECT,              \
                      BORDER_TYPE_REFLECT_101),                                \
    ::testing::Values(cv::Size{321, 240}, cv::Size{642, 480},                  \
                      cv::Size{1283, 720}, cv::Size{1934, 1080},               \
                      cv::Size{320, 240}, cv::Size{640, 480},                  \
                      cv::Size{1280, 720}, cv::Size{1920, 1080})),             \
  [](const testing::TestParamInfo<                                             \
      PplCvCudaBoxFilterTest ## T ## channels::ParamType>& info) {             \
    return convertToStringBoxFilter(info.param);                               \
  }                                                                            \
);

UNITTEST(uchar, 1)
UNITTEST(uchar, 3)
UNITTEST(uchar, 4)
UNITTEST(float, 1)
UNITTEST(float, 3)
UNITTEST(float, 4)
