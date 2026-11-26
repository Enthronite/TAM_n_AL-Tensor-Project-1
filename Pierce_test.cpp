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
    try <%Tensor <int> Tensor_4 = tester + Tensor_1;std::cout<<"Fail: Invalid shapes added.\n";%>
    catch(const std::invalid_argument& error) <% std::cout << "Pass: " << error.what() << '\n'; %>
    //try tensor subtraction
    Tensor <int> Tensor_4 = Tensor_1 - Tensor_2; std::cout << "Tensor 4: " << '\n'; Tensor_4.print_tensor();
    //try tensor multiplication
    Tensor <int> Tensor_5 = Tensor_3 * Tensor_4; std::cout << "Tensor 5: " << '\n'; Tensor_5.print_tensor();
    //try tensor sum function - note: you have to print it manually since the function only returns a value
    //update: I'm making a new tensor to test this function since a tensor of random numbers makes it hard to tell if the sum is correct
    Tensor <int> Tensor_6(<% 2 %>); Tensor_6.fill(); std::cout << "Tensor 6: " << '\n'; Tensor_6.print_tensor();
    std::cout << "Sum of Tensor 6: " << Tensor_6.sum() << '\n';
    std::cout << "Mean of Tensor 6: " << Tensor_6.mean() << '\n';

    // below, I will attempt to catch overflow errors
    
    const int maximum = std::numeric_limits<int>::max();
    const int minimum = std::numeric_limits<int>::lowest();
    Tensor <int> one(<% 1 %>);
    Tensor <int> two(<% 1 %>);
    // test 1: addition maximum overflow
    one.set_test_value(0, maximum);
    two.set_test_value(0, 1);
    try <% Tensor<int> result =  one + two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 2: addition minimum overflow
    one.set_test_value(0, minimum);
    two.set_test_value(0, -1);
    try <% Tensor<int> result =  one + two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 3: subtraction minimum overflow
    one.set_test_value(0, minimum);
    two.set_test_value(0, 1);
    try <% Tensor<int> result =  one - two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 4: subtraction maximum overflow
    one.set_test_value(0, maximum);
    two.set_test_value(0, -1);
    try <% Tensor<int> result =  one - two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 5: multiplication maximum overflow
    one.set_test_value(0, maximum);
    two.set_test_value(0, 2);
    try <% Tensor<int> result =  one * two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 6: multiplication minimum overflow
    one.set_test_value(0, minimum);
    two.set_test_value(0, 2);
    try <% Tensor<int> result =  one * two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 7: minimum edge case
    // explanation: I figured it out. The minimum limit for a signed integer is greater than the maximum limit because 0 is counted as 
    // a negative number, meaning that if you multiply the minimum limit by -1, it will cause an integer overflow error because the 
    // returned value will be maximum + 1
    one.set_test_value(0, minimum);
    two.set_test_value(0, -1);
    try <% Tensor<int> result =  one * two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // test 8: minimum edge case for the opposite variable, variable a
    // basically, because of how my multiplication function works, we need to check that multiplying by negative 1 is 
    // rejected for both the second tensor and the first tensor in a multiplication operation
    one.set_test_value(0, -1);
    two.set_test_value(0, minimum); // noticed I swapped them
    try <% Tensor<int> result =  one * two; std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // Additional Tests
    // ADD Test 1: Max Boundary Test
    one.set_test_value(0, maximum);
    two.set_test_value(0, 0);
    try <% Tensor<int> result =  one + two; std::cout << "Pass: max value was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Fail: " << error.what() << '\n'; %>
    // ADD Test 2: Minimum Boundary Test
    one.set_test_value(0, minimum);
    two.set_test_value(0, 0);
    try <% Tensor<int> result =  one - two; std::cout << "Pass: min value was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Fail: " << error.what() << '\n'; %>
    // ADD Test 3: Max Mult Boundary Test
    one.set_test_value(0, maximum);
    two.set_test_value(0, 1);
    try <% Tensor<int> result =  one * two; std::cout << "Pass: max value was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Fail: " << error.what() << '\n'; %>
    // ADD Test 4: Min Mult Boundary Test
    one.set_test_value(0, minimum);
    two.set_test_value(0, 1);
    try <% Tensor<int> result =  one * two; std::cout << "Pass: min value was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Fail: " << error.what() << '\n'; %>
    // ADD Test 5: Multiply by 0 Test
    one.set_test_value(0, minimum);
    two.set_test_value(0, 0);
    try <% Tensor<int> result =  one * two; std::cout << "Pass: min*0 accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Fail: " << error.what() << '\n'; %>
    // ADD Test 6: Negative Max Test
    one.set_test_value(0, maximum);
    two.set_test_value(0, -1);
    try <% Tensor<int> result =  one * two; std::cout << "Pass: max*-1 accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Fail: " << error.what() << '\n'; %>
    // ADD Test 7: Sum Overflow Test Max + Min
    Tensor <int> ovsu(<% 2 %>);
    ovsu.set_test_value(0, maximum);
    ovsu.set_test_value(1, 1);
    try <% ovsu.sum(); std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    ovsu.set_test_value(0, minimum);
    ovsu.set_test_value(1, -1);
    try <% ovsu.sum(); std::cout << "Fail: overflow was accepted.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // ADD Test 8: Unsigned Int Subtraction Test
    Tensor <unsigned int> unsubtes1(<%1%>);
    Tensor <unsigned int> unsubtes2(<%1%>);
    unsubtes1.set_test_value(0, 1);
    unsubtes2.set_test_value(0, 2);
    try <% Tensor<unsigned int> result =  unsubtes1 - unsubtes2; std::cout << "Fail: unsigned wraparound was not caught.\n"; %>
    catch ( const std::overflow_error& error )
    <% std::cout << "Pass: " << error.what() << '\n'; %>
    // basic tensor test below
    Tensor <double> scallar;
    scallar.printInfo();
%>
