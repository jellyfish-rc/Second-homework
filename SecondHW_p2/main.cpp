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

    // 牛顿下降法迭代
    Matrix2d H_inv;
    H_inv << 0.5, 0, 0, 0.5; // Hessian逆矩阵 [[2,0],[0,2]]的逆

    logger->debug("({}, {})", xk(0), xk(1)); // 格式化输出

    while (true) {
        Vector2d grad(2 * xk(0), 2 * xk(1)); // 梯度?
        Vector2d xk_new = xk - lambda * H_inv * grad;
        
        if ((xk_new - xk).norm() < delta) break;
        
        xk = xk_new;
        logger->debug("({}, {})", xk(0), xk(1));
    }

    // 输出最小值 f(x,y) = x² + y²
    double min_value = xk.squaredNorm();
    logger->info("{}", min_value);

    return 0;
}
