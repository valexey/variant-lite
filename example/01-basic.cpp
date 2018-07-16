#include "variant.hpp"

#include <cassert>
#include <string>
#include <iostream>

using namespace nonstd;

struct m1 {};
struct m2 {};
struct m3 {};
struct m4 {};
struct m5 {};
struct m6 {};
struct m7 {};
struct m8 {};
struct m9 {};
struct m10 {};
struct m11 {};
struct V {
	void operator()(char c)const{std::cout << c << std::endl;}
	void operator()(int i)const{std::cout << i << std::endl;}
	void operator()(long l)const{std::cout << l << std::endl;}
	void operator()(const std::string& s)const{std::cout << s << std::endl;}
	template <typename T> void operator()(const T&)const{}
};

int main()
{
    std::string hello = "hello, world"; 

    variant< char, int, long, std::string, m1, m2, m3, m4, m5, m6, m7, m8 > var;
    
		V v;
    var =  'v' ; assert( get<  0 >( var ) == 'v' );
	visit(v, var);
    var =  'v' ; assert( get<char>( var ) == 'v' );
	visit(v, var);
    var =   7  ; assert( get<int >( var ) ==  7  );
	visit(v, var);
    var =  42L ; assert( get<long>( var ) == 42L );    
	visit(v, var);
    var = hello; assert( get<std::string>( var ) == hello );
	visit(v, var);
}

#if 0
cl -EHsc -I../include/nonstd 01-basic.cpp && 01-basic.exe
g++ -std=c++98 -Wall -I../include/nonstd -o 01-basic.exe 01-basic.cpp && 01-basic.exe
#endif 
