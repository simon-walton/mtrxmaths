// main.cpp

#include "MtrxMaths.h"
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <locale>

using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    std::string file1;
    std::string file2;
    auto op = MtrxMaths::Op::none;
    int nParams = 0;

    using Eigen::MatrixXd;
    MatrixXd m1, m2;

    static const struct
    {
        const char * flag;
        MtrxMaths::Op op;
        int params;
    } commands[] =
        {
            {"-a", MtrxMaths::Op::add, 2},
            {"--add", MtrxMaths::Op::add, 2},
            {"-s", MtrxMaths::Op::subtract, 2},
            {"--subtract", MtrxMaths::Op::subtract, 2},
            {"-m", MtrxMaths::Op::multiply, 2},
            {"--multiply", MtrxMaths::Op::multiply, 2},
            {"-i", MtrxMaths::Op::invert, 1},
            {"--invert", MtrxMaths::Op::invert, 1},
            {"-D", MtrxMaths::Op::determinant, 1},
            {"--determinant", MtrxMaths::Op::determinant, 1},
            {"-d", MtrxMaths::Op::divide, 2},
            {"--divide", MtrxMaths::Op::divide, 2},
            {"-r", MtrxMaths::Op::reverseDivide, 2},
            {"--reverseDivide", MtrxMaths::Op::reverseDivide, 2},
            {"-t", MtrxMaths::Op::transpose, 1},
            {"--transpose", MtrxMaths::Op::transpose, 1},
            {"-q", MtrxMaths::Op::info, 1},
            {"--query", MtrxMaths::Op::info, 1},
        };

    if (argc < 3)
    {
        goto usage;
    }

    for (int c = 0; c < sizeof (commands) / sizeof(commands[0]); ++c)
    {
        if (strcmp(commands[c].flag, argv[1]) == 0)
        {
            op = commands[c].op;
            nParams = commands[c].params;
            break;
        }
    }

    if (argc != nParams + 2)
    {
        goto usage;
    }

    // Allow user's preferred locale for numbers.
    try
    {
        std::locale::global(std::locale(""));
        cout.imbue(std::locale());
    } catch (...)
    {
    }

    if (nParams >= 1)
    {
        if (!MtrxMaths::ReadMatrix(argv[2], m1))
        {
            cerr << argv[2] << ": couldn't parse file\n";
            return 1;
        }
        if (nParams >= 2)
        {
            if (!MtrxMaths::ReadMatrix(argv[3], m2))
            {
                cerr << argv[3] << ": couldn't parse file\n";
                return 1;
            }
        }
    }

    switch (op)
    {

    // Addition/subtraction
    case MtrxMaths::Op::add:
    case MtrxMaths::Op::subtract:
    {
        if (m1.rows() != m2.rows() || m1.cols() != m2.cols())
        {
            cerr << "Matrices must be same dimensions\n";
            return 1;
        }

        m1 = op == MtrxMaths::Op::add ? m1 + m2 : MatrixXd(m1 - m2);
        cout << m1.format(MtrxMaths::baseFormat) << std::endl;

        break;
    }
    //____________________________________________________________________

    // Multiplication
    case MtrxMaths::Op::multiply:
    {
        if (m1.cols() != m2.rows())
        {
            cerr << "Matrices have incompatible dimensions\n";
            return 1;
        }

        MatrixXd result(m1 * m2);
        cout << result.format(MtrxMaths::baseFormat) << std::endl;

        break;
    }
    //____________________________________________________________________

    // Inversion
    case MtrxMaths::Op::invert:
    {
        MatrixXd result;
        if (!MtrxMaths::InvertMatrix(m1, result))
        {
            return 1;
        }
        cout << result.format(MtrxMaths::baseFormat) << std::endl;

        break;
    }
    //____________________________________________________________________

    // Determinant
    case MtrxMaths::Op::determinant:
    {
        double d = 0.0;
        if (!MtrxMaths::Determinant(m1, d))
        {
            return 1;
        }

        cout << std::setprecision(17) << d << std::endl;
        break;
    }
    //____________________________________________________________________

    // Division
    case MtrxMaths::Op::divide:
    case MtrxMaths::Op::reverseDivide:
    {
        const bool isDivide = op == MtrxMaths::Op::divide;

        if (isDivide && m1.cols() != m2.rows()
            || !isDivide && m2.cols() != m2.rows())
        {
            cerr << "Matrices have incompatible dimensions\n";
            return 1;
        }
        if (!MtrxMaths::InvertMatrix(m2, m2))
        {
            return 1;
        }


        const MatrixXd result(isDivide ? m1 * m2 : m2 * m1);
        cout << result.format(MtrxMaths::baseFormat) << std::endl;

        break;
    }
    //____________________________________________________________________

    // Transpose
    case MtrxMaths::Op::transpose:
    {
        m1.transposeInPlace();
        cout << m1.format(MtrxMaths::baseFormat) << std::endl;
        break;
    }
    //____________________________________________________________________


    // Info
    case MtrxMaths::Op::info:
    {
        cout << argv[2] << ": " << m1.rows() << "x" << m1.cols() << std::endl;
        break;
    }
    //____________________________________________________________________

    default:
        goto usage;
    }

    return 0;

    usage:
    cerr << R"(Usage: mtrxmaths OPT FILE1 [FILE2]
    -a/--add            Addition
    -s/--subtract       Subtraction
    -m/--multiply       Multiplication
    -D/--determinant    Determinant
    -d/--divide         Division
    -r/--reverseDivide  Reverse division
    -i/--invert         Inversion
    -t/--transpose      Transpose
    -q/--query          Info
)";
    return 1;
}
