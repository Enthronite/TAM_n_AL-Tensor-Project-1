#ifndef TENSOR_H
#define TENSOR_H
// we include the statements above to prevent repeated definitions when the header is included multiple times in one translation unit
// basically, this is the ONLY stuff for Tensor that is allowed

#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <iostream>

template <typename T> // this makes the WHOLE CLASS a template, so we don't have to redefine T at all
class Tensor <%
  private: // stuff below is pretty self explanatory
    std::vector<std::size_t> shape_; 
    std::vector<T> data_;

  public:


    // These 'at' overloads allow us to access an element using multidimensional indices.
    // The 1st overload returns a modifiable reference, allowing the element to
    // be read or changed. The 2nd overload is for 'const' tensors and returns a
    // read-only reference. Both throw an exception if the indices are invalid.
    T& at(const std::vector<std::size_t>& indices); 
    const T& at(const std::vector<std::size_t>& indices) const;

    explicit Tensor() <%
      // this is the basic Tensor constructor, for when you want to intitialize a simple scalar tensor
      // to use it, you simply write "Tensor();", and then you have an empty tensor with size 1
      data_.resize(1);
      return;
    %>

    explicit Tensor(const std::vector<std::size_t>& dimensions) : shape_(dimensions) <%
    // we write explicit to say you must EXPLICITLY request construction, not implicitly 
    // consider the following examples: Tensor a(dimensions) -> OKAY, Tensor c = dimensions -> NOT OKAY
    // we pass the dimensions vector as constant to avoid meddling with it's contents and we reference it with & 
    // to avoid costly copying, and then we write " : shape(dimensions)" to copy all of the contents of dimensions 
    // to the member vector of Tensor "shape".
      if (shape_.empty()) <% // if no dimensions were provided, make a scalar tensor
        // I chose this as a fast solution but if we're not allowed to make scalar tensors I can 
        // change this to just throw the book at the user
        // EDIT 11/15/26 : I just figured out a scalar tensor has a shape of NOTHING, not 1, so I removed the line
        // shape_ = <% 1 %>;, because leaving the shape as empty like this: <%%>, is what we need for a scalar tensor
        data_.resize(1);
        return;
      %>
      
      std::size_t TL = 1; // total capacity
      const std::size_t MAXIMUM_CAPACITY = 1000000;
      int i = 0; // you may be wondering why I chose to do this. I hate for loops. That's it.
      // I don't understand them and I just hate how they look and I hate writing them out in anything other than Python
      // and I hate how you can't explicitly tell what they're doing and I hate how they initialize everything all at once
      // and so yeah, while loop that relies on outside counter to tell it when to finish
      while (i < static_cast<int>(shape_.size())) <%
        // I casted the size of the vector "shape" as an int so we can compare it to the int i with no issues
        std::size_t dimension = shape_<:i:>; // create a variable dimension that is equal to the dimension stored at shape_<:i:>
        if (dimension <= 0) <% // can't have a dimension less than or equal to 0
          throw std::invalid_argument("Your dimension is less than or equal to zero. Fix this: " + std::to_string(dimension));
          // I just said the same thing 3 times, y'know, just so you understand completely
        %>
        if (TL > std::numeric_limits<std::size_t>::max() / static_cast<std::size_t>(dimension)) <%
          // If Total * dim > MAX, then MAX / dim < Total
          // think about it for a second
          throw std::overflow_error("Integer Overflow Detected.");
        %>
        TL *= dimension;
        if (TL > MAXIMUM_CAPACITY) <%
          // the reason why we check the size of TL twice is because of what happens when the total value gets too big:
          // firstly, we need to make sure the next value we multiply the total by doesn't exceed the integer bounds 
          // because if it does, the value of total will loop back around and give us a deceptively small number, 
          // basically it's like how on old tech whenever something would exceed 255, it would go back to 0, so 
          // but after that, we also need to make sure that the final total value isn't stupidly big because if it 
          // is, processing it is going to be a real pain, so we want to check if it exceeds our max capacity,
          // which is a number that I arbritarily decided rather than any actual limit
          throw std::length_error("Your tensor is too long.");  
        %>
        i++;
      %>
      data_.resize(TL);
      // resize the data vector to fit the total capacity for the given dimensions
    %>

    // The shape vector has one entry for each dimension.
    std::size_t ndim() const <%return shape_.size();%> // return the dimension size, this tells you if a tensor is 1D, 2D, 3D, etc.

    std::size_t size() const <%return data_.size();%> // return the actual size of the tensor

    // Return a read-only reference so callers cannot change the stored shape.
    const std::vector<std::size_t>& shape() const <%return shape_;%> // return the dimensions of the tensor

    void printInfo() const <% // prints the shape, dimensions, and size of a tensor, does NOT print the elements
      std::cout << "Shape: [";
      for (std::size_t i = 0; i < ndim(); ++i) <%
        if (i > 0) <%
          std::cout << ", ";
        %>
        std::cout << shape_[i];
      %>
      std::cout << "]\n"
                << "Dimensions: " << ndim() << '\n'
                << "Elements: " << size() << '\n';
    %>

    void reshape(const std::vector<std::size_t>& newShape); // apply new dimensions to a tensor but keep the size the same
    void flatten(); // turn a tensor into a 1D vector
    Tensor<T> operator+(const Tensor<T>& other) const; // add every element of two tensors
    Tensor<T> operator-(const Tensor<T>& other) const; // subtract every element of two tensors
    Tensor<T> operator*(const Tensor<T>& other) const; // multiply every element of two tensors
    T sum() const; // return sum of all elements
    double mean() const;
    // the function below is purely for testing. If I need to remove it later, please let me know before we turn it in.
    void fill();
    void print_tensor() const;
    void set_test_value(std::size_t index, T value) <% data_.at(index) = value; %>
%>;

#include "Tensor_imp.tpp"

#endif

// to use tensor, all you have to do is write "#include "Tensor.h" " at the top of your file
// this way, you don't have to compile the tensor class seperately, the contents of this file
// are brought directly into your own file, essentially, I'm creating a library here for anyone
// to use, so if you want to edit the class, come here and edit this specifically
