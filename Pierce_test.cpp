// I will now begin working on this
// Warning: I have no idea what I'm doing and I have no idea how this code will interact with anything else
// so I'll probably need to fix a lot of stuff later on...

#include "Tensor.h"

#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <iostream>
// even though we don't need to rewrite the include statements, I'm doing so because it makes it clear 
// exactly what my file is using

template <typename T>
void Tensor<T>::reshape(const std::vector<std::size_t>& newShape) <%
    if (newShape.empty()) <%
        throw std::invalid_argument("Must provide new dimensions.");
    %>
    int i = 0;
    std::size_t TL = size(); // total capacity
    const std::size_t MAXIMUM_CAPACITY = 1000000;
    std::size_t TL2 = 1; // total capacity
    while (i < static_cast<int>(newShape.size())) <%
        std::size_t dimension = newShape<:i:>; 
        if (dimension <= 0) <% 
          throw std::invalid_argument("YOUR DIMENSION IS LESS THAN OR EQUAL TO ZERO. FIX THIS: " + std::to_string(dimension));
        %>
        if (TL2 > std::numeric_limits<std::size_t>::max() / static_cast<std::size_t>(dimension)) <%
          // If Total * dim > MAX, then MAX / dim < Total
          // think about it for a second
          throw std::overflow_error("INTEGER OVERFLOW DETECTED");
        %>
        TL2 *= dimension;
        if (TL2 > MAXIMUM_CAPACITY) <%
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
    if (TL2 != TL) <%
        throw std::invalid_argument("Your dimensions are invalid. They must equal " + std::to_string(TL) + " when multiplied together.");
    %>
    shape_ = newShape;
%>

int main() <%
    std::vector<std::size_t> test = <% 2, 3, 4 %>;
    Tensor<double> tester({2, 3, 4});
    tester.printInfo();
    std::vector<std::size_t> newTest = <% 6, 4 %>;
    tester.reshape(newTest);
    tester.printInfo();
%>
