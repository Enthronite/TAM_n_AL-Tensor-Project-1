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
        if (dimension <= 0) <% 
          throw std::invalid_argument("Your dimension is less than or equal to zero. Fix this: " + std::to_string(dimension));
        %>
        if (TL2 > std::numeric_limits<std::size_t>::max() / static_cast<std::size_t>(dimension)) <%
          throw std::overflow_error("Integer Overflow Detected.");
        %>
        TL2 *= dimension;
        if (TL2 > MAXIMUM_CAPACITY) <%
          throw std::length_error("Your tensor is too long.");  
        %>
        i++;
    %>
    // you may notice that I literally just copied the code that I used to initialize a tensor in the class. 
    // that's because like I said before, all we need to do is verify the new shape is the same as the old shape.
    // since the new shape vector doesn't have a size() function like the old shape does, we have to manually 
    // calculate it's size, thankfully, we already have code to calculate the size of a shape, which we use to 
    // make a tensor in the first place. That's why I can just copy the constructor code.
    if (TL2 != TL) <%
        throw std::invalid_argument("Your dimensions are invalid. They must equal " + std::to_string(TL) + " when multiplied together.");
    %>
    shape_ = newShape; // and now that we know it's safe, we just make the old shape equal the new shape.
%>


template <typename T>
void Tensor<T>::flatten() <%
    shape_ = <% size() %>;
%>

int main() <%
    try <% Tensor<int> invalid(<%2, 0, 4%>);std::cout<<"FAIL: zero dimension was accepted.\n";%>
    catch(const std::invalid_argument& error)<%std::cout<<"PASS: zero dimension rejected: " << error.what()<<'\n';%>
    std::vector<std::size_t> test = <% 2, 3, 4 %>;
    Tensor<double> tester({2, 3, 4});
    tester.printInfo();
    std::vector<std::size_t> newTest2 = <% 7, 4 %>;
    try <% tester.reshape(newTest2);%>
    catch(const std::exception& error) <% std::cerr << "Reshape failed: " << error.what() << '\n'; %>
// this is very important: you NEED to put your tests for invalid input in a try catch block to prevent the code 
// from crashing when it encounters an exception
    std::vector<std::size_t> newTest = <% 6, 4 %>;
    tester.reshape(newTest);
    tester.printInfo();
    tester.flatten();
    tester.printInfo(); 
%>
