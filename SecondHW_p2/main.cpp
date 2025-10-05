#include "formatter.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <memory>

using Eigen::Vector2d;
using Eigen::Matrix2d;

int main() {
    // 设置日志
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>("optimizer.log", true);
    auto logger = std::make_shared<spdlog::logger>("optimizer", 
        spdlog::sinks_init_list{console_sink, file_sink});
    
    spdlog::set_level(spdlog::level::debug);
    logger->set_level(spdlog::level::debug);

    // 参数设置
    const double lambda = 0.5;
    const double delta = 0.01;
    const long long student_id = 2254415180; 
    
    // 初始点
    Vector2d xk(student_id % 827, student_id % 1709);

    // 计算 Hessian 矩阵和逆矩阵
    Matrix2d H;
    H << 2, 0,
         0, 2;
    
    // 添加调试输出
    logger->debug("Hessian 矩阵 H: {}", H);
    
    // 使用 Eigen 计算逆矩阵
    Matrix2d H_inv = H.inverse();
    
    // 添加调试输出
    logger->debug("Hessian 逆矩阵 H_inv (通过 H.inverse() 计算): {}", H_inv);
    
    // 验证逆矩阵
    Matrix2d identity_check = H * H_inv;
    
    // 添加调试输出
    logger->debug("验证 H * H_inv = I (单位矩阵): {}", identity_check);

    logger->debug("初始点: ({}, {})", xk(0), xk(1));

    while (true) {
        Vector2d grad(2 * xk(0), 2 * xk(1)); // 梯度 ∇f = [2x, 2y]
        Vector2d xk_new = xk - lambda * H_inv * grad;
        
        if ((xk_new - xk).norm() < delta) break;
        
        xk = xk_new;
        logger->debug("({}, {})", xk(0), xk(1));
    }

    // 输出最小值 f(x,y) = x² + y²
    double min_value = xk.squaredNorm();
    logger->info("最小值: {}", min_value);

    return 0;
}
