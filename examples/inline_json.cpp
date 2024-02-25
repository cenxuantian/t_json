#include <iostream>
#include "t_json.hpp"

int main(){
    auto js = T_INLINE_JSON(
        {
            "data_1":1,
            "data_2":true,
            "data_3":{
                "data_1":1.5,
                "data_2":false,
            },
            "data_4":[
                1,2,{},11.3,"hello",false
            ],
            "data_5":"string_value"
        }
        
        );

    std::cout << js.Stringfy()<<'\n';

    return 0;
}