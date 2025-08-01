//
// Created by 13513 on 25-8-1.
//

#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#ifdef __cplusplus

#include <cstdint>
#include "arm_math.h"
//#include "stdint.h"

typedef enum {
    TransSucceeded = 0,
    TransFailed = 1,
    MatrixSingularError = 2,
    AreaTooSmallError = 3,
}PTStatus;

class PerspectiveTransform {
public:
    PerspectiveTransform(const uint16_t std_width, const uint16_t std_height) {
        std_width_ = std_width;
        std_height_ = std_height;
    }

    PTStatus solveTransMatrix(const uint16_t points[4][2]);
    PTStatus calcRectCenter(const uint16_t points[4][2]);

    uint16_t getRawCenterX() { return raw_cx_; }
    uint16_t getRawCenterY() { return raw_cy_; }

private:
    uint16_t std_width_, std_height_;
    uint16_t raw_cx_{}, raw_cy_ {};
    float32_t trans_matrix_[9] = {0.0f}; // t33 = 1
};

inline PerspectiveTransform pt(594, 420);

#endif

#endif //PERSPECTIVE_H
