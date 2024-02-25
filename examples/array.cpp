#include <iostream>
#include "t_json.hpp"

using namespace tcx::json;

int main(int, char**){

    Value a = Array();

    // push
    a.PushBack(10);
    a.PushBack(10.5f);
    a.PushBack("hello");

    // EmplaceBack is faster than PushBack when add Array and Object
    a.EmplaceBack(Array{"good",1232});
    a.EmplaceBack(Object());
    

    // access
    a[0] += 1;
    a[1] -=10;
    a[2] += " world";
    a[3][0] +=" morning";
    a[4]["child_0"] = false;
    a[4]["child_1"] = 1;
    // erase
    a.Erase(0);

    std::cout << a.Stringfy()<<'\n';

    // for each
    Array& arr = a.Cast<Array>();
    for(const auto&i:arr){
        std::cout << i.Stringfy()<<'\n';
    }



    std::system("pause");
}
