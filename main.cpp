#include <iostream>
#include <typeinfo>

template<typename Types>
void deal(const Types arg){
    // std::cout << arg << std::endl;
    std::cout << "type: " << typeid(arg).name() << std::endl;
}

template<typename T,typename... Types> 
void foo(const T t,const Types... rest){
    std::cout << sizeof...(Types) << std::endl;
    std::cout << sizeof...(rest) << std::endl;
    int arr[] = {(deal(rest), 0)...};

    std::cout << sizeof arr << std::endl;
    
}

struct Message
{
    char buf[100] = "message";
};


int main(){
    int i=111;
    short hi = 121;
    long li = 21212;
    long long lli = 1111111;
    char c = 'c';
    char *cstr = "hello";
    char cstr2[] = "hi";
    std::string str = "string";
    Message m;
    float f = 12.12;
    double d = 231.21;
    foo(1, i,hi,li,lli,c,cstr,cstr2,str,m,f,d,"const char *");
}