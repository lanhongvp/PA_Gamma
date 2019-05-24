// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2017 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "platform.h"
#include "net.h"

static int detect_headpose(const cv::Mat& bgr, std::vector<std::vector<float> >& hdp_scores)
{
    ncnn::Net headpose;

    std::cout << "Begin to analysis param and model " << std::endl;
    headpose.load_param("../model_param/headpose-int8.param");
    headpose.load_model("../model_param/headpose-int8.bin");
    // std::cout << "haha" << std::endl;

    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR, bgr.cols, bgr.rows, 224, 224);

    const float mean_vals[3] = {104.f, 117.f, 123.f};
    in.substract_mean_normalize(mean_vals, 0);

    ncnn::Extractor ex = headpose.create_extractor();

    ex.input("blob1", in);

    std::vector <ncnn::Mat> out;
    ncnn::Mat out1;
    ncnn::Mat out2;
    ncnn::Mat out3;
    ex.extract("fc_blob1", out1);
    out.push_back(out1);
    ex.extract("fc_blob2", out2);
    out.push_back(out2);
    ex.extract("fc_blob3", out3);
    out.push_back(out3);

    int hd_angles = 3;
    hdp_scores.resize(hd_angles);

    std::cout << "out height " << out1.h << std::endl;
    std::cout << "out width " << out1.w << std::endl;
    std::cout << "out width " << out2.w << std::endl;
    std::cout << "out width " << out3.w << std::endl;
    
    // hdp_scores[0].resize(out1.h);
    for (int i=0; i<hd_angles; i++) {
        hdp_scores[i].resize(out1.w);
        for (int j=0; j<out1.w; j++)
        {
            hdp_scores[i][j] = out[i][j];
            std::cout << out[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Param and model loaded" << std::endl;
    
    return 0;
}

static int print_hdpscores(const std::vector<std::vector<float> >& hdp_scores)
{
    // partial sort topk with index
    int hd_angles = hdp_scores.size();
    int hd_dims = hdp_scores[0].size();
    std::cout << "xixi" << std::endl;
    // print headpose score in hd_angles
    for(int i=0; i<hd_angles; i++) {
        std::cout << "pose " << i << " [";
        for(int j=0; j<hd_dims; j++) {
            std::cout << hdp_scores[i][j] << " ";
        }
        std::cout << "]" << std::endl;
    }

    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }

    const char* imagepath = argv[1];

    cv::Mat m = cv::imread(imagepath, 1);
    if (m.empty())
    {
        fprintf(stderr, "cv::imread %s failed\n", imagepath);
        return -1;
    }

    std::vector<std::vector<float> > hdp_scores;
    std::cout << "Begin to detect model " << std::endl;
    detect_headpose(m, hdp_scores);
    
    std::cout << "print headpose" << std::endl;
    print_hdpscores(hdp_scores);

    // return 0;
}
