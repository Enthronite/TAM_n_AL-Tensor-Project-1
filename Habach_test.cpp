#include "Tensor.h"


#include <cassert>
#include <sstream>
#include <type_traits>

// These tests cover the information functions implemented by Tamer.
template <typename T>
void checkInfo(const std::vector<std::size_t>& dimensions,
               std::size_t expectedSize, const std::string& expectedOutput) {
    const Tensor<T> tensor(dimensions);
    assert(tensor.ndim() == dimensions.size());
    assert(tensor.size() == expectedSize);
    assert(tensor.shape() == dimensions);
    static_assert(std::is_same<decltype(tensor.shape()),
                  const std::vector<std::size_t>&>::value,
                  "shape() must return a const reference");
    assert(&tensor.shape() == &tensor.shape());

    std::ostringstream output;
    std::streambuf* previous = std::cout.rdbuf(output.rdbuf());
    tensor.printInfo();
    std::cout.rdbuf(previous);
    assert(output.str() == expectedOutput);
    assert(tensor.shape() == dimensions);
    assert(tensor.size() == expectedSize);
}

int main() {
    checkInfo<int>({10}, 10, "Shape: [10]\nDimensions: 1\nElements: 10\n");
    checkInfo<float>({3, 4}, 12, "Shape: [3, 4]\nDimensions: 2\nElements: 12\n");
    checkInfo<double>({2, 3, 4}, 24, "Shape: [2, 3, 4]\nDimensions: 3\nElements: 24\n");
    checkInfo<double>({3, 32, 32}, 3072, "Shape: [3, 32, 32]\nDimensions: 3\nElements: 3072\n");
    checkInfo<int>({2, 1, 3, 4}, 24, "Shape: [2, 1, 3, 4]\nDimensions: 4\nElements: 24\n");

    // The existing constructor normalizes an empty shape to {1}.
    const Tensor<int> emptyShape(std::vector<std::size_t>{});
    assert(emptyShape.ndim() == 1);
    assert(emptyShape.size() == 1);
    assert(emptyShape.shape() == std::vector<std::size_t>{1});
    std::cout << "Tensor information tests passed.\n";
}
