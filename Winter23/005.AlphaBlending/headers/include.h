/**
 * @file include.h
 * @author Maximilian (https://github.com/MaximilianYan)
 * @brief Common consts
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef INCLUDE_H
#define INCLUDE_H

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <immintrin.h>

#include <opencv2/opencv.hpp>

#include "fileIO.h"

using namespace cv;

const int32_t WINSIZEX = 960;
const int32_t WINSIZEY = 1008;

const float SCALE = 1.f / 400.0f; // [1/px]
const float OFFSETX = -2.f;
const float OFFSETY = -SCALE * WINSIZEY / 2.f;

const float INFRAD = 10.f;
const int32_t INFNUM = 700;

const float FACTORPOW = 40.f;

const Scalar textColor = CV_RGB(255, 185, 0);

// void drawM(Mat image);
// extern const char WINNAME[];
// extern const int32_t BOOST_F;

const char WINNAME[] = "alphaBlending";
const int32_t BOOST_F = 1;
// const char* getPrName();

#endif // INCLUDE_H
