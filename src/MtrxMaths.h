// MtrxMaths.h

#pragma once

#include <vector>
#include <string>
#include <iosfwd>

#include "Eigen/Dense"
#include "Eigen/src/Core/IO.h"

class MtrxMaths
{
public:
    enum class Op {none, add, subtract, multiply, divide, reverseDivide, invert, determinant, transpose, info};

    // Output format is full round-trip precision (17 digits for double)
    static const Eigen::IOFormat baseFormat;

    static bool ReadMatrix(const std::string& filename, Eigen::MatrixXd& outMatrix);
    static bool InvertMatrix(const Eigen::MatrixXd& inMatrix, Eigen::MatrixXd& outMatrix);
    static bool Determinant(const Eigen::MatrixXd& matrix, double& det);

private:
    static bool ReadRow(std::istream& file, std::vector<double>& row);
};
