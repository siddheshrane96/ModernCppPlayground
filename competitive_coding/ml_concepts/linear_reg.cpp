#include <iostream>
#include <vector>
#include <Eigen/Dense>
/**
 * Write a function that performs linear regression using the normal equation.
 * The function should take a matrix X (features) and a vector y (target) as input, and return the coefficients of the linear regression model.
 * Round your answer to four decimal places, -0.0 is a valid result for rounding a very small number.
 * 
 * Example:
        input: X = [[1, 1], [1, 2], [1, 3]], y = [1, 2, 3]
        output: [0.0, 1.0]
        reasoning: The linear model is y = 0.0 + 1.0*x, perfectly fitting the input data.
 */

Eigen::VectorXd linear_regression_direct(const Eigen::MatrixXd& M, const Eigen::VectorXd & y)
{
    assert(M.rows()==y.rows());
    Eigen::VectorXd theta = (M.transpose() * M).inverse() * M.transpose() * y;
    theta = theta.unaryExpr([](double val) {
        return std::round(val * 10000.0) / 10000.0;
    });
    return theta;
}

Eigen::VectorXd linear_regression_grad_desc(const Eigen::MatrixXd& M, const Eigen::VectorXd& y, size_t max_steps=30, float learning_rate=0.01f)
{
    assert(M.rows() == y.rows());
    Eigen::VectorXd x(M.cols());
    for(size_t i=0; i<M.cols(); ++i)
        x[i] = 0;

    for(size_t step=0; step<max_steps; ++step)
    {
        double total_error = 0;
        for(size_t row=0; row<M.rows(); ++row)
        {
            double y_pred = 0;
            for(size_t col=0; col<M.cols(); ++col)
            {
                y_pred += M[row][col] * x[col];
            }
            total_error += (y[row] - y_pred);
        }

        for(size_t col=0; col<M.cols(); ++col)
        {
            double der = -2*(total_error)*M
            x[col] = x[col] - alpha
        }
    }
    return x;
}

// Eigen::VectorXd linear_regression_grad_desc(const Eigen::MatrixXd& M, const Eigen::VectorXd& y, size_t max_steps=30, float learning_rate=0.01f)
// {
//     // Step 1: Initialize weights to zero
//     Eigen::VectorXd x(M.cols());
//     for (size_t i = 0; i < M.cols(); ++i) {
//         x[i] = 0;
//     }

//     // Step 2: Perform gradient descent
//     for (size_t step = 0; step < max_steps; ++step) {
//         double total_error = 0;

//         // Step 3: Calculate predictions and total error
//         Eigen::VectorXd errors(M.rows());
//         for (size_t i = 0; i < M.rows(); ++i) {
//             double y_pred = 0;
//             for (size_t j = 0; j < M.cols(); ++j) {
//                 y_pred += M(i, j) * x[j];
//             }
//             errors[i] = y[i] - y_pred;
//             total_error += errors[i] * errors[i];
//         }
//         total_error /= M.rows();  // Optional: average error per sample

//         // Step 4: Compute gradient for each feature and update weights
//         for (size_t j = 0; j < M.cols(); ++j) {
//             double gradient_j = 0;
//             for (size_t i = 0; i < M.rows(); ++i) {
//                 gradient_j += -2.0 * M(i, j) * errors[i];
//             }
//             gradient_j /= M.rows();
//             x[j] = x[j] - learning_rate * gradient_j;
//         }
//     }

//     // Step 6: Return the optimized weights
//     return x;
// }

// Eigen::VectorXd linear_regression_grad_desc(const Eigen::MatrixXd& M, const Eigen::VectorXd& y, size_t max_steps=30, float learning_rate=0.01f)
// {
//     // Step 1: Initialize x to zero
//     Eigen::VectorXd x = Eigen::VectorXd::Zero(M.cols());

//     // Step 2: Perform gradient descent for max_steps iterations
//     for(size_t step = 0; step < max_steps; ++step)
//     {
//         // Step 3: Compute the error vector (y - M * x)
//         Eigen::VectorXd error = y - M * x;

//         // Step 4: Compute the gradient
//         Eigen::VectorXd gradient = -2.0 / M.rows() * M.transpose() * error;

//         // Step 5: Update x using the gradient
//         x = x - learning_rate * gradient;
//     }

//     // Step 6: Return the optimized weights x
//     return x;
// }


int main()
{
    Eigen::MatrixXd M(3,2);
    M << 1,1,1,2,1,3;
    Eigen::VectorXd y(3);
    y << 1,2,3;
    std::cout << "M:\n" << M << "\ny:\n" << y << std::endl;
    std::cout << "Coeffs:\n" << linear_regression_direct(M,y) << std::endl;
    linear_regression_grad_desc(M, y);
    return 0;
}
