#include <iostream>
#include "t_json.hpp"

using namespace tcx::json;

int main(int, char**){
    
    Value a = Object();
    a["field_1"] = 10;
    a["field_2"] = Array{1,2,3,4,5.5f};
    a["field_3"] = Object();
    a["field_3"]["child_1"] = 10.5f;
    a["field_3"]["child_2"] = Array({1,2,3,4,5,6});
    a["field_4"] = Null();
    a["field_4"] = false;
    a["field_5"] = "hello_t_json";

    a.Emplace("fiedld_6",Array{"hi",1,0,1.5f});

    std::cout << a.Stringfy()<<'\n';

    Object& obj = a.Cast<Object>();
    for(const auto&i:obj){
        std::cout << i.first<<" is "<< i.second.Stringfy()<<'\n';
    }

    
    std::system("pause");
}
