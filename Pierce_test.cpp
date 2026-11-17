// I will now begin working on this
// Warning: I have no idea what I'm doing and I have no idea how this code will interact with anything else
// so I'll probably need to fix a lot of stuff later on...

// EDIT: I don't need to write any function in this cpp file at all since they're all in the tensor class through the 
// .tpp file, which means this file is now exclusively a testing file. It really is like Java.

#include "Tensor.h"
#include "rng.h"

int main() <%
    //testing invalid tensor parameters
    try <% Tensor<int> invalid(<%2, 0, 4%>);std::cout<<"FAIL: zero dimension was accepted.\n";%>
    catch(const std::invalid_argument& error)<%std::cout<<"PASS: zero dimension rejected: " << error.what()<<'\n';%>
    //creating tensor tester by making the shape and then the tensor
    std::vector<std::size_t> test = <% 2, 3, 4 %>; Tensor<int> tester(test);
    //print info to confirm it worked
    tester.printInfo();
    //create new shape
    std::vector<std::size_t> newTest2 = <% 7, 4 %>;
    //test reshaping tensor tester with different element size
    try <% tester.reshape(newTest2);%>
    catch(const std::exception& error) <% std::cerr << "Reshape failed: " << error.what() << '\n'; %>
    // this is very important: you NEED to put your tests for invalid input in a try catch block to prevent the code 
    // from crashing when it encounters an exception
    
    //create tensor shape with valid size
    std::vector<std::size_t> newTest = <% 6, 4 %>;
    //try to reshape tensor tester
    tester.reshape(newTest);
    tester.printInfo();
    tester.flatten();
    tester.printInfo(); 

    // new tests

    // create shapes
    std::vector<std::size_t> A = <% 2, 1, 4 %>; std::vector<std::size_t> B = A;
    //create tensors
    Tensor<int> Tensor_1(A); Tensor<int> Tensor_2(B);
    //fill tensors with random values
    Tensor_1.fill(); Tensor_2.fill();
    //print tensor 1
    std::cout << "Tensor 1: " << '\n'; Tensor_1.print_tensor();
    //print tensor 2
    std::cout << "Tensor 2: " << '\n'; Tensor_2.print_tensor();
    //create a new tensor by adding tensor 1 and 2 together
    Tensor <int> Tensor_3 = Tensor_1 + Tensor_2; std::cout << "Tensor 3: " << '\n'; Tensor_3.print_tensor();
    //test invalid tensor addition
    try <%Tensor <int> Tensor_4 = tester + Tensor_1;std::cout<<"FAIL: Invalid shapes added.\n";%>
    catch(const std::invalid_argument& error) <% std::cout << "PASS: " << error.what() << '\n'; %>
    
%>
