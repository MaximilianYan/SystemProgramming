#include "include.h"

__m256i mmy256_set123();

const char WINNAME[] = "mandelbrotAVX";

const int32_t BOOST_F = 8;
__m256i ADDSEQ /**/ = { 0 };
const __m256i ONESEQ /**/ = _mm256_set1_epi32(1);
const __m256 SCALESEQ/**/ = _mm256_set1_ps(SCALE);
const __m256 TWOSEQ  /**/ = _mm256_set1_ps(2.f);
const __m256 INFRADPOSSEQ = _mm256_set1_ps(INFRAD);

int main() {
    if (WINSIZEY % BOOST_F) {
        printf("BOOST DIVISIBILITY!!!\n");
        return 1;
    }
    printf("run2\n");
    ADDSEQ /**/ = mmy256_set123();
    fflush(stdout);

    Mat image(WINSIZEY, WINSIZEX, CV_8UC3);
    imshow(WINNAME, image);

    clock_t timer = clock();

    int key = 0;
    while (key != 27) {
        for (int32_t pxY = 0; pxY < WINSIZEY; ++pxY) {
            for (int32_t pxX = 0; pxX < WINSIZEX; pxX += BOOST_F) {

                // float ptX = OFFSETX + SCALE * pxX;
                // float ptY = OFFSETY + SCALE * pxY;

                __m256 ptX = _mm256_set1_ps(OFFSETX);
                __m256 ptY = _mm256_set1_ps(OFFSETY);

                __m256i pxXs = _mm256_add_epi32(_mm256_set1_epi32(pxY), ADDSEQ);
                ptX = _mm256_add_ps(ptX, _mm256_mul_ps(_mm256_cvtepi32_ps(pxXs), SCALESEQ));
                ptY = _mm256_add_ps(ptY, _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_set1_epi32(pxY)), SCALESEQ));

                // float vX = ptX;
                // float vY = ptY;
                __m256 vX = ptX;
                __m256 vY = ptY;

                // float vAbsQuadr = 0;
                __m256 vAbsQuadr = { 0 };

                int32_t counter = 0;
                __m256i counterStopped = _mm256_set1_epi32(0);

                // 0 if correct
                __m256i lessThenInfrad = { 0 };
                // 0 if less then infinity
                bool oneLessThenInfrad = 1;

                while (counter < INFNUM &&
                    oneLessThenInfrad) {

                    // float quadrX = vX * vX - vY * vY;
                    __m256 quadrX = _mm256_sub_ps(_mm256_mul_ps(vX, vX), _mm256_mul_ps(vY, vY));
                    // float quadrY = 2 * vX * vY;
                    __m256 quadrY = _mm256_mul_ps(_mm256_mul_ps(vX, vY), TWOSEQ);

                    // vX = quadrX + ptX;
                    vX = _mm256_add_ps(quadrX, ptX);
                    // vY = quadrY + ptY;
                    vY = _mm256_add_ps(quadrY, ptY);


                    // vAbsQuadr = vX * vX + vY * vY;
                    vAbsQuadr = _mm256_add_ps(_mm256_mul_ps(vX, vX), _mm256_mul_ps(vY, vY));

                    // DUNGEROUS: can catch overflow
                    lessThenInfrad = (__m256i)_mm256_cmp_ps(vAbsQuadr, INFRADPOSSEQ, _CMP_GE_OS); // >INF => -1
                    oneLessThenInfrad = _mm256_testz_si256(lessThenInfrad, lessThenInfrad);

                    counterStopped = _mm256_add_epi32(counterStopped, _mm256_add_epi32(ONESEQ, lessThenInfrad));
                    ++counter;
                }

                if (counter == INFNUM) {
                    image.at<Vec3b>(pxY, pxX)[0] = 0;
                    image.at<Vec3b>(pxY, pxX)[1] = 0;
                    image.at<Vec3b>(pxY, pxX)[2] = 0;
                } else {
                    // float factor = 1.0f - (float)counter / INFNUM;
                    float factor = (float)counter / INFNUM;

                    image.at<Vec3b>(pxY, pxX)[0] = 50;
                    // image.at<Vec3b>(pxY, pxX)[1] = (uint8_t)(256.f * pow(factor, 30));
                    image.at<Vec3b>(pxY, pxX)[1] = (uint8_t)(256.f * pow(factor, 0.2f));
                    image.at<Vec3b>(pxY, pxX)[2] = 0;
                }
            }
        }


        double spf = ((double)(-timer) + (timer = clock())) / CLOCKS_PER_SEC;
        char fpsS[10] = {};
        sprintf(fpsS, "%f", 1 / spf);
        // putText(image, fpsS, Point(0, 25), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(118, 185, 0), 2);

        // imshow(WINNAME, image);
        key = waitKey(1);
    }


    printf("Program finished 0\n");
    return 0;
}

__m256i mmy256_set123() {
    printf("run0\n");
    __m256i res = _mm256_set1_epi32(0);

    printf("run1\n");
    for (int32_t i = 0; i < BOOST_F; ++i) {
        ((int32_t*)&res)[i] = i;
    }

    printf("run0\n");
    return res;
}
