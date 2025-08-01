//
// Created by 13513 on 25-8-1.
//

#include "../Inc/perspective.hpp"

/**
 * @brief  计算A4纸透视变换的转换矩阵
 * @param  points 四个角点的坐标：{{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}}
 * @retval TransSucceeded：计算成功；MatrixSingularError：角点可能共线；AreaTooSmallError：面积太小，排除
 */
PTStatus PerspectiveTransform::solveTransMatrix(const uint16_t points[4][2]) {
    // 更稳健的面积计算（考虑凸四边形）
    float area = 0.5f * fabsf(
        points[0][0]*(points[1][1] - points[3][1]) +
        points[1][0]*(points[2][1] - points[0][1]) +
        points[2][0]*(points[3][1] - points[1][1]) +
        points[3][0]*(points[0][1] - points[2][1])
    );
    if (area < 100.0f) { // 使用命名常量提高可读性
        return AreaTooSmallError;
    }

    // 使用临时变量简化表达式
    const auto w = static_cast<float>(std_width_);
    const auto h = static_cast<float>(std_height_);

    // 构建系数矩阵A（优化内存布局）
    float32_t A_data[64] = {
        // 点0 (0,0)
        0, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0,
        // 点1 (w,0)
        w, 0, 1, 0, 0, 0, -w*points[1][0], 0,
        0, 0, 0, w, 0, 1, -w*points[1][1], 0,
        // 点2 (w,h)
        w, h, 1, 0, 0, 0, -w*points[2][0], -h*points[2][0],
        0, 0, 0, w, h, 1, -w*points[2][1], -h*points[2][1],
        // 点3 (0,h)
        0, h, 1, 0, 0, 0, 0, -h*points[3][0],
        0, 0, 0, 0, h, 1, 0, -h*points[3][1]
    };

    // 常数向量B（使用循环初始化）
    float32_t b_data[8];
    for (int i = 0; i < 4; i++) {
        b_data[2*i]   = static_cast<float32_t>(points[i][0]);
        b_data[2*i+1] = static_cast<float32_t>(points[i][1]);
    }

    // ==== 最小二乘解 t = (A^T A)^{-1} A^T b ====
    arm_matrix_instance_f32 A, A_t, AtA, AtA_inv, A_pseudo_inv, B, T;
    float32_t A_t_data[64], AtA_data[64], AtA_inv_data[64], A_pseudo_inv_data[64], t_data[8];

    // 初始化矩阵
    arm_mat_init_f32(&A, 8, 8, A_data);
    arm_mat_init_f32(&B, 8, 1, b_data);
    arm_mat_init_f32(&T, 8, 1, t_data);

    // 计算A的转置
    arm_mat_init_f32(&A_t, 8, 8, A_t_data);
    if (arm_mat_trans_f32(&A, &A_t) != ARM_MATH_SUCCESS)
        return TransFailed;

    // 计算AtA = A^T * A
    arm_mat_init_f32(&AtA, 8, 8, AtA_data);
    if (arm_mat_mult_f32(&A_t, &A, &AtA) != ARM_MATH_SUCCESS)
        return TransFailed;

    // 求AtA的逆
    arm_mat_init_f32(&AtA_inv, 8, 8, AtA_inv_data);
    if (arm_mat_inverse_f32(&AtA, &AtA_inv) != ARM_MATH_SUCCESS)
        return MatrixSingularError;

    // 计算伪逆 A⁺ = (A^T A)^{-1} A^T
    arm_mat_init_f32(&A_pseudo_inv, 8, 8, A_pseudo_inv_data);
    if (arm_mat_mult_f32(&AtA_inv, &A_t, &A_pseudo_inv) != ARM_MATH_SUCCESS)
        return TransFailed;

    // 计算解向量 t = A⁺ b
    if (arm_mat_mult_f32(&A_pseudo_inv, &B, &T) != ARM_MATH_SUCCESS)
        return TransFailed;

    // 存储变换矩阵 (3x3)
    for (uint8_t i = 0; i < 8; i++) {
        trans_matrix_[i] = t_data[i];
    }
    trans_matrix_[8] = 1.0f;

    return TransSucceeded;
}

/**
 * @brief  通过仿射变换得到原图中的中点
 * @return TransSucceeded：计算成功；TransFailed：计算失败
 */
PTStatus PerspectiveTransform::calcRectCenter(const uint16_t points[4][2]) {
    PTStatus status = solveTransMatrix(points);
    if (status != TransSucceeded) {
        return status;
    }

    uint16_t std_cx = std_width_ / 2;
    uint16_t std_cy = std_height_ / 2;

    // 齐次坐标 [x, y, 1]
    float32_t std_center[3] = {
        static_cast<float32_t>(std_cx),
        static_cast<float32_t>(std_cy),
        1.0f
    };

    float32_t raw_center[3];
    arm_matrix_instance_f32 T, p_std, p_raw;

    // 使用正向变换矩阵 trans_matrix_
    arm_mat_init_f32(&T, 3, 3, trans_matrix_);
    arm_mat_init_f32(&p_std, 3, 1, std_center);
    arm_mat_init_f32(&p_raw, 3, 1, raw_center);

    // 计算：p_raw = T * p_std
    if (arm_mat_mult_f32(&T, &p_std, &p_raw) != ARM_MATH_SUCCESS) {
        return TransFailed;
    }

    // 齐次坐标归一化 (透视除法)
    if (fabsf(raw_center[2]) < 1e-6f) {  // 防止除零
        return TransFailed;
    }

    raw_cx_ = (uint16_t)lroundf(raw_center[0] / raw_center[2]);
    raw_cy_ = (uint16_t)lroundf(raw_center[1] / raw_center[2]);

    return TransSucceeded;
}
