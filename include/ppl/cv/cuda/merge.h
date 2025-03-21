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

#ifndef _ST_HPC_PPL3_CV_CUDA_MERGE_H_
#define _ST_HPC_PPL3_CV_CUDA_MERGE_H_

#include "cuda_runtime.h"

#include "ppl/common/retcode.h"

namespace ppl {
namespace cv {
namespace cuda {

/**
 * @brief Combine 3 single-channel images into one 3-channel image.
 * @tparam T The data type, used for both source image and destination image,
 *         currently only uint8_t(uchar) and float are supported.
 * @param stream         cuda stream object.
 * @param height         input&output image's height.
 * @param width          input&output image's width.
 * @param inWidthStride  input image's width stride, it is `width * channels`
 *                       for cudaMalloc() allocated data, `pitch / sizeof(T)`
 *                       for 2D cudaMallocPitch() allocated data.
 * @param inData0        the first single-channel input image data.
 * @param inData1        the second single-channel input image data.
 * @param inData2        the third single-channel input image data.
 * @param outWidthStride the width stride of output image, similar to
 *                       inWidthStride.
 * @param outData        output image data.
 * @return The execution status, succeeds or fails with an error code.
 * @note 1 For best performance, a 2D array allocated by cudaMallocPitch() is
 *         recommended.
 * @warning All parameters must be valid, or undefined behaviour may occur.
 * @remark The fllowing table show which data type and channels are supported.
 * <table>
 * <tr><th>Data type(T)<th>channels
 * <tr><td>uint8_t(uchar)<td>3
 * <tr><td>float<td>3
 * </table>
 * <table>
 * <caption align="left">Requirements</caption>
 * <tr><td>CUDA platforms supported <td>CUDA 7.0
 * <tr><td>Header files <td>#include "ppl/cv/cuda/merge.h";
 * <tr><td>Project      <td>ppl.cv
 * </table>
 * @since ppl.cv-v1.0.0
 * ###Example
 * @code{.cpp}
 * #include "ppl/cv/cuda/merge.h"
 * using namespace ppl::cv::cuda;
 *
 * int main(int argc, char** argv) {
 *   int width    = 640;
 *   int height   = 480;
 *   int channels = 3;
 *
 *   float* dev_input0;
 *   float* dev_input1;
 *   float* dev_input2;
 *   float* dev_output;
 *   size_t input_pitch, output_pitch;
 *   cudaMallocPitch(&dev_input0, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_input1, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_input2, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_output, &output_pitch,
 *                   width * channels * sizeof(float), height);
 *
 *   cudaStream_t stream;
 *   cudaStreamCreate(&stream);
 *   Merge3Channels<float>(stream, height, width, input_pitch / sizeof(float),
 *                         dev_input0, dev_input1, dev_input2,
 *                         output_pitch / sizeof(float), dev_output);
 *   cudaStreamSynchronize(stream);
 *
 *   cudaFree(dev_output);
 *   cudaFree(dev_input0);
 *   cudaFree(dev_input1);
 *   cudaFree(dev_input2);
 *
 *   return 0;
 * }
 * @endcode
 */
template <typename T>
ppl::common::RetCode Merge3Channels(cudaStream_t stream,
                                    int height,
                                    int width,
                                    int inWidthStride,
                                    const T* inData0,
                                    const T* inData1,
                                    const T* inData2,
                                    int outWidthStride,
                                    T* outData);

/**
 * @brief Combine 4 single-channel images into one 4-channel image.
 * @tparam T The data type, used for both source image and destination image,
 *         currently only uint8_t(uchar) and float are supported.
 * @param stream         cuda stream object.
 * @param height         input&output image's height.
 * @param width          input&output image's width.
 * @param inWidthStride  input image's width stride, it is `width * channels`
 *                       for cudaMalloc() allocated data, `pitch / sizeof(T)`
 *                       for 2D cudaMallocPitch() allocated data.
 * @param inData0        the first single-channel input image data.
 * @param inData1        the second single-channel input image data.
 * @param inData2        the third single-channel input image data.
 * @param inData3        the fourth single-channel input image data.
 * @param outWidthStride the width stride of output image, similar to
 *                       inWidthStride.
 * @param outData        output image data.
 * @return The execution status, succeeds or fails with an error code.
 * @note 1 For best performance, a 2D array allocated by cudaMallocPitch() is
 *         recommended.
 * @warning All parameters must be valid, or undefined behaviour may occur.
 * @remark The fllowing table show which data type and channels are supported.
 * <table>
 * <tr><th>Data type(T)<th>channels
 * <tr><td>uint8_t(uchar)<td>4
 * <tr><td>float<td>4
 * </table>
 * <table>
 * <caption align="left">Requirements</caption>
 * <tr><td>CUDA platforms supported <td>CUDA 7.0
 * <tr><td>Header files <td>#include "ppl/cv/cuda/merge.h";
 * <tr><td>Project      <td>ppl.cv
 * </table>
 * @since ppl.cv-v1.0.0
 * ###Example
 * @code{.cpp}
 * #include "ppl/cv/cuda/merge.h"
 * using namespace ppl::cv::cuda;
 *
 * int main(int argc, char** argv) {
 *   int width    = 640;
 *   int height   = 480;
 *   int channels = 4;
 *
 *   float* dev_input0;
 *   float* dev_input1;
 *   float* dev_input2;
 *   float* dev_input3;
 *   float* dev_output;
 *   size_t input_pitch, output_pitch;
 *   cudaMallocPitch(&dev_input0, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_input1, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_input2, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_input3, &input_pitch, width * sizeof(float),
 *                   width * sizeof(float), height);
 *   cudaMallocPitch(&dev_output, &output_pitch,
 *                   width * channels * sizeof(float), height);
 *
 *   cudaStream_t stream;
 *   cudaStreamCreate(&stream);
 *   Merge4Channels<float>(stream, height, width, input_pitch / sizeof(float),
 *                         dev_input0, dev_input1, dev_input2,  dev_input3,
 *                         output_pitch / sizeof(float), dev_output);
 *   cudaStreamSynchronize(stream);
 *
 *   cudaFree(dev_output);
 *   cudaFree(dev_input0);
 *   cudaFree(dev_input1);
 *   cudaFree(dev_input2);
 *   cudaFree(dev_input3);
 *
 *   return 0;
 * }
 * @endcode
 */
template <typename T>
ppl::common::RetCode Merge4Channels(cudaStream_t stream,
                                    int height,
                                    int width,
                                    int inWidthStride,
                                    const T* inData0,
                                    const T* inData1,
                                    const T* inData2,
                                    const T* inData3,
                                    int outWidthStride,
                                    T* outData);

}  // namespace cuda
}  // namespace cv
}  // namespace ppl

#endif  // _ST_HPC_PPL3_CV_CUDA_MERGE_H_
