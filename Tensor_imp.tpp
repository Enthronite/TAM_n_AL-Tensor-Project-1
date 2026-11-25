#pragma once

#include "rng.h"
#include "Tensor.h"

#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <iostream> 
// even though we don't need to rewrite the include statements, I'm doing so because it makes it clear 
// exactly what my file is using

// even though the class declares a template, since I'm making this function outside of the class, now I have to declare:
template <typename T>
void Tensor<T>::reshape(const std::vector<std::size_t>& newShape) <%
    // the function is void because we're just changing a class variable, not returning anything
    // that's also why all we need is a new vector to replace the old shape vector with
    // also, all this function is really doing is making sure the new shape vector is valid, and if it is,
    // it swaps it in to the variable "shape_"

    // EDIT 11/15/26: You might remember I had code to reject empty vectors for the reshape function. I removed it
    // because there could be a situation where the user initialized a tensor as a scalar tensor and then flattens
    // it using the flatten() function, which would make it a 1D tensor with 1 element, and then calls reshape on 
    // the tensor with an empty vector like this: A.reshape(<%%>); , to make it a scalar tensor again. In that 
    // situation, we should allow an empty input. We don't have to worry about empty input messing with other 
    // tensors because I already have a statement below to reject the new shape if it's size doesn't match the size 
    // of the old tensor shape, which we can guarantee since the loop to count the size of the new shape will never 
    // run if the reshape vector is empty. 

    int i = 0; // counter
    std::size_t TL = size(); // total capacity
    const std::size_t MAXIMUM_CAPACITY = 1000000;
    std::size_t TL2 = 1; // total capacity
    while (i < static_cast<int>(newShape.size())) <%

        std::size_t dimension = newShape<:i:>; 

        if (dimension <= 0) 
        <% throw std::invalid_argument("Your dimension is less than or equal to zero. Fix this: " + std::to_string(dimension)); %>

        if (TL2 > std::numeric_limits<std::size_t>::max() / static_cast<std::size_t>(dimension)) 
        <% throw std::overflow_error("Integer Overflow Detected."); %>

        TL2 *= dimension;

        if (TL2 > MAXIMUM_CAPACITY) 
        <% throw std::length_error("Your tensor is too long."); %>

        i++;

    %>
    // you may notice that I literally just copied the code that I used to initialize a tensor in the class. 
    // that's because like I said before, all we need to do is verify the new shape is the same as the old shape.
    // since the new shape vector doesn't have a size() function like the old shape does, we have to manually 
    // calculate it's size, thankfully, we already have code to calculate the size of a shape, which we use to 
    // make a tensor in the first place. That's why I can just copy the constructor code.
    if (TL2 != TL) 
    <% throw std::invalid_argument("Your dimensions are invalid. They must equal " + std::to_string(TL) + " when multiplied together."); %>
    shape_ = newShape; // and now that we know it's safe, we just make the old shape equal the new shape.
%>

// Convert the tensor into a one-dimensional tensor without changing its data.
// The new shape contains one dimension equal to the total number of elements.
template <typename T>
void Tensor<T>::flatten() <% shape_ = <% size() %>; %>

// Access an element for reading or modification.
template <typename T>
T& Tensor<T>::at(const std::vector<std::size_t>& indices) {
    if (indices.size() != shape_.size()) {
        throw std::invalid_argument(
            "The number of indices must match the number of tensor dimensions.");
    }

    std::size_t flatIndex = 0;

    for (std::size_t i = 0; i < indices.size(); ++i) {
        if (indices[i] >= shape_[i]) {
            throw std::out_of_range("Tensor index is out of range.");
        }

        flatIndex = flatIndex * shape_[i] + indices[i];
    }

    return data_.at(flatIndex);
}

// Access an element of a const tensor without allowing modification.
template <typename T>
const T& Tensor<T>::at(const std::vector<std::size_t>& indices) const {
    if (indices.size() != shape_.size()) {
        throw std::invalid_argument(
            "The number of indices must match the number of tensor dimensions.");
    }

    std::size_t flatIndex = 0;
    for (std::size_t i = 0; i < indices.size(); ++i) {
        if (indices[i] >= shape_[i]) {
            throw std::out_of_range("Tensor index is out of range.");
        }
        flatIndex = flatIndex * shape_[i] + indices[i];
    }

    return data_.at(flatIndex);
}

template <typename T>
Tensor<T> Tensor<T>::operator+(const Tensor<T>& other) const <%

    // first, we need to make sure that the shapes of the two tensors are the exact same
    // thankfully, this is made very simple by the vector class, since it overloads the "==" operator 
    // to check two or more vectors and make sure they are the same size, have the same elements, and that they're in the same order
    // so that's all my work done for me :D

    if (other.shape() != shape()) 
    <% throw std::invalid_argument("Your shapes do not match. You may only add tensors with the same shape."); %>

    Tensor<T> result(shape_); 
    const T maximum = std::numeric_limits<T>::max();
    const T minimum = std::numeric_limits<T>::lowest();
    std::size_t i = 0; while (i < size()) <%

        T a = data_<:i:>;
        T b = other.data_<:i:>;
        if constexpr (std::is_integral_v<T>) <%
            if ((b > 0 && a > maximum - b)) <%
                throw std::overflow_error("Your element " + std::to_string(b) + " is too large to be added to " + std::to_string(a) + ".");
            %>
            if constexpr (std::is_signed_v<T>) <%
                if (b < 0 && a < minimum - b) <%
                    throw std::overflow_error("Your element " + std::to_string(b) + " is too small to be added to " + std::to_string(a) + ".");
                %>
            %>
        %>
        
        result.data_<:i:> = a + b; 
        i++; 

    %> 
    
    return result;
%>

template <typename T>
Tensor<T> Tensor<T>::operator-(const Tensor<T>& other) const <%

    // first, we need to make sure that the shapes of the two tensors are the exact same
    // thankfully, this is made very simple by the vector class, since it overloads the "==" operator 
    // to check two or more vectors and make sure they are the same size, have the same elements, and that they're in the same order
    // so that's all my work done for me :D

    if (other.shape() != shape()) 
    <% throw std::invalid_argument("Your shapes do not match. You may only add tensors with the same shape."); %>

    Tensor<T> result(shape_); 
    const T maximum = std::numeric_limits<T>::max();
    const T minimum = std::numeric_limits<T>::lowest();
    std::size_t i = 0; while (i < size()) <% 
    
        T a = data_<:i:>;
        T b = other.data_<:i:>;
        if constexpr (std::is_integral_v<T>) <%
            if constexpr (std::is_signed_v<T>) <%
                if (b > 0 && a < minimum + b) <%
                    throw std::overflow_error("Your element " + std::to_string(b) + " is too big to subtract from " + std::to_string(a) + ".");
                %> 
                else if (b < 0 && a > maximum + b) <%
                    throw std::overflow_error("Your element " + std::to_string(b) + " is too small to subtract from " + std::to_string(a) + ".");
                %>
            %> 
            else if (a < b) <%
                throw std::overflow_error("You can't store negative numbers in an unsigned data type, and " + std::to_string(a) + " - " + std::to_string(b) + " produces a negative number.");
            %>
        %>
        
        result.data_<:i:> = a - b; 
        i++; 

    %> 
    
    return result;

%>

template <typename T>
Tensor<T> Tensor<T>::operator*(const Tensor<T>& other) const <%

    // first, we need to make sure that the shapes of the two tensors are the exact same
    // thankfully, this is made very simple by the vector class, since it overloads the "==" operator 
    // to check two or more vectors and make sure they are the same size, have the same elements, and that they're in the same order
    // so that's all my work done for me :D

    if (other.shape() != shape()) 
    <% throw std::invalid_argument("Your shapes do not match. You may only add tensors with the same shape."); %>

    Tensor<T> result(shape_);
    const T maximum = std::numeric_limits<T>::max();
    const T minimum = std::numeric_limits<T>::lowest(); 
    std::size_t i = 0; while (i < size()) <% 
    
        T a = data_<:i:>;
        T b = other.data_<:i:>;

        if constexpr (std::is_integral_v<T>) <%
            if constexpr (std::is_signed_v<T>) <%
                if (a > 0) <%
                    if (b > 0 && a > maximum / b) <%
                        // ex: max = 100, a = 20, b = 10, 100/10 = 10 < a, a(b) = 20(10) > 100
                        throw std::overflow_error(std::to_string(b) + " is too big to be multiplied to " + std::to_string(a) + ".");
                    %>
                    else if (b < 0 && b < minimum / a) <% // here specifically we're checking b < minimum / a instead of a > minimum / b because there is a special edge case if b = -1
                        // ex: min = -100, a = 20, b = -10, -100/-10 = 10 < a, a(b) = 20(-10) < -100
                        throw std::overflow_error(std::to_string(b) + " is too small to be multiplied to " + std::to_string(a) + ".");
                    %>
                %>
                else if (a < 0) <%
                    if (b > 0 && a < minimum / b) <%
                        // ex: min = -100, a = -20, b = 10, -100/10 = -10 > a, a(b) = -20(10) < -100
                        throw std::overflow_error(std::to_string(b) + " is too big to be multiplied to " + std::to_string(a) + ".");
                    %>
                    if (b < 0 && a < maximum / b) <%
                        // ex: max = 100, a = -20, b = -10, 100/-10 = -10 > a, a(b) = -20(-10) > 100
                        throw std::overflow_error(std::to_string(b) + " is too small to be multiplied to " + std::to_string(a) + ".");
                    %>
                %>
            %>
            else if (b > 0 && a > maximum / b) <%
                throw std::overflow_error(std::to_string(b) + " is too big to be multiplied to " + std::to_string(a) + ".");
            %>
            else if (b < 0) <%
                throw std::overflow_error(std::to_string(b) + " is too small to be multiplied to " + std::to_string(a) + ".");
                // the reason why I'm throwing an overflow error here instead of an invalid argument error is because technically
                // a negative b value a valid argument, but trying to multiply a unsigned data type to a negative signed data type 
                // will cause unsigned wrap around, because unsigned types can't store negative numbers, so tecnhically that's an 
                // overflow error because by trying to go below 0, you're flowing over the minimum limit and forcing a to wrap around
                // to an extremely large value. Think of how in old games, going below 0 would cause the number to shoot back to 255.
            %>
        %>
        
        result.data_<:i:> = a * b; 
        i++; 

    %> 
    
    return result;

%>

template <typename T>
T Tensor<T>::sum() const <%
    std::size_t i = 0;
    T total<%%>;
    while (i < size()) <%
        T value = data_<:i:>;

        if constexpr (std::is_integral_v<T>) <%
            if (value > 0 && total > std::numeric_limits<T>::max() - value) <%
                throw std::overflow_error("Sum exceeds the maximum value.");
            %>
        %>

        if constexpr (std::is_signed_v<T>) <%
            if (value < 0 && total < std::numeric_limits<T>::lowest() - value) <%
                throw std::overflow_error("Sum falls below the minimum value.");
            %>
        %>

        total += value;
        i++;
    %>
    return total;
%>

template <typename T>
double Tensor<T>::mean() const <% 

    if(size() == 0)<% throw std::domain_error("Cannot perform mean operation on empty tensor."); %>

    double total = sum(); return total/size(); 
    // we have to write "double total = sum()" first to avoid losing the decimals (as opposed to return sum()/size())

%>

template <typename T>
void Tensor<T>::fill() <%
    RandomNumberCreator rd;
    std::size_t i = 0;
    while (i < size()) <% data_<:i:> = rd.get_random_number(1, 100); i++; %>
%>

template <typename T>
void Tensor<T>::print_tensor() const<%

    // I thought about making a feature where the tensor would print visually showing it's dimensions but then I considered
    // that some insane person could create a 4D tensor and gave up

    std::size_t i = 0; while (i < size()) <% std::cout << data_<:i:> << " "; i++; %> std::cout << '\n';

%>
