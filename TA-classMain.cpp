template <typename T>

class Tensor {
private :
std :: vector <std :: size_t > shape_ ;
std :: vector <T> data_;
public :

Tensor ();

Tensor (const std :: vector <std :: size_t >& shape);

std :: size_t ndim () const;
std :: size_t size /// Really Write now!!
const std :: vector <std :: size_t >& shape () const;
T& at(const std :: vector <std :: size_t >& indices );
const T& at(const std :: vector <std :: size_t >& indices ) const;
void reshape (const std :: vector <std :: size_t >& newShape );
void flatten ();
Tensor <T> operator +( const Tensor <T>& other) const;
Tensor <T> operator -( const Tensor <T>& other) const;
Tensor <T> operator *( const Tensor <T>& other) const; // element -wise
T sum () const;
double mean () const;
void printInfo () const;
//WAS HERE!!!!
};
