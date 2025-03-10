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

#ifndef _ST_HPC_PPL3_CV_CUDA_RESIZE_H_
#define _ST_HPC_PPL3_CV_CUDA_RESIZE_H_

#include <cuda_runtime.h>

#include "ppl/common/retcode.h"

namespace ppl {
namespace cv {
namespace cuda {

/**
 * @brief Scale the image with the bilinear interpolation method.
 * @tparam T The data type of input and output image, currently only \a
 *         uint8_t(uchar) and \a float are supported.
 * @tparam channels The number of channels of input image, 1, 3 and 4 are
 *         supported.
 * @param stream           cuda stream object.
 * @param inHeight         input image's height.
 * @param inWidth          input image's width need to be processed.
 * @param inWidthStride    input image's width stride, it is `width * channels`
 *                         for cudaMalloc() allocated data, `pitch / sizeof(T)`
 *                         for 2D cudaMallocPitch() allocated data.
 * @param inData           input image data.
 * @param outHeight        output image's height.
 * @param outWidth         output image's width need to be processed.
 * @param outWidthStride   the width stride of output image, similar to
 *                         inWidthStride.
 * @param outData          output image data.
 * @return The execution status, succeeds or fails with an error code.
 * @warning All input parameters must be valid, or undefined behaviour may occur.
 * @remark The fllowing table show which data type and channels are supported.
 * <table>
 * <tr><th>Data type(T)<th>channels
 * <tr><td>uint8_t(uchar)<td>1
 * <tr><td>uint8_t(uchar)<td>3
 * <tr><td>uint8_t(uchar)<td>4
 * <tr><td>float<td>1
 * <tr><td>float<td>3
 * <tr><td>float<td>4
 * </table>
 * <table>
 * <caption align="left">Requirements</caption>
 * <tr><td>CUDA platforms supported <td>CUDA 7.0
 * <tr><td>Header files  <td> #include "ppl/cv/cuda/resize.h"
 * <tr><td>Project       <td> ppl.cv
 * </table>
 * @since ppl.cv-v1.0.0
 * ###Example
 * @code{.cpp}
 * #include "ppl/cv/cuda/resize.h"
 * using namespace ppl::cv::cuda;
 *
 * int main(int argc, char** argv) {
 *   int src_width  = 320;
 *   int src_height = 240;
 *   int dst_width  = 640;
 *   int dst_height = 480;
 *   int channels = 3;
 *
 *   float* dev_input;
 *   float* dev_output;
 *   size_t input_pitch, output_pitch;
 *   cudaMallocPitch(&dev_input, &input_pitch,
 *                   width * channels * sizeof(float), height);
 *   cudaMallocPitch(&dev_output, &output_pitch,
 *                   width * channels * sizeof(float), height);
 *
 *   cudaStream_t stream;
 *   cudaStreamCreate(&stream);
 *   ResizeLinear<float, 3>(stream, src_height, src_width,
 *                          input_pitch / sizeof(float), dev_input,
 *                          dst_height, dst_width, output_pitch / sizeof(float),
 *                          dev_output);
 *   cudaStreamSynchronize(stream);
 *
 *   cudaFree(dev_input);
 *   cudaFree(dev_output);
 *
 *   return 0;
 * }
 * @endcode
 */
template <typename T, int channels>
ppl::common::RetCode ResizeLinear(cudaStream_t stream,
                                  int inHeight,
                                  int inWidth,
                                  int inWidthStride,
                                  const T* inData,
                                  int outHeight,
                                  int outWidth,
                                  int outWidthStride,
                                  T* outData);

/**
 * @brief Scale the image with the nearest neighbor interpolation method.
 * @tparam T The data type of input and output image, currently only \a
 *         uint8_t(uchar) and \a float are supported.
 * @tparam channels The number of channels of input image, 1, 3 and 4 are
 *         supported.
 * @param stream           cuda stream object.
 * @param inHeight         input image's height.
 * @param inWidth          input image's width need to be processed.
 * @param inWidthStride    input image's width stride, it is `width * channels`
 *                         for cudaMalloc() allocated data, `pitch / sizeof(T)`
 *                         for 2D cudaMallocPitch() allocated data.
 * @param inData           input image data.
 * @param outHeight        output image's height.
 * @param outWidth         output image's width need to be processed.
 * @param outWidthStride   the width stride of output image, similar to
 *                         inWidthStride.
 * @param outData          output image data.
 * @return The execution status, succeeds or fails with an error code.
 * @warning All input parameters must be valid, or undefined behaviour may occur.
 * @remark The fllowing table show which data type and channels are supported.
 * <table>
 * <tr><th>Data type(T)<th>channels
 * <tr><td>uint8_t(uchar)<td>1
 * <tr><td>uint8_t(uchar)<td>3
 * <tr><td>uint8_t(uchar)<td>4
 * <tr><td>float<td>1
 * <tr><td>float<td>3
 * <tr><td>float<td>4
 * </table>
 * <table>
 * <caption align="left">Requirements</caption>
 * <tr><td>CUDA platforms supported <td>CUDA 7.0
 * <tr><td>Header files  <td> #include "ppl/cv/cuda/resize.h"
 * <tr><td>Project       <td> ppl.cv
 * </table>
 * @since ppl.cv-v1.0.0
 * ###Example
 * @code{.cpp}
 * #include "ppl/cv/cuda/resize.h"
 * using namespace ppl::cv::cuda;
 *
 * int main(int argc, char** argv) {
 *   int src_width  = 320;
 *   int src_height = 240;
 *   int dst_width  = 640;
 *   int dst_height = 480;
 *   int channels = 3;
 *
 *   float* dev_input;
 *   float* dev_output;
 *   size_t input_pitch, output_pitch;
 *   cudaMallocPitch(&dev_input, &input_pitch,
 *                   width * channels * sizeof(float), height);
 *   cudaMallocPitch(&dev_output, &output_pitch,
 *                   width * channels * sizeof(float), height);
 *
 *   cudaStream_t stream;
 *   cudaStreamCreate(&stream);
 *   ResizeNearestPoint<float, 3>(stream, src_height, src_width,
 *                                input_pitch / sizeof(float), dev_input,
 *                                dst_height, dst_width,
 *                                output_pitch / sizeof(float), dev_output);
 *   cudaStreamSynchronize(stream);
 *
 *   cudaFree(dev_input);
 *   cudaFree(dev_output);
 *
 *   return 0;
 * }
 * @endcode
 */
template <typename T, int channels>
ppl::common::RetCode ResizeNearestPoint(cudaStream_t stream,
                                        int inHeight,
                                        int inWidth,
                                        int inWidthStride,
                                        const T* inData,
                                        int outHeight,
                                        int outWidth,
                                        int outWidthStride,
                                        T* outData);

/**
 * @brief Scale the image with the pixel area interpolation method.
 * @tparam T The data type of input and output image, currently only \a
 *         uint8_t(uchar) and \a float are supported.
 * @tparam channels The number of channels of input image, 1, 3 and 4 are
 *         supported.
 * @param stream           cuda stream object.
 * @param inHeight         input image's height.
 * @param inWidth          input image's width need to be processed.
 * @param inWidthStride    input image's width stride, it is `width * channels`
 *                         for cudaMalloc() allocated data, `pitch / sizeof(T)`
 *                         for 2D cudaMallocPitch() allocated data.
 * @param inData           input image data.
 * @param outHeight        output image's height.
 * @param outWidth         output image's width need to be processed.
 * @param outWidthStride   the width stride of output image, similar to
 *                         inWidthStride.
 * @param outData          output image data.
 * @return The execution status, succeeds or fails with an error code.
 * @warning All input parameters must be valid, or undefined behaviour may occur.
 * @remark The fllowing table show which data type and channels are supported.
 * <table>
 * <tr><th>Data type(T)<th>channels
 * <tr><td>uint8_t(uchar)<td>1
 * <tr><td>uint8_t(uchar)<td>3
 * <tr><td>uint8_t(uchar)<td>4
 * <tr><td>float<td>1
 * <tr><td>float<td>3
 * <tr><td>float<td>4
 * </table>
 * <table>
 * <caption align="left">Requirements</caption>
 * <tr><td>CUDA platforms supported <td>CUDA 7.0
 * <tr><td>Header files  <td> #include "ppl/cv/cuda/resize.h"
 * <tr><td>Project       <td> ppl.cv
 * </table>
 * @since ppl.cv-v1.0.0
 * ###Example
 * @code{.cpp}
 * #include "ppl/cv/cuda/resize.h"
 * using namespace ppl::cv::cuda;
 *
 * int main(int argc, char** argv) {
 *   int src_width  = 320;
 *   int src_height = 240;
 *   int dst_width  = 640;
 *   int dst_height = 480;
 *   int channels = 3;
 *
 *   float* dev_input;
 *   float* dev_output;
 *   size_t input_pitch, output_pitch;
 *   cudaMallocPitch(&dev_input, &input_pitch,
 *                   width * channels * sizeof(float), height);
 *   cudaMallocPitch(&dev_output, &output_pitch,
 *                   width * channels * sizeof(float), height);
 *
 *   cudaStream_t stream;
 *   cudaStreamCreate(&stream);
 *   ResizeArea<float, 3>(stream, src_height, src_width,
 *                        input_pitch / sizeof(float), dev_input,
 *                        dst_height, dst_width, output_pitch / sizeof(float),
 *                        dev_output);
 *   cudaStreamSynchronize(stream);
 *
 *   cudaFree(dev_input);
 *   cudaFree(dev_output);
 *
 *   return 0;
 * }
 * @endcode
 */
template <typename T, int channels>
ppl::common::RetCode ResizeArea(cudaStream_t stream,
                                int inHeight,
                                int inWidth,
                                int inWidthStride,
                                const T* inData,
                                int outHeight,
                                int outWidth,
                                int outWidthStride,
                                T* outData);

}  // namespace cuda
}  // namespace cv
}  // namespace ppl

#endif  // _ST_HPC_PPL3_CV_CUDA_RESIZE_H_
