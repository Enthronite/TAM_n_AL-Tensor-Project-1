#ifndef TENSOR_H
#define TENSOR_H
// we include the statements above to prevent repeated definitions when the header is included multiple times in one translation unit
// basically, this is the ONLY stuff for Tensor that is allowed

#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>

template <typename T> // this makes the WHOLE CLASS a template, so we don't have to redefine T at all
class Tensor <%
  private: // stuff below is pretty self explanatory
    std::vector<std::size_t> shape_; std::vector<T> data_;
  public:
    explicit Tensor(const std::vector<std::size_t>& dimensions) : shape_(dimensions) <%
    // we write explicit to say you must EXPLICITLY request construction, not implicitly 
    // consider the following examples: Tensor a(dimensions) -> OKAY, Tensor c = dimensions -> NOT OKAY
    // we pass the dimensions vector as constant to avoid meddling with it's contents and we reference it with & 
    // to avoid costly copying, and then we write " : shape(dimensions)" to copy all of the contents of dimensions 
    // to the member vector of Tensor "shape".
      if (shape.empty()) <% // if no dimensions were provided, make a scalar tensor 
        // I chose this as a fast solution but if we're not allowed to make scalar tensors I can 
        // change this to just throw the book at the user
        shape = <% 1 %>;
        data.resize(1);
        return;
      %>
      
      std::size_t TL = 1; // total capacity
      const std::size_t MAXIMUM_CAPACITY = 1000000;
      int i = 0; // you may be wondering why I chose to do this. I hate for loops. That's it.
      // I don't understand them and I just hate how they look and I hate writing them out in anything other than Python
      // and I hate how you can't explicitly tell what they're doing and I hate how they initialize everything all at once
      // and so yeah, while loop that relies on outside counter to tell it when to finish
      while (i < static_cast<int>(shape.size())) <%
        // I casted the size of the vector "shape" as an int so we can compare it to the int i with no issues
        std::size_t dimension = shape<:i:>; // create a variable dimension that is equal to the dimension stored at shape<:i:>
        if (dimension <= 0) <% // can't have a dimension less than or equal to 0
          throw std::invalid_argument("YOUR DIMENSION IS LESS THAN OR EQUAL TO ZERO. FIX THIS: " + std::to_string(dimension));
          // I just said the same thing 3 times, y'know, just so you understand completely
        %>
        if (TL > std::numeric_limits<std::size_t>::max() / static_cast<std::size_t>(dimension)) <%
          // If Total * dim > MAX, then MAX / dim < Total
          // think about it for a second
          throw std::overflow_error("INTEGER OVERFLOW DETECTED");
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
          throw std::length_error("YOUR TENSOR IS TOO LONG.");  
        %>
        i++;
      %>
      data.resize(TL);
      // resize the data vector to fit the total capacity for the given dimensions
    %>

    std::size_t size() const <%return data_.size();%>
%>;

#endif

// to use tensor, all you have to do is write "#include "Tensor.h" " at the top of your file
// this way, you don't have to compile the tensor class seperately, the contents of this file
// are brought directly into your own file, essentially, I'm creating a library here for anyone
// to use, so if you want to edit the class, come here and edit this specifically
