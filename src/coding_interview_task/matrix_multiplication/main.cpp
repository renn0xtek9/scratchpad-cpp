#include <array>
#include <iostream>

template <int NROW, int NCOL>
class Matrix {
 public:
  std::array<std::array<double, NCOL>, NROW> elements{};
};

namespace {

template <int NROW1, int NCOL1, int NROW2, int NCOL2>
double ElementProduct(const Matrix<NROW1, NCOL1>& mat1,
                      const Matrix<NROW2, NCOL2>& mat2,
                      const std::size_t& row_index,
                      const std::size_t& col_index) {
  double result{};
  for (std::size_t element_index = 0; element_index < NCOL1; ++element_index) {
    result += mat1.elements.at(row_index).at(element_index) * mat2.elements.at(col_index).at(element_index);
  }
  return result;
}
}  // namespace

template <int NROW1, int NCOL1, int NROW2, int NCOL2, std::enable_if_t<NCOL1 == NROW2, int> = 0>
Matrix<NROW1, NCOL2> MatrixMutliplication(const Matrix<NROW1, NCOL1>& mat1, const Matrix<NROW2, NCOL2>& mat2) {
  Matrix<NROW1, NCOL2> result{};
  for (std::size_t row_index = 0; row_index < NROW1; ++row_index) {
    for (std::size_t col_index = 0; col_index < NCOL2; ++col_index) {
      result.elements.at(row_index).at(col_index) = ElementProduct(mat1, mat2, row_index, col_index);
    }
  }
  return result;
}

template <int NROW, int NCOL>
void Display(const Matrix<NROW, NCOL>& mat) {
  for (const auto& row : mat.elements) {
    for (const auto& col : row) {
      std::cout << " " << col;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  Matrix<2, 2> mat1{};
  mat1.elements.at(0).at(0) = 0.0;
  mat1.elements.at(0).at(1) = 1.0;
  mat1.elements.at(1).at(0) = 1.0;
  mat1.elements.at(1).at(1) = 1.0;

  Matrix<2, 2> mat2{};
  mat2.elements.at(0).at(0) = 2.0;
  mat2.elements.at(0).at(1) = 2.0;
  mat2.elements.at(1).at(0) = 2.0;
  mat2.elements.at(1).at(1) = 2.0;

  auto result = MatrixMutliplication(mat1, mat2);

  // Matrix<1,2> bad_mat{}; // not compile on purpose.
  // auto bad_result=matrix_multiplication(mat1,bad_mat);

  Display(mat1);
  Display(mat2);
  Display(result);
  std::cout << "Coding Task: Matrix multiplication: END" << std::endl;
  return 0;
}
