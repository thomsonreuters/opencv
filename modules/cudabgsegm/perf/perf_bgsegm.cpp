/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "perf_precomp.hpp"

#ifdef HAVE_OPENCV_CUDAIMGPROC
#  include "opencv2/cudaimgproc.hpp"
#endif

using namespace std;
using namespace testing;
using namespace perf;

#if defined(HAVE_XINE)         || \
    defined(HAVE_GSTREAMER)    || \
    defined(HAVE_QUICKTIME)    || \
    defined(HAVE_QTKIT)        || \
    defined(HAVE_AVFOUNDATION) || \
    defined(HAVE_FFMPEG)       || \
    defined(WIN32) /* assume that we have ffmpeg */

#  define BUILD_WITH_VIDEO_INPUT_SUPPORT 1
#else
#  define BUILD_WITH_VIDEO_INPUT_SUPPORT 0
#endif

//////////////////////////////////////////////////////
// FGDStatModel

#if BUILD_WITH_VIDEO_INPUT_SUPPORT

DEF_PARAM_TEST_1(Video, string);

PERF_TEST_P(Video, FGDStatModel,
            Values(string("gpu/video/768x576.avi")))
{
    const int numIters = 10;

    declare.time(60);

    const string inputFile = perf::TestBase::getDataPath(GetParam());

    cv::VideoCapture cap(inputFile);
    ASSERT_TRUE(cap.isOpened());

    cv::Mat frame;
    cap >> frame;
    ASSERT_FALSE(frame.empty());

    if (PERF_RUN_CUDA())
    {
        cv::cuda::GpuMat d_frame(frame), foreground;

        cv::Ptr<cv::cuda::BackgroundSubtractorFGD> d_fgd = cv::cuda::createBackgroundSubtractorFGD();
        d_fgd->apply(d_frame, foreground);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            d_frame.upload(frame);

            startTimer();
            if(!next())
                break;

            d_fgd->apply(d_frame, foreground);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            d_frame.upload(frame);

            d_fgd->apply(d_frame, foreground);
        }

        CUDA_SANITY_CHECK(foreground, 1e-2, ERROR_RELATIVE);

#ifdef HAVE_OPENCV_CUDAIMGPROC
        cv::cuda::GpuMat background3, background;
        d_fgd->getBackgroundImage(background3);
        cv::cuda::cvtColor(background3, background, cv::COLOR_BGR2BGRA);
        CUDA_SANITY_CHECK(background, 1e-2, ERROR_RELATIVE);
#endif
    }
    else
    {
        FAIL_NO_CPU();
    }
}

#endif

//////////////////////////////////////////////////////
// MOG

#if BUILD_WITH_VIDEO_INPUT_SUPPORT

DEF_PARAM_TEST(Video_Cn_LearningRate, string, MatCn, double);

PERF_TEST_P(Video_Cn_LearningRate, MOG,
            Combine(Values("gpu/video/768x576.avi", "gpu/video/1920x1080.avi"),
                    CUDA_CHANNELS_1_3_4,
                    Values(0.0, 0.01)))
{
    const int numIters = 10;

    const string inputFile = perf::TestBase::getDataPath(GET_PARAM(0));
    const int cn = GET_PARAM(1);
    const float learningRate = static_cast<float>(GET_PARAM(2));

    cv::VideoCapture cap(inputFile);
    ASSERT_TRUE(cap.isOpened());

    cv::Mat frame;

    cap >> frame;
    ASSERT_FALSE(frame.empty());

    if (cn != 3)
    {
        cv::Mat temp;
        if (cn == 1)
            cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
        else
            cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
        cv::swap(temp, frame);
    }

    if (PERF_RUN_CUDA())
    {
        cv::Ptr<cv::BackgroundSubtractor> d_mog = cv::cuda::createBackgroundSubtractorMOG();

        cv::cuda::GpuMat d_frame(frame);
        cv::cuda::GpuMat foreground;

        d_mog->apply(d_frame, foreground, learningRate);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            startTimer();
            if(!next())
                break;

            d_mog->apply(d_frame, foreground, learningRate);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            d_mog->apply(d_frame, foreground, learningRate);
        }

        CUDA_SANITY_CHECK(foreground);
    }
    else
    {
        cv::Ptr<cv::BackgroundSubtractor> mog = cv::createBackgroundSubtractorMOG();
        cv::Mat foreground;

        mog->apply(frame, foreground, learningRate);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            startTimer();
            if(!next())
                break;

            mog->apply(frame, foreground, learningRate);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            mog->apply(frame, foreground, learningRate);
        }

        CPU_SANITY_CHECK(foreground);
    }
}

#endif

//////////////////////////////////////////////////////
// MOG2

#if BUILD_WITH_VIDEO_INPUT_SUPPORT

DEF_PARAM_TEST(Video_Cn, string, int);

PERF_TEST_P(Video_Cn, DISABLED_MOG2,
            Combine(Values("gpu/video/768x576.avi", "gpu/video/1920x1080.avi"),
                    CUDA_CHANNELS_1_3_4))
{
    const int numIters = 10;

    const string inputFile = perf::TestBase::getDataPath(GET_PARAM(0));
    const int cn = GET_PARAM(1);

    cv::VideoCapture cap(inputFile);
    ASSERT_TRUE(cap.isOpened());

    cv::Mat frame;

    cap >> frame;
    ASSERT_FALSE(frame.empty());

    if (cn != 3)
    {
        cv::Mat temp;
        if (cn == 1)
            cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
        else
            cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
        cv::swap(temp, frame);
    }

    if (PERF_RUN_CUDA())
    {
        cv::Ptr<cv::BackgroundSubtractorMOG2> d_mog2 = cv::cuda::createBackgroundSubtractorMOG2();
        d_mog2->setDetectShadows(false);

        cv::cuda::GpuMat d_frame(frame);
        cv::cuda::GpuMat foreground;

        d_mog2->apply(d_frame, foreground);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            startTimer();
            if(!next())
                break;

            d_mog2->apply(d_frame, foreground);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            d_mog2->apply(d_frame, foreground);
        }

        CUDA_SANITY_CHECK(foreground);
    }
    else
    {
        cv::Ptr<cv::BackgroundSubtractorMOG2> mog2 = cv::createBackgroundSubtractorMOG2();
        mog2->setDetectShadows(false);

        cv::Mat foreground;

        mog2->apply(frame, foreground);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            startTimer();
            if(!next())
                break;

            mog2->apply(frame, foreground);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            mog2->apply(frame, foreground);
        }

        CPU_SANITY_CHECK(foreground);
    }
}

#endif

//////////////////////////////////////////////////////
// MOG2GetBackgroundImage

#if BUILD_WITH_VIDEO_INPUT_SUPPORT

PERF_TEST_P(Video_Cn, MOG2GetBackgroundImage,
            Combine(Values("gpu/video/768x576.avi", "gpu/video/1920x1080.avi"),
                    CUDA_CHANNELS_1_3_4))
{
    const string inputFile = perf::TestBase::getDataPath(GET_PARAM(0));
    const int cn = GET_PARAM(1);

    cv::VideoCapture cap(inputFile);
    ASSERT_TRUE(cap.isOpened());

    cv::Mat frame;

    if (PERF_RUN_CUDA())
    {
        cv::Ptr<cv::BackgroundSubtractor> d_mog2 = cv::cuda::createBackgroundSubtractorMOG2();

        cv::cuda::GpuMat d_frame;
        cv::cuda::GpuMat d_foreground;

        for (int i = 0; i < 10; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            d_mog2->apply(d_frame, d_foreground);
        }

        cv::cuda::GpuMat background;

        TEST_CYCLE() d_mog2->getBackgroundImage(background);

        CUDA_SANITY_CHECK(background, 1);
    }
    else
    {
        cv::Ptr<cv::BackgroundSubtractor> mog2 = cv::createBackgroundSubtractorMOG2();
        cv::Mat foreground;

        for (int i = 0; i < 10; ++i)
        {
            cap >> frame;
            ASSERT_FALSE(frame.empty());

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            mog2->apply(frame, foreground);
        }

        cv::Mat background;

        TEST_CYCLE() mog2->getBackgroundImage(background);

        CPU_SANITY_CHECK(background);
    }
}

#endif

//////////////////////////////////////////////////////
// GMG

#if BUILD_WITH_VIDEO_INPUT_SUPPORT

DEF_PARAM_TEST(Video_Cn_MaxFeatures, string, MatCn, int);

PERF_TEST_P(Video_Cn_MaxFeatures, GMG,
            Combine(Values(string("gpu/video/768x576.avi")),
                    CUDA_CHANNELS_1_3_4,
                    Values(20, 40, 60)))
{
    const int numIters = 150;

    const std::string inputFile = perf::TestBase::getDataPath(GET_PARAM(0));
    const int cn = GET_PARAM(1);
    const int maxFeatures = GET_PARAM(2);

    cv::VideoCapture cap(inputFile);
    ASSERT_TRUE(cap.isOpened());

    cv::Mat frame;
    cap >> frame;
    ASSERT_FALSE(frame.empty());

    if (cn != 3)
    {
        cv::Mat temp;
        if (cn == 1)
            cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
        else
            cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
        cv::swap(temp, frame);
    }

    if (PERF_RUN_CUDA())
    {
        cv::cuda::GpuMat d_frame(frame);
        cv::cuda::GpuMat foreground;

        cv::Ptr<cv::BackgroundSubtractorGMG> d_gmg = cv::cuda::createBackgroundSubtractorGMG();
        d_gmg->setMaxFeatures(maxFeatures);

        d_gmg->apply(d_frame, foreground);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            if (frame.empty())
            {
                cap.release();
                cap.open(inputFile);
                cap >> frame;
            }

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            startTimer();
            if(!next())
                break;

            d_gmg->apply(d_frame, foreground);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            if (frame.empty())
            {
                cap.release();
                cap.open(inputFile);
                cap >> frame;
            }

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            d_frame.upload(frame);

            d_gmg->apply(d_frame, foreground);
        }

        CUDA_SANITY_CHECK(foreground);
    }
    else
    {
        cv::Mat foreground;
        cv::Mat zeros(frame.size(), CV_8UC1, cv::Scalar::all(0));

        cv::Ptr<cv::BackgroundSubtractorGMG> gmg = cv::createBackgroundSubtractorGMG();
        gmg->setMaxFeatures(maxFeatures);

        gmg->apply(frame, foreground);

        int i = 0;

        // collect performance data
        for (; i < numIters; ++i)
        {
            cap >> frame;
            if (frame.empty())
            {
                cap.release();
                cap.open(inputFile);
                cap >> frame;
            }

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            startTimer();
            if(!next())
                break;

            gmg->apply(frame, foreground);

            stopTimer();
        }

        // process last frame in sequence to get data for sanity test
        for (; i < numIters; ++i)
        {
            cap >> frame;
            if (frame.empty())
            {
                cap.release();
                cap.open(inputFile);
                cap >> frame;
            }

            if (cn != 3)
            {
                cv::Mat temp;
                if (cn == 1)
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2GRAY);
                else
                    cv::cvtColor(frame, temp, cv::COLOR_BGR2BGRA);
                cv::swap(temp, frame);
            }

            gmg->apply(frame, foreground);
        }

        CPU_SANITY_CHECK(foreground);
    }
}

#endif
