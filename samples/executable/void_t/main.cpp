#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <iostream>
#include <vector>

class Sample {
public:
    using size_type = size_t;
    size_type   _size;
public:
    Sample() {
    }
    size_type size() const {
        return _size;
    }
};

template< class T, class = std::void_t<> >
struct has_sizetype : public std::false_type
{};
template< class T >
struct has_sizetype<T, std::void_t<typename T::size_type>> : std::true_type
{};

template< class T, class = std::void_t<> >
struct has_size_member : public std::false_type {
};

template< class T >
struct has_size_member< T, std::void_t<decltype(T::_size)> > : public std::true_type {
};

template< class T, class = std::void_t<> >
struct has_size_method : public std::false_type {
};

template< class T >
struct has_size_method<T, std::void_t< decltype(std::declval<T>().size())> > : public std::true_type {
};

template< class T, class = std::void_t<> > 
struct iterable : public std::false_type {
};

template< class T >
struct iterable<T, std::void_t< decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())> > : public std::true_type {
};

int main() {
    std::cout<<"has_sizetype<Sample>::value : "<< has_sizetype<Sample>::value <<std::endl;
    std::cout<<"has_size_member<Sample>::value : "<< has_size_member<Sample>::value <<std::endl;    
    std::cout<<"has_size_method<Sample>::value : "<< has_size_method<Sample>::value <<std::endl;
    std::cout<<"iterable<Sample>::value : "<< iterable<Sample>::value <<std::endl;
    std::cout<<"iterable<std::vector<int>>::value : "<< iterable<std::vector<int>>::value <<std::endl;
    return 0;
}