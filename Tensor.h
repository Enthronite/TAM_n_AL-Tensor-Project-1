#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>

template <typename T>
class Tensor
{
private:
    std::vector<std::size_t> shape_;
    std::vector<T> data_;

public:
    explicit Tensor(const std::vector<std::size_t>& dimensions)
        : shape_(dimensions)
    {
        // Put your existing constructor implementation here,
        // including your validation and data_.resize(total).
    }

    std::size_t size() const
    {
        return data_.size();
    }
};

#endif
