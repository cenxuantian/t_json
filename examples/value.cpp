#include <iostream>
#include "t_json.hpp"

using namespace tcx::json;

int main(int, char**){
    
    Value a = Object();
    a["field_1"] = 10;
    a["field_2"] = Array{1,2,3,4,5};
    a["field_3"] = Object();
    a["field_3"]["child_1"] = 10.5f;
    a["field_3"]["child_2"] = Array({1,2,3,4,5,6});
    a["field_4"] = Null();
    a["field_4"] = false;
    a["field_5"] = "hello_t_json";
    std::cout << a.Stringfy()<<'\n';

    // cast
    Number field_1 = a["field_1"].Cast<Number>();
    Array field_2 = a["field_2"].Cast<Array>();
    Object field_3 = a["field_3"].Cast<Object>();
    Boolean field_4 = a["field_4"].Cast<Boolean>();
    String field_5 = a["field_5"].Cast<String>();
    


    // caompare
    Value _1 = 10;
    Value _2 = 100;
    if(_1>_2){
        std::cout << "_1>_2\n";
    }else if(_1<_2){
        std::cout << "_1<_2\n";
    }else{
        std::cout << "_1=_2\n";
    }

    Value b = false;
    if(b.Cast<Boolean>()){
        std::cout << "b = true\n";
    }else{
        std::cout << "b = false\n";
    }

    std::system("pause");
}
