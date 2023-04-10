#include "include.h"

void mmy256_set123(__m256i* res);

const char WINNAME[] = "mandelbrotAVX";
const int32_t BOOST_F = 8;

alignas(32) const __m256i ONESEQ     /**/ = _mm256_set1_epi32(1);
alignas(32) const __m256i BITSEQ     /**/ = _mm256_set1_epi32(0xFFFFFFFF);
alignas(32) /**/  __m256i ADDSEQ     /**/ = _mm256_set1_epi32(0);
alignas(32) const __m256 SCALESEQ    /**/ = _mm256_set1_ps(SCALE);
alignas(32) const __m256 TWOSEQ      /**/ = _mm256_set1_ps(2.f);
alignas(32) const __m256 INFRADQADRPOSSEQ = _mm256_set1_ps(INFRAD * INFRAD);

int main() {
    if (WINSIZEX % BOOST_F) {
        printf("BOOST DIVISIBILITY!!!\n");
        return 1;
    }
    mmy256_set123(&ADDSEQ);

    Mat image(WINSIZEY, WINSIZEX, CV_8UC3);
    imshow(WINNAME, image);

    clock_t timer = clock();

    int key = 0;
    while (key != 27) {
        for (int32_t pxY = 0; pxY < WINSIZEY; ++pxY) {
            for (int32_t pxX = 0; pxX < WINSIZEX; pxX += BOOST_F) {

                // float ptX = OFFSETX + SCALE * pxX;
                // float ptY = OFFSETY + SCALE * pxY;

                alignas(32) __m256 ptX = _mm256_set1_ps(OFFSETX);
                alignas(32) __m256 ptY = _mm256_set1_ps(OFFSETY);

                alignas(32) __m256i pxXs = _mm256_add_epi32(_mm256_set1_epi32(pxX), ADDSEQ);
                ptX = _mm256_add_ps(ptX, _mm256_mul_ps(_mm256_cvtepi32_ps(pxXs), SCALESEQ));
                ptY = _mm256_add_ps(ptY, _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_set1_epi32(pxY)), SCALESEQ));

                // float vX = ptX;
                // float vY = ptY;
                alignas(32) __m256 vX = ptX;
                alignas(32) __m256 vY = ptY;

                // float vAbsQuadr = 0;
                alignas(32) __m256 vAbsQuadr = { 0 };

                int32_t counter = 0;
                alignas(32) __m256i counterStopped = _mm256_set1_epi32(0);

                // 0 if correct
                alignas(32) __m256i lessThanInfrad = { 0 };
                // 0 if less than infinity
                bool oneLessThanInfrad = 1;

                while (counter < INFNUM &&
                    oneLessThanInfrad) {
                    // float quadrX = vX * vX - vY * vY;
                    alignas(32) __m256 quadrX = _mm256_sub_ps(_mm256_mul_ps(vX, vX), _mm256_mul_ps(vY, vY));
                    // float quadrY = 2 * vX * vY;
                    alignas(32) __m256 quadrY = _mm256_mul_ps(_mm256_mul_ps(vX, vY), TWOSEQ);

                    // vX = quadrX + ptX;
                    vX = _mm256_add_ps(quadrX, ptX);
                    // vY = quadrY + ptY;
                    vY = _mm256_add_ps(quadrY, ptY);

                    // vAbsQuadr = vX * vX + vY * vY;
                    vAbsQuadr = _mm256_add_ps(_mm256_mul_ps(vX, vX), _mm256_mul_ps(vY, vY));

                    // DUNGEROUS: can catch overflow
                    lessThanInfrad = (__m256i)_mm256_cmp_ps(vAbsQuadr, INFRADQADRPOSSEQ, _CMP_NLE_UQ); // >INF => -1
                    alignas(32) __m256i notLessThanInfrad = _mm256_xor_si256(lessThanInfrad, BITSEQ);
                    oneLessThanInfrad = !_mm256_testz_si256(notLessThanInfrad, notLessThanInfrad);

                    counterStopped = _mm256_add_epi32(counterStopped, _mm256_add_epi32(ONESEQ, lessThanInfrad));
                    ++counter;
                }

                for (int32_t dx = 0; dx < BOOST_F; ++dx) {
                    counter = ((int32_t*)&counterStopped)[dx];

                    if (counter == INFNUM) {
                        image.at<Vec3b>(pxY, pxX + dx)[0] = 0;
                        image.at<Vec3b>(pxY, pxX + dx)[1] = 0;
                        image.at<Vec3b>(pxY, pxX + dx)[2] = 0;
                    } else {
                        float factor = 1.0f - (float)counter / INFNUM;

                        image.at<Vec3b>(pxY, pxX + dx)[0] = 50;
                        image.at<Vec3b>(pxY, pxX + dx)[1] = (uint8_t)(255.f * pow(factor, FACTORPOW));
                        image.at<Vec3b>(pxY, pxX + dx)[2] = 255;
                    }
                }
            }
        }


        double spf = ((double)(-timer) + (timer = clock())) / CLOCKS_PER_SEC;
        char fpsS[10] = {};
        sprintf(fpsS, "%f", 1 / spf);
        putText(image, fpsS, Point(0, 25), cv::FONT_HERSHEY_DUPLEX, 1.0, textColor, 2);

        imshow(WINNAME, image);
        key = waitKey(1);
    }

    printf("Program finished 0\n");
    return 0;
}

void mmy256_set123(__m256i* res) {
    for (int32_t i = 0; i < BOOST_F; ++i) {
        ((int32_t*)res)[i] = i;
    }
}
