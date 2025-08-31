// MtrxMaths.cpp

#include "MtrxMaths.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

using std::vector;
using Eigen::MatrixXd;

const Eigen::IOFormat MtrxMaths::baseFormat(17, Eigen::DontAlignCols);

using std::cerr;

bool MtrxMaths::ReadMatrix(const std::string& filename, MatrixXd& outMatrix)
{
    std::ifstream file(filename, std::ios_base::in);
    if (!file.is_open())
    {
        cerr << filename << ": couldn't open file: " << std::strerror(errno) << "\n";
        return false;
    }

    int rows = 0;
    int cols = 0;

    vector<vector<double> > mx;
    while (true)
    {
        vector<double> row;
        if (!ReadRow(file, row))
        {
            return false;
        }
        if (rows == 0)
        {
            if (row.size() == 0)
            {
                return false;
            }
            cols = int(row.size());
        }

        if (row.size() == 0)
        {
            break;
        }

        if (row.size() != cols)
        {
            std::cerr << "All rows must have the same number of elements\n";
            return false;
        }

        mx.push_back(std::move(row));
        ++rows;
    }

    outMatrix.resize(rows, cols);
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            outMatrix(r, c) = mx[r][c];
        }
    }

    return true;
}

bool MtrxMaths::ReadRow(std::istream& file, vector<double>& row)
{
    std::string str;
    while(true)
    {
        std::getline(file, str);

        std::istringstream l(str);
        std::ws(l);
        if (l.eof())
        {
            if (file.eof())
            {
                return true;
            }
            continue;
        }
        if (l.peek() == '#')
        {
            continue;
        }
        while (true)
        {
            double entry;
            l >> entry;
            if (l.fail())
            {
                return false;
            }

            row.push_back(entry);
            std::ws(l);
            if (l.peek() == ',')
            {
                char c;
                l.get(c);
                std::ws(l);
            }

            if (l.eof())
            {
                return true;
            }
        }
    }
    return true;
}

bool MtrxMaths::InvertMatrix(const MatrixXd& inMatrix, MatrixXd& outMatrix)
{
    const auto n = inMatrix.rows();
    if (n != inMatrix.cols())
    {
        cerr << "Invertable matrix must be square\n";
        return false;
    }

    Eigen::PartialPivLU<MatrixXd> LU(inMatrix);
    if (LU.determinant() == 0)
    {
        cerr << "Matrix is singular";
        return false;
    }
    const MatrixXd x1(LU.inverse());

    // Perform one level of iterative refinement on the solution.
    // Note this calculates the right (numeric) inverse;
    // probably should have an option for the left inverse.
    const MatrixXd x2((2.0 * Eigen::MatrixXd::Identity(n, n) - x1 * inMatrix) * x1) ;

    outMatrix = std::move(x2);
    return true;
}

bool MtrxMaths::Determinant(const Eigen::MatrixXd& matrix, double& det)
{
    if (matrix.rows() != matrix.cols())
    {
        cerr << "Matrix must be square\n";
        return false;
    }

    Eigen::PartialPivLU<MatrixXd> LU(matrix);
    det = LU.determinant();

    return true;
}
