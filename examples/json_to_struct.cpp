#include <iostream>
#include "t_json.hpp"

struct Test_struct_1
{
    std::string _1;
    double _2;
};


struct Test_struct_2{
    std::string _1;
    int _2;
    double _3;
    Test_struct_1 _4;
};

T_J2S_BEGIN(Test_struct_1)
T_J2S_DEF(_1)
T_J2S_DEF(_2)
T_J2S_END()

T_J2S_BEGIN(Test_struct_2)
T_J2S_DEF(_1)
T_J2S_DEF(_2)
T_J2S_DEF(_3)
T_J2S_DEF(_4)
T_J2S_END()

using namespace tcx::json;

int main(){
    Value v = Object();
    v["_1"] = "hello";
    v["_2"] = 1;
    v["_3"] = 1.1;
    v["_4"] = Object();
    v["_4"]["_1"] = "world";
    v["_4"]["_2"] = 10;

    Test_struct_2 res = {};
    Structify(res,v);

    std::cout << res._1<<' ';
    std::cout << res._4._1<<'\n';

    return 0;
}