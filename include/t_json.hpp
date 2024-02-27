/*
* 
This is a JSON formatter
Written by Cenxuan_Tian
Email: cenxuan.tian@outlook.com

MIT License

Copyright (c) 2024 Cenxuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


*/

// #if __cplusplus < 201703L
// #error "please use c++17 standard"
// #else

#pragma once
#ifndef __TCX_T_JSON_H__
#define __TCX_T_JSON_H__ 1
// c++ headers
#include <iostream> // for throw
#include <string>
#include <sstream>

// optional
// use this macro to enable transform from stl to Object or Value
#ifdef TCX_ENABLE_STL_TRANS
#include <stack>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>
#include <deque>
#endif


namespace tcx { namespace json {
// containers
// predef

template <class _Tp> class __LinkList;  // linklist class the container of Array
template<typename _Tp> class __LinkStack;   // stack based on linklist
template<class _Tp> class __Node;        // linklist node class
template <class _Tp> class __LinkList_iter; // linklist iter class
template <class _Tp1,class _Tp2> class __OrderedMap;    // ordered map class based on linklist
class __JSON_String_Paeser;       // json paser class

enum class Var;
enum class Type;
class Value;  // interface Value
typedef Value& RefValue;
struct Null;    // interface Null
struct Boolean; // interface Boolean
typedef double Number;  // interface Number based on double
typedef std::string String; // interface String based on std::string
typedef __LinkList<Value> Array;    // interface Array based on __LinkList
typedef __OrderedMap<String, Value> Object; // inter face object based on __OrderedMap


enum class Var {
    V_NULL = -2,        // JavaScript null value
    V_UNDEFINED = -1,   // JavaScript undefined value
    V_FALSE = 0,        // JavaScript false
    V_TRUE = 1,         // JavaScript true
};
enum class Type {
    T_NULL = 2, // JavaScript null
    T_NUMBER = 3,// JavaScript Number
    T_STRING = 4,// JavaScript String
    T_ARRAY = 5,// JavaScript Array
    T_OBJECT = 6,// JavaScript Object
    T_BOOLEAN = 7// JavaScript Boolean
};

// type traits ----------------------

// check if the type is a valid tcx::json::type
template<typename T> constexpr bool _ValidType_V = false;
template<> constexpr bool _ValidType_V<Number> = true;
template<> constexpr bool _ValidType_V<Boolean> = true;
template<> constexpr bool _ValidType_V<String> = true;
template<> constexpr bool _ValidType_V<Null> = true;
template<> constexpr bool _ValidType_V<Value> = true;
template<> constexpr bool _ValidType_V<Array> = true;
template<> constexpr bool _ValidType_V<Object> = true;



// get the type value enum (varible) of a type
template<typename T> constexpr Type _ToType_V = Type::T_NULL;
template<> constexpr Type _ToType_V<Number>  = Type::T_NUMBER;
template<> constexpr Type _ToType_V<Boolean> = Type::T_BOOLEAN;
template<> constexpr Type _ToType_V<String>  = Type::T_STRING;
template<> constexpr Type _ToType_V<Null>  = Type::T_NULL;
template<> constexpr Type _ToType_V<Array> = Type::T_ARRAY;
template<> constexpr Type _ToType_V<Object> = Type::T_OBJECT;

// define type to json type
#define ___REGISTER_TCX_TYPE_TO_JSON_0(__InPutType,__OutPutType)\
    template<> struct _ToJson<__InPutType> {\
        typedef __OutPutType Type;}

// define with 1 template
#define ___REGISTER_TCX_TYPE_TO_JSON_1(__InPutType,__OutPutType)\
    template<typename __Change_Type_1> struct _ToJson<__InPutType<__Change_Type_1>> {\
        typedef __OutPutType Type;}

// define with 2 template
#define ___REGISTER_TCX_TYPE_TO_JSON_2(__InPutType,__OutPutType)\
    template<typename __Change_Type_1, typename __Change_Type_2> struct _ToJson<__InPutType<__Change_Type_1, __Change_Type_2>> {\
        typedef std::conditional_t<std::is_same_v<typename _ToJson<__Change_Type_1>::Type, String>, __OutPutType, Null> Type;};


template<typename T> struct _ToJson {typedef Null Type;};
// number types
___REGISTER_TCX_TYPE_TO_JSON_0(unsigned char,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(char,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(unsigned short,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(short,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(unsigned int,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(int,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(unsigned long,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(long,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(unsigned long long,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(long long,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(double,Number);
___REGISTER_TCX_TYPE_TO_JSON_0(float,Number);

// string types
___REGISTER_TCX_TYPE_TO_JSON_0(unsigned char*,String);
___REGISTER_TCX_TYPE_TO_JSON_0(char*,String);
___REGISTER_TCX_TYPE_TO_JSON_0(const unsigned char*,String);
___REGISTER_TCX_TYPE_TO_JSON_0(const char*,String);
___REGISTER_TCX_TYPE_TO_JSON_0(std::string,String);

// boolean types
___REGISTER_TCX_TYPE_TO_JSON_0(bool,Boolean);
___REGISTER_TCX_TYPE_TO_JSON_0(Boolean,Boolean);

// array types
#ifdef _GLIBCXX_VECTOR
___REGISTER_TCX_TYPE_TO_JSON_1(std::vector,Array);
___REGISTER_TCX_TYPE_TO_JSON_1(const std::vector,Array);
#endif
#ifdef _GLIBCXX_QUEUE
___REGISTER_TCX_TYPE_TO_JSON_1(std::queue,Array);
___REGISTER_TCX_TYPE_TO_JSON_1(const std::queue,Array);
#endif
#ifdef _GLIBCXX_STACK
___REGISTER_TCX_TYPE_TO_JSON_1(std::stack,Array);
___REGISTER_TCX_TYPE_TO_JSON_1(const std::stack,Array);
#endif
#ifdef _GLIBCXX_LIST
___REGISTER_TCX_TYPE_TO_JSON_1(std::list,Array);
___REGISTER_TCX_TYPE_TO_JSON_1(const std::list,Array);
#endif
___REGISTER_TCX_TYPE_TO_JSON_1(__LinkList,Array);
___REGISTER_TCX_TYPE_TO_JSON_1(const __LinkList,Array);
#ifdef _GLIBCXX_DEQUE
template<typename ItemType> struct _ToJson<std::deque<ItemType,std::allocator<ItemType>>> {typedef Array Type;};
template<typename ItemType> struct _ToJson<const std::deque<ItemType,std::allocator<ItemType>>> {typedef Array Type;};
#endif
// object types
#ifdef _GLIBCXX_UNORDERED_MAP
___REGISTER_TCX_TYPE_TO_JSON_2(std::unordered_map,Object)
___REGISTER_TCX_TYPE_TO_JSON_2(const std::unordered_map,Object)
#endif
#ifdef _GLIBCXX_MAP
___REGISTER_TCX_TYPE_TO_JSON_2(std::map,Object)
___REGISTER_TCX_TYPE_TO_JSON_2(const std::map,Object)
#endif
___REGISTER_TCX_TYPE_TO_JSON_2(__OrderedMap,Object)
___REGISTER_TCX_TYPE_TO_JSON_2(const __OrderedMap,Object)

// uitilities
// these macros will be undef at the end of this file, so do not call this macros out of this file
#define __switch(_enum) switch(_enum){
#define __match(_enum_v,_code) case _enum_v:{ _code; break;}
#define __return_match(_enum_v,_rtn_v) case _enum_v:{ return _rtn_v;}
#define __match_(_code) default:{ _code; break;}    // match default
#define __return_default(_rtn_v) default:{ return _rtn_v;}
#define __switch_end()  }
#define __return_if(_con,_rtn_v) {if(_con) return _rtn_v;}
#define _C(_T) Cast<_T>()
#define _RTN_SELF() return *this

// Class Interfaces----------------------------------------------------------------------------------------
// Container node



// when object of this class be deleted, all children will be deleted
// when object of this class be copied, all children will be copied
template<class _Tp>
class __Node{
friend class __LinkList<_Tp>;
friend class __LinkStack<_Tp>;
friend class __LinkList_iter<_Tp>;

private:
    __Node<_Tp>* father = nullptr;
    __Node<_Tp>* child = nullptr;
    __Node<_Tp>* root = nullptr;
    _Tp* data = nullptr;
    __Node() noexcept{
        this->root = this;
        this->child = nullptr;
        this->data = nullptr;
    }
    __Node(const _Tp& data){
        this->root = this;
        this->child = nullptr;
        this->data = new _Tp(data);
    }
    __Node(_Tp&& data){
        this->root = this;
        this->child = nullptr;
        this->data = new _Tp(std::forward<_Tp>(data));
    }
    ~__Node(){
        if(this->child){
            delete this->child;
            this->child = nullptr;
            this->father = nullptr;
            this->root = this;
        }
        if(this->data){
            delete this->data;
            this->data = nullptr;
        }
        return;
    }
    __Node(const __Node& other){    // equal to other
        this->root = this;
        this->child = nullptr;

        if(other.data){
            this->data = new _Tp(*(other.data)); // copy all other values
        }else{
            this->data = nullptr;
        }
        if(other.child){
            this->child = new __Node<_Tp>(*(other.child));
        }
    }
    __Node(__Node&& other) noexcept{
        this->root = this;
        this->father = nullptr;

        this->child = other.child;
        this->data = other.data;
        
        other.father->child= nullptr;

        other.data = nullptr;
        other.child = nullptr;
    }
    __Node& operator=(const __Node& other){
        if(this->data){
            delete this->data;
            // std::cout << "delete\n";
            this->data = nullptr;
        }
        if(other.data){
            this->data = new _Tp(*(other.data));
        }
        if(this->child){
            delete this->child;
            this->child = nullptr;
        }
        if(other.child){
            this->child = new __Node<_Tp>(*(other.child));
        }
        _RTN_SELF();
    }
    __Node& operator=(__Node&& other) noexcept {
        this->child = other.child;
        this->data = other.data;
        other.father->child= nullptr;
        other.data = nullptr;
        other.child = nullptr;
        _RTN_SELF();
    }
    bool data_equal(const __Node& other) const {
        if(this->data == other.data)return true;
        else{
            if(!this->data||!other.data)return false;
            else return std::equal(*(this->data),*(other.data));
        }
    }
    bool operator==(const __Node& other) const noexcept{
        return  this->father == other.father
                &&this->child == other.child
                &&this->data == other.child
                &&this->root == other.root;
    }

};
// Linklist iter
template <class _Tp>
class __LinkList_iter{
friend class __LinkList<_Tp>;
private:
    __Node<_Tp>* cur_node = nullptr;
public:
    __LinkList_iter()noexcept :cur_node(nullptr){}
    __LinkList_iter(__Node<_Tp>* node)noexcept :cur_node(node){}
    ~__LinkList_iter()noexcept {}
    __LinkList_iter& operator=(const __LinkList_iter& other)noexcept {
        cur_node = other.cur_node;
    }
    __LinkList_iter& operator=(__LinkList_iter&& other) noexcept{
        cur_node = other.cur_node;
    }
    _Tp& operator*()noexcept{
        return *(cur_node->data);
    }
    const _Tp& operator*() const noexcept{
        return *(cur_node->data);
    }
    __LinkList_iter& operator++() noexcept{
        cur_node = cur_node->child;
        _RTN_SELF();
    }
    __LinkList_iter& operator++(int) noexcept{
        cur_node = cur_node->child;
        _RTN_SELF();
    }
    bool operator==(const __LinkList_iter& other) const noexcept{
        return cur_node == other.cur_node;
    }
    bool operator!=(const __LinkList_iter& other)const noexcept{
        return cur_node != other.cur_node;
    }
    __LinkList_iter& operator+(size_t pos) noexcept{
        for(size_t i=0;i<pos;i++){
            (*this)++;
        }
        _RTN_SELF();
    }
};
// linklist
template <class _Tp>
class __LinkList{
protected:
    __Node<_Tp>* root;
    __Node<_Tp>* __get_last_child() noexcept{
        __Node<_Tp>* cur_node = this->root;
        while(cur_node->child){
            cur_node = cur_node->child;
        }
        return cur_node;
    }
public:
    __LinkList():root(new __Node<_Tp>()){

    }
    ~__LinkList(){
        delete root;
    }
    __LinkList(const __LinkList& other){
        this->root = new __Node<_Tp>(*(other.root));
    }
    __LinkList(__LinkList&& other){
        this->root = new __Node<_Tp>(*(other.root));
    }
#ifdef _GLIBCXX_VECTOR
    template<typename _Other_Type> __LinkList(const std::vector<_Other_Type>& data):root(new __Node<_Tp>()){
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
    }
    template<typename _Other_Type> __LinkList& operator=(const std::vector<_Other_Type>& data){
        delete root;
        this->root = new __Node<_Tp>();
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
        _RTN_SELF();
    }
#endif
#ifdef _GLIBCXX_QUEUE
    template<typename _Other_Type> __LinkList(const std::queue<_Other_Type>& data):root(new __Node<_Tp>()){
        std::queue<_Other_Type> __in_queue_=  data;
        auto last = this->root;
        while (!__in_queue_.empty())
        {
            last->child = new __Node<_Tp>(__in_queue_.front());
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
            __in_queue_.pop();
        }
    }
    template<typename _Other_Type> __LinkList& operator=(const std::queue<_Other_Type>& data){
        delete root;
        this->root = new __Node<_Tp>();
        std::queue<_Other_Type> __in_queue_=  data;
        auto last = this->root;
        while (!__in_queue_.empty())
        {
            last->child = new __Node<_Tp>(__in_queue_.front());
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
            __in_queue_.pop();
        }
        _RTN_SELF();
    }
#endif
#ifdef _GLIBCXX_LIST
    template<typename _Other_Type> __LinkList(const std::list<_Other_Type>& data):root(new __Node<_Tp>()){
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
    }
    template<typename _Other_Type> __LinkList& operator=(const std::list<_Other_Type>& data){
        delete root;
        this->root = new __Node<_Tp>();
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
        _RTN_SELF();
    }
#endif
#ifdef _GLIBCXX_STACK
    template<typename _Other_Type> __LinkList(const std::stack<_Other_Type>& data):root(new __Node<_Tp>()){
        std::stack<_Other_Type> __in_stack;
        auto last = this->root;
        while (!__in_stack.empty())
        {
            last->child = new __Node<_Tp>(__in_stack.top());
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
            __in_stack.pop();
        }
        this->reverse();
    }
    template<typename _Other_Type> __LinkList& operator=(const std::stack<_Other_Type>& data){
        delete root;
        this->root = new __Node<_Tp>();
        std::stack<_Other_Type> __in_stack=  data;
        auto last = this->root;
        while (!__in_stack.empty())
        {
            last->child = new __Node<_Tp>(__in_stack.top());
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
            __in_stack.pop();
        }
        _RTN_SELF();
    }
#endif
#ifdef _GLIBCXX_DEQUE
    template<typename _Other_Type> __LinkList(const std::deque<_Other_Type,std::allocator<_Other_Type>>& data):root(new __Node<_Tp>()){
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
    }
    template<typename _Other_Type> __LinkList& operator=(const std::deque<_Other_Type,std::allocator<_Other_Type>>& data){
        delete root;
        this->root = new __Node<_Tp>();
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
        _RTN_SELF();
    }
#endif
    __LinkList(const std::initializer_list<_Tp>& data):root(new __Node<_Tp>()){
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
    }
    __LinkList& operator=(const std::initializer_list<_Tp>& data){
        delete root;
        this->root = new __Node<_Tp>();
        auto last = this->root;
        for(const auto&i:data){
            last->child = new __Node<_Tp>(i);
            last->child->child = nullptr;
            last->child->father = last;
            last->child->root = this->root;
            last = last->child;
        }
        _RTN_SELF();
    }
    __LinkList& operator=(const __LinkList& other){
        delete root;
        this->root = new __Node<_Tp>(*(other.root));
        _RTN_SELF();
    }
    __LinkList& operator=(__LinkList&& other){
        delete root;
        this->root = new __Node<_Tp>(*(other.root));
        _RTN_SELF();
    }
    size_t size()const noexcept{
        size_t cur_size = 0;
        __Node<_Tp>* cur_node = this->root;
        while(cur_node->child){
            cur_size++;
            cur_node = cur_node->child;
        }
        return cur_size;
    }
    _Tp& operator[](size_t pos){
        if(!this->root->child){
            throw std::out_of_range("");
        }
        size_t cur_pos = 0;
        __Node<_Tp>* cur_node = this->root->child;
        while(cur_node->child && pos>cur_pos){
            cur_pos++;
            cur_node = cur_node->child;
        }
        if(pos != cur_pos){
            throw std::out_of_range("");
        }
        return *(cur_node->data);
    }
    const _Tp& operator[](size_t pos) const {
        if(!this->root->child){
            throw std::out_of_range("");
        }
        size_t cur_pos = 0;
        __Node<_Tp>* cur_node = this->root->child;
        while(cur_node->child&& pos>cur_pos){
            cur_pos++;
            cur_node = cur_node->child;
        }
        return *(cur_node->data);
    }
    _Tp& at(size_t pos){
        return this->operator[](pos);
    }
    const _Tp& at(size_t pos)const{
        return this->operator[](pos);
    }
    void push_back(const _Tp& data){
        auto last = __get_last_child();
        last->child = new __Node<_Tp>(data);
        last->child->father = last;
        last->child->root = this->root;
    }
    void emplace_back(_Tp&& data){
        auto last = __get_last_child();
        last->child = new __Node<_Tp>(data);
        last->child->father = last;
        last->child->root = this->root;
    }
    void erase(size_t pos){
        if(!this->root->child){
            throw std::out_of_range("");
        }
        size_t cur_pos = 0;
        __Node<_Tp>* cur_node = this->root->child;
        while(cur_node->child && pos>cur_pos){
            cur_pos++;
            cur_node = cur_node->child;
        }
        if(pos != cur_pos){
            throw std::out_of_range("");
        }
        auto cur_child = cur_node->child;
        auto cur_father = cur_node->father;
        cur_father->child = cur_child;

        if(cur_child){
            cur_child->father = cur_father;
        }
        cur_node->child = nullptr;  // aviod child be deleted
        delete cur_node;
    }
    void erase(const __LinkList_iter<_Tp>& iter){
        __Node<_Tp>* cur_node = iter.cur_node;
        auto cur_child = cur_node->child;
        auto cur_father = cur_node->father;
        cur_father->child = cur_child;

        if(cur_child){
            cur_child->father = cur_father;
        }
        cur_node->child = nullptr;
        delete cur_node;
    }
    void clear(){
        delete root;
        root = new __Node<_Tp>();
    }
    __LinkList_iter<_Tp> begin() noexcept{
        if(root->child){
            return root->child;
        }else{
            return nullptr;
        }
    }
    __LinkList_iter<_Tp> end() noexcept{
        return nullptr;
    }
    __LinkList_iter<_Tp> begin() const noexcept{
        if(root->child){
            return root->child;
        }else{
            return nullptr;
        }
    }
    __LinkList_iter<_Tp> end() const noexcept{
        return nullptr;
    }
    bool operator==(const __LinkList& other)const{
        auto t_begin = this->begin();
        auto o_begin = other.begin();
        auto t_end = this->end();
        auto o_end = other.end();
        while (t_begin!=t_end && o_begin !=o_end)
        {
            if(*(t_begin) != *(o_begin)){
                return false;
            }
            t_begin++;
            o_begin++;
        }
        if((t_begin!=t_end)||(o_begin!=o_end)){
            return false;
        }
        return true;
    }
    bool operator!=(const __LinkList& other)const noexcept{
        return !this->operator==(other);
    }
    void reverse() noexcept{
        __Node<_Tp>* cur_node = __get_last_child();
        if(cur_node == root)return;
        root->child = cur_node;
        while (1)
        {
            __Node<_Tp>* next = cur_node->father;
            __Node<_Tp>* cur_father = cur_node->father;
            __Node<_Tp>* cur_child = cur_node->child;
            cur_node->father = cur_child?cur_child:root;
            cur_node->child = cur_father;
            if(next->father == root){
                next->father = cur_node;
                next->child = nullptr;
                break;
            }else{
                cur_node = next;
            }
        }
        return;
    }
};

template<typename _Tp>
class __LinkStack:public __LinkList<_Tp>{
public:
    _Tp& top(){
        return *(this->__get_last_child()->data);
    }
    void pop(){
        __Node<_Tp>* last = this->__get_last_child();
        last->father->child = nullptr;
        delete last;
    }
    void push(const _Tp& data){
        this->push_back(data);
    }
    void emplace(_Tp&& data){
        this->emplace_back(std::forward<_Tp>(data));
    }
};

// Orderdmap based on linklist
template <class _Tp1,class _Tp2>
class __OrderedMap{
private:
    __LinkList<std::pair<_Tp1,_Tp2>> data;
public:
    __OrderedMap()noexcept{}
    ~__OrderedMap()noexcept{}
    __OrderedMap(const __OrderedMap& other)noexcept{
        this->data = other.data;
    }
    __OrderedMap(__OrderedMap&& other){
        this->data = std::forward(other.data);
    }
#ifdef _GLIBCXX_UNORDERED_MAP
    __OrderedMap(const std::unordered_map<_Tp1,_Tp2>& _uMap){
        for(const auto&i:_uMap){
            data.emplace_back(i);
        }
    }
#endif
#ifdef _GLIBCXX_MAP
    __OrderedMap(const std::map<_Tp1,_Tp2>& _Map){
        for(const auto&i:_Map){
            data.emplace_back(i);
        }
    }
#endif

    __OrderedMap(const std::initializer_list<std::pair<_Tp1,_Tp2>>& _iniList){
        for(const auto&i:_iniList){
            data.emplace_back(i);
        }
    }
    
    __OrderedMap& operator=(const __OrderedMap& other){
        this->data = other.data;
        _RTN_SELF();
    }
    __OrderedMap& operator=(__OrderedMap&& other){
        this->data = std::move(other.data);
        _RTN_SELF();
    }
#ifdef _GLIBCXX_UNORDERED_MAP
    __OrderedMap& operator=(const std::unordered_map<_Tp1,_Tp2>& _uMap){
        data ={};
        for(const auto&i:_uMap){
            data.emplace_back(i);
        }
        _RTN_SELF();
    }
#endif
#ifdef _GLIBCXX_MAP
    __OrderedMap& operator=(const std::map<_Tp1,_Tp2>& _uMap){
        data ={};
        for(const auto&i:_uMap){
            data.emplace_back(i);
        }
        _RTN_SELF();
    }
#endif
    __OrderedMap& operator=(const std::initializer_list<std::pair<_Tp1,_Tp2>>& _uMap){
        data ={};
        for(const auto&i:_uMap){
            data.emplace_back(i);
        }
        _RTN_SELF();
    }
    _Tp2& at(const _Tp1& _key){
        for(std::pair<_Tp1,_Tp2>&i:data){
            if(i.first == _key){
                return i.second;
            }
        }
        std::string _error_of_t__c__x__ = "no key named "+_key;
        throw std::out_of_range(_error_of_t__c__x__);
    }
    const _Tp2& at(const _Tp1& _key) const{
        for(const std::pair<_Tp1,_Tp2>&i:data){
            if(i.first == _key){
                return i.second;
            }
        }
        std::string error = "no key named "+_key;
        throw std::out_of_range(error);
    }
    int count(const _Tp1& _key) const noexcept{
        for(const std::pair<_Tp1,_Tp2>&i:data){
            if(i.first == _key){
                return 1;
            }
        }
        return 0;
    }
    _Tp2& operator[](const _Tp1& _key){
        return this->at(_key);
    }
    const _Tp2& operator[](const _Tp1& _key)const{
        return this->at(_key);
    }
    bool operator==(const __OrderedMap& other) const noexcept(noexcept(this->data == other.data)){
        if(this->data == other.data)return true;
        else return false;
    }
    __OrderedMap& emplace(const std::pair<_Tp1,_Tp2>& item)noexcept(noexcept(this->data.emplace_back(item))){
        for(std::pair<_Tp1,_Tp2>&i:data){
            if(i.first == item.first){
                i.second = item.second;
                _RTN_SELF();
            }
        }
        this->data.emplace_back(item);
        _RTN_SELF();
    }
    __OrderedMap& emplace(_Tp1 const& _key ,_Tp2&& _val)
        noexcept(
            noexcept(std::declval<std::pair<_Tp1,_Tp2>&>().second = std::forward<_Tp2>(_val))&&
            noexcept(this->data.emplace_back(std::pair<_Tp1,_Tp2>(_key,std::forward<_Tp2>(_val)))))
    {
        // if exist change
        for(std::pair<_Tp1,_Tp2>&i:data){
            if(i.first == _key){
                i.second = std::forward<_Tp2>(_val);
                _RTN_SELF();
            }
        }
        // if not exist
        this->data.emplace_back(std::pair<_Tp1,_Tp2>(_key,std::forward<_Tp2>(_val)));
        _RTN_SELF();
    }
    auto begin() noexcept(noexcept(this->data.begin())){
        return this->data.begin();
    }
    auto end() noexcept(noexcept(this->data.end())){
        return this->data.end();
    }
    const auto begin()const noexcept(noexcept(this->data.begin())){
        return this->data.begin();
    }
    const auto end() const noexcept(noexcept(this->data.end())){
        return this->data.end();
    }
    size_t size()const noexcept{
        return data.size();
    }
    __OrderedMap& erase(const _Tp1& _key) {
        auto start = data.begin();
        auto end = data.end();
        while (start!=end)
        {
            if((*start).first == _key){
                data.erase(start);
                break;
            }
            start++;
        }
        _RTN_SELF();
    }
};
// null type
struct Null {
    inline static const Var value = Var::V_NULL;
    Null() {}
    template<typename T>
    Null(T) {}
};

struct Boolean{
public:
    Var value;
    Boolean() :value(Var::V_FALSE) {}
    Boolean(const Boolean& other) :value(other.value){}
    Boolean(Boolean&& other)noexcept :value(other.value){}
    Boolean& operator=(const Boolean& other)noexcept{value=other.value;_RTN_SELF();}
    Boolean& operator=(Boolean&& other)noexcept{value=other.value;_RTN_SELF();}
    template<typename T>
    Boolean(T _b)noexcept:value(Var::V_FALSE){}
    Boolean(bool _b)noexcept:value(_b?Var::V_TRUE:Var::V_FALSE){}
    Boolean(Number _n)noexcept:value(_n?Var::V_TRUE:Var::V_FALSE){}
    template<typename T>
    Boolean& operator=(T other)noexcept{value=Var::V_FALSE;_RTN_SELF();}
    Boolean& operator=(bool other)noexcept{value=other?Var::V_TRUE:Var::V_FALSE;_RTN_SELF();}
    Boolean& operator=(Number other)noexcept{value=other?Var::V_TRUE:Var::V_FALSE;_RTN_SELF();}

    bool operator==(const Boolean& other)const noexcept{return value == other.value;}
    bool operator!=(const Boolean& other)const noexcept{return value != other.value;}

    operator bool()const noexcept{return value==Var::V_TRUE;}
    operator int()const noexcept{return value==Var::V_TRUE?1:0;}
};

// any type in JavaScript
// If this is an object/array, when call operator[], a vitual temp child will be allocated.
// This->__temp_child has the information of its father(this)'s key (when call operator[]).
// If this->__temp_child has called the operator=, its father(this) will add it to self.
// In order to mantain just one __temp_child, when father called the operator[], the previous
// allocated child will be deleted
class Value {
private:
    Value* __temp_father = nullptr; // father node (temp)
    String __temp_in_key_s = "";    // key of father find this
    Value* __temp_child = nullptr;  // when this call operatorp[], alloc a child, when child call operator= father add child
    Type __type;                    // data type
    void* __pdata;                  // data

    void __DeleteData() {
        switch (this->__type)
        {
        case Type::T_NULL: {
            delete (Var*)this->__pdata;
            break;
        }
        case Type::T_NUMBER: {
            delete (Number*)this->__pdata;
            break;
        }
        case Type::T_STRING: {
            delete (String*)this->__pdata;
            break;
        }
        case Type::T_ARRAY: {
            delete (Array*)this->__pdata;
            break;
        }
        case Type::T_OBJECT: {
            delete (Object*)this->__pdata;
            break;
        }
        case Type::T_BOOLEAN:{
            delete (Boolean*)this->__pdata;
        }
        default:
            break;
        }
    }
    void __CopyData(void* other_data) {
        switch (this->__type)
        {
        case Type::T_NULL: {
            this->__pdata = new Null();
            break;
        }
        case Type::T_NUMBER: {
            this->__pdata = new Number(*(Number*)(other_data));
            break;
        }
        case Type::T_STRING: {
            this->__pdata = new String(*(String*)(other_data));
            break;
        }
        case Type::T_ARRAY: {
            this->__pdata = new Array(*(Array*)(other_data));
            break;
        }
        case Type::T_OBJECT: {
            this->__pdata = new Object(*(Object*)(other_data));
            break;
        }
        case Type::T_BOOLEAN:{
            this->__pdata = new Boolean(*(Boolean*)(other_data));
        }
        default:
            break;
        }
    }
    void __NewTempCHild() {
        __temp_child = new Value();
        __temp_child->__temp_father = this;
        __temp_in_key_s = "";
    }
    void __FreeTempChild() {
        if (__temp_child) {
            delete __temp_child;
            __temp_child = nullptr;
        }
    }
    void __CheckAndAddToFather() {  // check if this has a father and if father is an object, append this to the father
        if (this->__temp_father) {
            if (__temp_father->__type == Type::T_OBJECT && __temp_in_key_s.size()) {
                Value __temp(*this);
                __temp.__temp_father = nullptr;
                __temp.__temp_in_key_s = "";
                __temp_father->Cast<Object>().emplace(__temp_in_key_s,std::move(__temp));
            }
        }
    }
    static String __ToString(double d){
        std::stringstream ss;
        ss <<d;
        return ss.str();
    }
    bool __TypeEqual(const Value& other) const noexcept{
        return this->__type == other.__type;
    }
public:
    Value()
        :__type(Type::T_NULL)
        , __pdata(new Null())
    {

        // std::cout << (uint64_t)(this) << " normal called constructor\n";
    }
    Value(const Value& other)
        :__type(other.__type)
    {
        __CopyData(other.__pdata);
        // std::cout << (uint64_t)(this)<< " copy called constructor\n";
    }
    Value(Value&& other) noexcept
        :__type(other.__type)
        , __pdata(other.__pdata)
    {
        other.__pdata = new Null();
        other.__type = Type::T_NULL;
        // std::cout << (uint64_t)(this)<< " move called constructor\n";
    };
    ~Value() {
        this->__DeleteData();
        this->__FreeTempChild();
        // std::cout << (uint64_t)(this)<< " freed\n";
    }
    template<typename T> Value(T data) {
        this->__type = _ToType_V<typename _ToJson<std::decay_t<T>>::Type>;

        

        if constexpr(std::is_rvalue_reference_v<T>){
            this->__pdata = new typename _ToJson<std::decay_t<T>>::Type(std::forward<T>(data));
        }else{
            this->__pdata = new typename _ToJson<std::decay_t<T>>::Type(data);
        }
        
        
    }
    Value& operator=(const Value& other) {
        this->__DeleteData();
        this->__type = other.__type;
        __CopyData(other.__pdata);
        this->__CheckAndAddToFather();
        // std::cout << (uint64_t)(this)<< " copy called assigned\n";
        _RTN_SELF();
    }
    Value& operator=(Value&& other) noexcept {
        this->__DeleteData();
        this->__type = other.__type;
        this->__pdata = other.__pdata;
        other.__pdata = new Null();
        other.__type = Type::T_NULL;
        this->__CheckAndAddToFather();
        // std::cout << (uint64_t)(this)<< " move called assigned\n";
        _RTN_SELF();
    }
    template<typename T> Value& operator=(T data) { // this func is opeional, if this func is not exist, will call the "Value& operator=(const Value& other)" and construct a new Value
        this->__DeleteData();
        if constexpr (std::is_same<typename _ToJson<std::decay_t<T>>::Type,String>::value){
            String __in = data;
            String __out = "";
            for(char i : __in){ // add slash
                if(i == '"'){
                    __out+='\\';
                    __out+=i;
                }else{
                    __out+=i;
                }
            }
            this->__type = _ToType_V<typename _ToJson<std::decay_t<T>>::Type>;
            this->__pdata = new typename _ToJson<std::decay_t<T>>::Type(__out);
        }else{
            this->__type = _ToType_V<typename _ToJson<std::decay_t<T>>::Type>;
            this->__pdata = new typename _ToJson<std::decay_t<T>>::Type(data);
        }

        // std::cout << (uint64_t)(this) << " any called assigned\n";
        this->__CheckAndAddToFather();
        _RTN_SELF();
    }
    bool operator==(const Value& other) const noexcept{
        __return_if(!__TypeEqual(other),false);
        __switch(__type)
        __return_match(Type::T_NULL,true)
        __return_match(Type::T_NUMBER,this->_C(Number) == other._C(Number))
        __return_match(Type::T_ARRAY,this->_C(Array) == other._C(Array))
        __return_match(Type::T_STRING,this->_C(String) == other._C(String))
        __return_match(Type::T_OBJECT,this->_C(Object) == other._C(Object))
        __return_match(Type::T_BOOLEAN,this->_C(Boolean)== other._C(Boolean))
        __return_default(false)
        __switch_end()
    }
    bool operator!=(const Value& other) const{
        return !this->operator==(other);
    }
    Value& operator[](const Value& key) { // return the Child value or temp child value
        __FreeTempChild();
        __NewTempCHild();
        if (key.__type == Type::T_STRING) {
            String key_s = key.Cast<String>();
            __switch(__type)
            case Type::T_ARRAY: {
                size_t pos = String::npos;
                try { pos = std::stoll(key_s); }
                catch (const std::exception&) {}
                if (pos <= ((Array*)(this->__pdata))->size() - 1) { // has the key
                    return (*((Array*)(this->__pdata)))[pos];
                }
                break;
            }
            case Type::T_OBJECT: {
                if (((Object*)(this->__pdata))->count(key_s)) {
                    return ((Object*)(this->__pdata))->at(key_s);
                }
                else {
                    __temp_child->__temp_in_key_s = key_s;
                }
                break;
            }
            case Type::T_NULL:
            case Type::T_NUMBER:
            case Type::T_STRING:
            case Type::T_BOOLEAN:
            default:break;
            __switch_end()
        }
        else if (key.__type == Type::T_NUMBER) {
            size_t pos = (size_t)key.Cast<Number>();
            switch (this->__type)
            {
            case Type::T_ARRAY: {
                size_t len = this->Length();
                if (!len) break;
                if (pos <= len - 1) { // has the key
                    return (*((Array*)(this->__pdata)))[pos];
                }
                break;
            }
            case Type::T_OBJECT:
            case Type::T_NULL:
            case Type::T_NUMBER:
            case Type::T_STRING:
            default:break;
            }
        }
        return *__temp_child;
    }
    Value& At(const Value& key) {
        return this->operator[](key);
    }
    template<typename T> bool Castable() noexcept{
        if (_ToType_V<typename _ToJson<std::decay_t<T>>::Type> == this->__type) {
            return true;
        }
        else return false;
    }
    template<typename T> typename _ToJson<std::decay_t<T>>::Type& Cast() noexcept{
        return *(typename _ToJson<std::decay_t<T>>::Type*)(this->__pdata);
    }
    template<typename T> const typename _ToJson<std::decay_t<T>>::Type& Cast() const {
        return *(typename _ToJson<std::decay_t<T>>::Type*)(this->__pdata);
    }
    const Type GetType()const {
        return this->__type;
    }
    const size_t Length()const {
        switch (this->__type)
        {
        case Type::T_ARRAY:return ((Array*)(this->__pdata))->size();
        case Type::T_OBJECT:return ((Object*)(this->__pdata))->size();
        case Type::T_NULL:
        case Type::T_NUMBER:
        case Type::T_STRING:
        case Type::T_BOOLEAN:
        default:break;
        }
        return 0;
    }
    Array Keys() {
        Array res;
        switch (this->__type)
        {
        case Type::T_ARRAY: {
            size_t size = this->Length();
            for (size_t i = 0; i < size; i++) {
                res.push_back(i);
            }
            break;
        }
        case Type::T_OBJECT: {
            Object& self_data = this->Cast<Object>();
            for (const auto& i : self_data) {
                res.push_back(i.first);
            }
            break;
        }
        case Type::T_NULL:
        case Type::T_NUMBER:
        case Type::T_STRING:
        case Type::T_BOOLEAN:
        default:break;
        }
        return res;
    }
    const String Stringfy(bool format = true, size_t indent = 4, size_t level = 0) const {
        String res = "";
        if (format) {
            String indent_s = "";
            String single_indent_s = "";
            for (size_t i = 0; i < level * indent; i++) {
                indent_s += ' ';
            }
            for (size_t i = 0; i < indent; i++) {
                single_indent_s += ' ';
            }
            level++;
            switch (this->__type)
            {
            case Type::T_ARRAY: {
                const size_t size = this->Length();
                if (size) {
                    res = "[\n";
                    const Array& self_data = this->Cast<Array>();
                    size_t pos = 0;
                    const size_t last = size - 1;
                    for (const auto& i : self_data) {
                        res += indent_s + single_indent_s;
                        if (i.GetType() == Type::T_STRING) {
                            res += '"' + i.Stringfy(format, indent, level) + '"';
                        }
                        else {
                            res += i.Stringfy(format, indent, level);
                        }

                        if (pos != last) {
                            res += ",\n";
                        }
                        else {
                            res += "\n";
                        }
                        pos++;
                    }
                    res += indent_s + "]";
                }
                else {
                    res = "[]";
                }

                break;
            }
            case Type::T_OBJECT: {
                const size_t size = this->Length();
                if (size) {
                    res = "{\n";
                    const Object& self_data = this->Cast<Object>();

                    size_t pos = 0;
                    const size_t last = size - 1;
                    for (const auto& i : self_data) {
                        res += indent_s + single_indent_s;
                        res += '"' + i.first + "\" : ";
                        if (i.second.GetType() == Type::T_STRING) {
                            res += '"' + i.second.Stringfy(format, indent, level) + '"';
                        }
                        else {
                            res += i.second.Stringfy(format, indent, level);
                        }
                        if (pos != last) {
                            res += ",\n";
                        }
                        else {
                            res += "\n";
                        }
                        pos++;
                    }
                    res += indent_s + "}";
                }
                else {
                    res = "{}";
                }
                break;
            }
            case Type::T_NULL: {
                res = "0";
                break;
            }
            case Type::T_NUMBER: {
                res = __ToString(this->Cast<Number>());
                break;
            }
            case Type::T_STRING: {
                res = this->Cast<String>();
                break;
            }
            case Type::T_BOOLEAN:{
                if(this->Cast<Boolean>()){
                    res = "true";
                }else{
                    res = "false";
                }
            }
            default:break;
            }
        }
        else {
            switch (this->__type)
            {
            case Type::T_ARRAY: {
                const size_t size = this->Length();
                if (size) {
                    res = "[";
                    const Array& self_data = this->Cast<Array>();
                    size_t pos = 0;
                    const size_t last = size - 1;
                    for (const auto& i : self_data) {
                        if (i.GetType() == Type::T_STRING) {
                            res += '"' + i.Stringfy(format, indent, level) + '"';
                        }
                        else {
                            res += i.Stringfy(format, indent, level);
                        }

                        if (pos != last) {
                            res += ",";
                        }
                        pos++;
                    }
                    res += "]";
                }
                else {
                    res = "[]";
                }

                break;
            }
            case Type::T_OBJECT: {
                const size_t size = this->Length();
                if (size) {
                    res = "{";
                    const Object& self_data = this->Cast<Object>();

                    size_t pos = 0;
                    const size_t last = size - 1;
                    for (const auto& i : self_data) {
                        res += '"' + i.first + "\":";
                        if (i.second.GetType() == Type::T_STRING) {
                            res += '"' + i.second.Stringfy(format, indent, level) + '"';
                        }
                        else {
                            res += i.second.Stringfy(format, indent, level);
                        }
                        if (pos != last) {
                            res += ",";
                        }
                        pos++;
                    }
                    res += +"}";
                }
                else {
                    res = "{}";
                }
                break;
            }
            case Type::T_NULL: {
                res = "0";
                break;
            }
            case Type::T_NUMBER: {
                res = __ToString(this->Cast<Number>());
                break;
            }
            case Type::T_STRING: {
                res = this->Cast<String>();
                break;
            }
            case Type::T_BOOLEAN:{
                if(this->Cast<Boolean>()){
                    res = "true";
                }else{
                    res = "false";
                }
            }
            default:break;
            }
        }
        return res;
    }
    bool PushBack(const Value& val) {
        if (this->__type == Type::T_ARRAY) {
            this->Cast<Array>().push_back(val);
            return true;
        }
        return false;
    }
    bool EmplaceBack(Value&& val) {
        if (this->__type == Type::T_ARRAY) {
            this->Cast<Array>().emplace_back(std::forward<Value>(val));
            return true;
        }
        return false;
    }
    bool Emplace(const String& key, Value&& val) {
        if (this->__type == Type::T_OBJECT) {
            this->Cast<Object>().emplace(key, std::forward<Value>(val));
            return true;
        }
        return false;
    }
    bool Erase(const Value& key) {
        size_t length = this->Length();
        if (!length)return false;
        if (this->__type == Type::T_ARRAY && length) {
            Array& self_data = this->Cast<Array>();
            if (key.__type == Type::T_STRING) {
                size_t pos = String::npos;
                try { pos = std::stoll(key.Cast<String>()); }
                catch (std::exception&) {
                    return false;
                }
                if (pos < length) {
                    self_data.erase(self_data.begin() + pos);
                    return true;
                }
            }
            else if (key.__type == Type::T_NUMBER) {
                size_t pos = (size_t)key.Cast<Number>();
                if (pos < length) {
                    self_data.erase(self_data.begin() + pos);
                    return true;
                }
            }
            return true;
        }
        else if (this->__type == Type::T_OBJECT && key.__type == Type::T_STRING) {
            auto& self_data = this->Cast<Object>();
            String key_s = key.Cast<String>();
            if (self_data.count(key_s)) {
                self_data.erase(key_s);
                return true;
            }
        }
        return false;
    }
    Value& Back() {
        if (this->Length()) {
            return this->operator[](this->Length() - 1);
        }
        else return this->operator[](String::npos);
    }
    Value& Front() {
        return this->operator[](0);
    }
    // Can be used by Object
    bool HasOwnProperty(const String& key) const {
        if (this->__type == Type::T_OBJECT) {
            return this->_C(Object).count(key);
        }
        return false;
    }
    // Can be used by String & Number
    Value operator+(const Value& other) const{
        if(this->__type == Type::T_NUMBER && other.__type == Type::T_NUMBER){
            return this->_C(Number)+other._C(Number);
        }
        else if(this->__type == Type::T_STRING&& other.__type == Type::T_STRING){
            return this->_C(String)+other._C(String);
        }
        else{
            throw std::invalid_argument("cannot add non-number type");
            return Null();
        }
    }
    // Can be used by Number
    Number operator-(const Value& other) const{
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->_C(Number)-other._C(Number);
        }else{
            throw std::invalid_argument("cannot add non-number type");
            return 0;
        }
    }
    // Can be used by String & Number
    Value& operator+=(const Value& other){
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            this->_C(Number)+= other._C(Number);
        }
        else if(this->__type == Type::T_STRING&& other.__type == Type::T_STRING){
            this->_C(String)+= other._C(String);
        }
        else{
            throw std::invalid_argument("cannot add non-number type");
        }
        _RTN_SELF();
    }
    // Can be used by Number
    Value& operator-=(const Value& other){
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            this->Cast<Number>()-= other.Cast<Number>();
        }else{
            throw std::invalid_argument("cannot add non-number type");
        }
        _RTN_SELF();
    }
    // Can be used by Number
    Boolean operator>(const Value& other)const{
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->Cast<Number>()>other.Cast<Number>();
        }else{
            throw std::invalid_argument("cannot add non-number type");
            return false;
        }
    }
    // Can be used by Number
    Boolean operator<(const Value& other)const{
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->Cast<Number>()<other.Cast<Number>();
        }else{
            throw std::invalid_argument("cannot add non-number type");
            return false;
        }
    }
    // Can be used by Number
    Boolean operator>=(const Value& other)const{
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->Cast<Number>()>=other.Cast<Number>();
        }else{
            throw std::invalid_argument("cannot add non-number type");
            return false;
        }
    }
    // Can be used by Number
    Boolean operator<=(const Value& other)const{
        if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->Cast<Number>()<=other.Cast<Number>();
        }else{
            throw std::invalid_argument("cannot add non-number type");
            return false;
        }
    }
    // Can be used by Boolean & Number
    Boolean operator&&(const Value& other)const{
        if(this->__type == Type::T_BOOLEAN&& other.__type == Type::T_BOOLEAN){
            return this->Cast<Boolean>()&&other.Cast<Boolean>();
        }
        else if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->Cast<Number>()&&other.Cast<Number>();
        }
        else{
            throw std::invalid_argument("type error");
            return false;
        }
    }
    // Can be used by Boolean & Number
    Boolean operator||(const Value& other)const{
        if(this->__type == Type::T_BOOLEAN&& other.__type == Type::T_BOOLEAN){
            return this->Cast<Boolean>()||other.Cast<Boolean>();
        }
        else if(this->__type == Type::T_NUMBER&& other.__type == Type::T_NUMBER){
            return this->Cast<Number>()&&other.Cast<Number>();
        }
        else{
            throw std::invalid_argument("type error");
            return false;
        }
    }
    // Can be used by Boolean
    Boolean operator!(){
        if(this->__type == Type::T_BOOLEAN){
            return !(this->Cast<Boolean>());
        }else{
            throw std::invalid_argument("type error");
            return false;
        }
    }
    // Can be used by String with any other type
    Value& operator<<(const Value& other){
        if(this->__type == Type::T_STRING){
            *this+=other.Stringfy();
            _RTN_SELF();
        }else{
            throw std::invalid_argument("type error");
            _RTN_SELF();
        }
    }
    Boolean IsNull()const noexcept{
        return this->__type == Type::T_NULL;
    }
    Boolean IsNumber()const noexcept{
        return this->__type == Type::T_NUMBER;
    }
    Boolean IsString()const noexcept{
        return this->__type == Type::T_STRING;
    }
    Boolean IsBoolean()const noexcept{
        return this->__type == Type::T_BOOLEAN;
    }
    Boolean IsArray()const noexcept{
        return this->__type == Type::T_ARRAY;
    }
    Boolean IsObject()const noexcept{
        return this->__type == Type::T_OBJECT;
    }

};


// parser
class __JSON_String_Paeser {
private:
    inline static const  char __space_char_set[4] = {' ','\t','\r','\n'};
    inline static bool __Is_Space(char __c) noexcept {
        for(int i=0;i<4;i++){
            __return_if(__c==__space_char_set[i],true)
        }
        return false;
    }
    size_t __Find_First_Not_Space(const String& str,size_t start = 0,size_t end = String::npos) {
        size_t size = str.size();
        if (!size)return String::npos;
        end = std::min(end, size-1);
        for (size_t i = start; i <= end; i++) {
            if (!__Is_Space(str[i])) {
                return i;
            }
        }
        return String::npos;
    }
    size_t __Find_Last_Not_Space(const String& str, size_t start = 0, size_t end = String::npos) {
        size_t size = str.size();
        if (!size)return String::npos;
        end = std::min(size - 1, end);
        for (size_t i = end; i >= start; i--) {
            if (!__Is_Space(str[i])) {
                return i;
            }
        }
        return String::npos;
    }
    // contains start and stop
    String __StrBetween(const String& src, size_t start, size_t stop) {
        if (start >= stop) {
            return "";
        }
        return src.substr(start, stop - start + 1);
    }
    // the first must matched
    // return the matched end position
    size_t __Match(const String& str, char tar_start,char tar_stop ,size_t cur_pos, size_t end_pos){
        if (cur_pos >= end_pos - 1) { // not last
            return String::npos;
        }
        if (str[cur_pos] != tar_start)return String::npos;

        if (tar_start == '"') {
            for (size_t i = cur_pos + 1; i < end_pos; i++) {
                if (str[i] == tar_stop) {
                    if (i == cur_pos + 1)return cur_pos + 1;
                    if (str[i - 1] != '\\') {
                        return i;
                    }
                }
            }
            return String::npos;
        }
        else if (tar_start == '[' || tar_start == '{') {
            __LinkStack<size_t> record;
            record.push(cur_pos);
            for (size_t i = cur_pos + 1; i < end_pos; i++) {
                if (str[i] == tar_start) {
                    record.push(i);
                }
                else if (str[i] == tar_stop) {
                    if (!record.size()) {
                        return String::npos;
                    }
                    record.pop();
                    if (!record.size()) {
                        return i;
                    }
                }
            }
            return String::npos;
        }
        else return String::npos;


    }
    // the first must be a quate
    size_t __MatchQuate(const String& str, size_t cur_pos,size_t end_pos) {
        return __Match(str, '"', '"', cur_pos, end_pos);
    }
    size_t __MatchCurlyBrace(const String& str, size_t cur_pos, size_t end_pos) {
        return __Match(str, '{', '}', cur_pos, end_pos);
    }
    size_t __MatchSquareBrace(const String& str, size_t cur_pos, size_t end_pos) {
        return __Match(str, '[', ']', cur_pos, end_pos);
    }
    bool __IsNumber(char __c) noexcept{
        if (__c >= '0' && __c <= '9')return true;
        else if (__c == '.')return true;
        else return false;
    }
    void __RemoveSpace(String& str) {
        size_t size = str.size();
        if (!size)return;
        auto start = __Find_First_Not_Space(str);
        auto end = __Find_Last_Not_Space(str);
        if (start== String::npos|| end == String::npos)return;
        if (start > end) {
            str = "";
            return;
        }
        str = str.substr(start, end - start + 1);
        return;
    }
    double __ToDouble(const char __c) const noexcept{
        switch (__c)
        {
        case '0':return 0;
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
        default:return __c;
        }
    }

    Value __ParseObject(const String& str, size_t& total_size,size_t& cur_pos,size_t& end_pos){
        // obj start
        Value res = Object();
        read_one_member:    // lable of loop
        {
            // std::cout << res.Stringfy() << "\n";
            cur_pos = __Find_First_Not_Space(str, cur_pos+1, end_pos);
            if (cur_pos == end_pos) {
                return res;
            }
            // read key
            if (str[cur_pos] != '"')return Null();
            size_t out_quate = __MatchQuate(str, cur_pos, end_pos); // this is the position of '}'
            if (out_quate == String::npos)return Null();
            String cur_key = __StrBetween(str, cur_pos+1, out_quate-1);
            if (cur_pos >= end_pos)return Null();
            cur_pos = __Find_First_Not_Space(str, out_quate + 1, end_pos);
            if (str[cur_pos] != ':') return Null();
            // read value
            cur_pos = __Find_First_Not_Space(str, cur_pos + 1, end_pos);
            if(cur_pos>=end_pos)return Null();
            char val_first_key = str[cur_pos];
            if (val_first_key == '"') {  // val is str
                out_quate = __MatchQuate(str, cur_pos, end_pos);
                if (out_quate < end_pos) {
                    res[cur_key] = __StrBetween(str, cur_pos+1, out_quate-1);
                }
                cur_pos = out_quate+1;
            }
            else if(val_first_key == '[') { // val is array
                out_quate = __MatchSquareBrace(str, cur_pos, end_pos);
                if (out_quate < end_pos) {
                    res[cur_key] = Parse(__StrBetween(str, cur_pos, out_quate));
                }
                cur_pos = out_quate + 1;
            }
            else if (val_first_key == '{') { // val is object
                out_quate = __MatchCurlyBrace(str, cur_pos, end_pos);
                if (out_quate < end_pos) {
                    res[cur_key] = Parse(__StrBetween(str, cur_pos, out_quate));
                }
                cur_pos = out_quate + 1;
            }
            else if (val_first_key == 'f') { // val is false
                if(((cur_pos+4)<end_pos) &&(__StrBetween(str, cur_pos, cur_pos +4) == "false")){
                    res[cur_key] = false;
                }
                cur_pos = cur_pos + 5;
            }
            else if (val_first_key == 't') { // val is false
                if(((cur_pos+3)<=(end_pos-1)) &&(__StrBetween(str, cur_pos, cur_pos +3) == "true")){
                    res[cur_key] = true;
                }
                cur_pos = cur_pos + 4;
            }
            else { // val can be a number
                size_t num_start = cur_pos;
                while (__IsNumber(str[cur_pos]) && cur_pos < end_pos) {
                    cur_pos++;
                }
                if (cur_pos == num_start + 1) {
                    res[cur_key] = __ToDouble(str[num_start]);
                }else{
                    try {
                        res[cur_key] = std::stod(__StrBetween(str, num_start, cur_pos - 1));
                    }
                    catch (const std::exception&) {
                        return Null();
                    }
                }
            }
            if (cur_pos == end_pos)return res;
            cur_pos = __Find_First_Not_Space(str, cur_pos, end_pos);
            if (cur_pos == end_pos)return res;
            if (str[cur_pos] == ',') {
                goto read_one_member;
            }
            else return Null();
        }

    }
    Value __ParseArray(const String& str, size_t& total_size,size_t& cur_pos,size_t& end_pos){
        // arr start
        Value res = Array();
        read_one_value: // lable of loop
        cur_pos = __Find_First_Not_Space(str, cur_pos + 1, end_pos);
        if (cur_pos == end_pos) {
            return res;
        }

        char val_first_key = str[cur_pos];
        if (val_first_key == '"') {  // val is str
            size_t out_quate = __MatchQuate(str, cur_pos, end_pos);
            if (out_quate < end_pos) {
                res.PushBack(__StrBetween(str, cur_pos + 1, out_quate - 1));
            }
            cur_pos = out_quate + 1;
        }
        else if (val_first_key == '[') { // val is array
            size_t out_quate = __MatchSquareBrace(str, cur_pos, end_pos);
            if (out_quate < end_pos) {
                res.PushBack(Parse(__StrBetween(str, cur_pos, out_quate)));
            }
            cur_pos = out_quate + 1;
        }
        else if (val_first_key == '{') { // val is object
            size_t out_quate = __MatchCurlyBrace(str, cur_pos, end_pos);
            if (out_quate < end_pos) {
                res.PushBack(Parse(__StrBetween(str, cur_pos, out_quate)));
            }
            cur_pos = out_quate + 1;
        }
        else if (val_first_key == 'f') { // val is false
            if(((cur_pos+4)<end_pos) &&(__StrBetween(str, cur_pos, cur_pos +4) == "false")){
                res.PushBack(false);
            }
            cur_pos = cur_pos + 5;
        }
        else if (val_first_key == 't') { // val is true
            if(((cur_pos+3)<=(end_pos-1)) &&(__StrBetween(str, cur_pos, cur_pos +3) == "true")){
                res.PushBack(true);
            }
            cur_pos = cur_pos + 4;
        }
        else { // val can be a number
            size_t num_start = cur_pos;
            while (__IsNumber(str[cur_pos]) && cur_pos < end_pos) {
                cur_pos++;
            }
            if (cur_pos == num_start + 1) {
                res.PushBack(__ToDouble(str[num_start]));
            }
            else {
                std::string __current_value = __StrBetween(str, num_start, cur_pos - 1);
                try {
                    res.PushBack(std::stod(__current_value));
                }
                catch (const std::exception&) {
                    res.PushBack(Null());
                }
            }
        }
        if (cur_pos == end_pos)return res;
        cur_pos = __Find_First_Not_Space(str, cur_pos, end_pos);
        if (cur_pos == end_pos)return res;
        if (str[cur_pos] == ',') {
            goto read_one_value;
        }
        else return Null();
    }
public:
    __JSON_String_Paeser() {}
    ~__JSON_String_Paeser() {}
    __JSON_String_Paeser(const __JSON_String_Paeser&) = delete;
    __JSON_String_Paeser(__JSON_String_Paeser&&) = delete;
    __JSON_String_Paeser& operator=(const __JSON_String_Paeser&) = delete;
    __JSON_String_Paeser& operator=(__JSON_String_Paeser&&) = delete;

    Value Parse(const String& str) {
        // set local-global value
        size_t total_size = str.size();
        if (total_size <2) {    // check least length
            return Null();
        }
        size_t cur_pos = __Find_First_Not_Space(str);   // find first valid pos
        size_t end_pos = __Find_Last_Not_Space(str);    // find last valid pos
        if (cur_pos == String::npos || end_pos == String::npos || cur_pos >= end_pos) { // check pos
            return Null();
        }
        if (str[cur_pos] == '{' && str[end_pos] == '}')
            return __ParseObject(str,total_size,cur_pos,end_pos);   // this is an object
        else if (str[cur_pos] == '[' && str[end_pos] == ']')   // this is an array
            return __ParseArray(str,total_size,cur_pos,end_pos);
        else return Null();
    }

};

// Function Interfaces----------------------------------------------------------------------------------------
template<typename T, bool __parse_str = false> Value Parse(T val) {
    if constexpr (std::is_same_v<typename _ToJson<std::decay_t<T>>::Type, String>) {
        return Parse<const String&, __parse_str>(val);
    }
    else {
        return val;
    }
}
template <> Value Parse<const String&,false>(const String& str) {
    return str;
}
template <> Value Parse<const String&, true>(const String& str) {
    return __JSON_String_Paeser().Parse(str);
    return str;
}
Value ParseStr(const String& str) {
    return Parse<const String&, true>(str);
}
template<typename _Struct_T> bool Structify(_Struct_T& __out_pStruct__, Value& __in_v__) {
    if constexpr (std::is_same_v<typename _ToJson<std::decay_t<_Struct_T>>::Type, String>) {
        if constexpr (std::is_same_v<std::decay_t<_Struct_T>, String>) {
            __out_pStruct__ = __in_v__.Cast<String>();
        }
        else if constexpr (std::is_same_v<std::decay_t<_Struct_T>, const char*>) {
            __out_pStruct__ = __in_v__.Cast<String>().c_str();
        }
        else if constexpr (std::is_same_v<std::decay_t<_Struct_T>, char*>) {
            ::memcpy(__out_pStruct__, __in_v__.Cast<String>().c_str(), __in_v__.Cast<String>().size());
        }
        else if constexpr (std::is_same_v<std::decay_t<_Struct_T>, const unsigned char*>) {
            __out_pStruct__ = __in_v__.Cast<String>().c_str();
        }
        else if constexpr (std::is_same_v<std::decay_t<_Struct_T>, unsigned char*>) {
            ::memcpy(__out_pStruct__, __in_v__.Cast<String>().c_str(), __in_v__.Cast<String>().size());
        }
        else {
            return false;
        }
        return true;
    }
    else {
        if (__in_v__.GetType() == _ToType_V<typename _ToJson<std::decay_t<_Struct_T>>::Type>) {
            if constexpr (std::is_copy_assignable_v<_Struct_T>) {
                __out_pStruct__ = (_Struct_T)(__in_v__.Cast<typename _ToJson<std::decay_t<_Struct_T>>::Type>());
            }
            else if constexpr (std::is_move_assignable_v<_Struct_T>) {
                __out_pStruct__ = std::move((_Struct_T)(__in_v__.Cast<typename _ToJson<std::decay_t<_Struct_T>>::Type>()));
            }
        }
        else return false;
        return true;
    }
}

// undef previous defined macros
#undef __switch
#undef __match
#undef __return_match
#undef __match_
#undef __return_default
#undef __switch_end
#undef __return_if
#undef _C
}} // namespace tcx

// Macros
#define __T_C_X_JSON_STR(str)       tcx::json::Parse<const tcx::json::String&, true>(str)
#define __T_C_X_INLINE_JSON(...)    tcx::json::Parse<const tcx::json::String&, true>(#__VA_ARGS__)

#define __TCX_ENUM2JSON_DEF_BEGIN(_Enum_Type)\
    template<> tcx::json::Value tcx::json::Parse<_Enum_Type>(_Enum_Type __enum__){switch (__enum__){

#define __TCX_ENUM2JSON_DEF_MEMBER(_Key,_Value)\
    case decltype(__enum__)::_Key: return _Value;

#define __TCX_ENUM2JSON_DEF_END() \
    default:return Null();}}

#define __TCX_JSON2ENUM_DEF_BEGIN(_Enum_Type)\
    template<> bool tcx::json::Structify<_Enum_Type>(_Enum_Type& __out_pEnum__, Value& __in_v__){

#define __TCX_JSON2ENUM_DEF_MEMBER(_Key,_Value)\
    if(__in_v__ == _Value){__out_pEnum__ = std::decay_t<decltype(__out_pEnum__)>::_Key;return true;}

#define __TCX_JSON2ENUM_DEF_END()  return false;}


#define __TCX_STRUCT2JSON_DEF_BEGIN(_Struct_Type) \
    template<> tcx::json::Value tcx::json::Parse<_Struct_Type>(_Struct_Type ___obj___){\
        tcx::json::Value ___res___ = tcx::json::Object();

#define __TCX_STRUCT2JSON_DEF_END() return ___res___;}

#define __TCX_STRUCT2JSON_DEF_NAMED_MEMBER(_StrOutKeyName,_Member)\
    ___res___[_StrOutKeyName] = tcx::json::Parse<std::decay_t<decltype(___obj___._Member)>>(___obj___._Member);

#define __TCX_JSON2STRUCT_DEF_BEGIN(_Struct_Type)\
    template<> bool tcx::json::Structify<_Struct_Type>(_Struct_Type& __out_pStruct, Value& __v__) {\
    bool ___res___ = true;
#define __TCX_JSON2STRUCT_DEF_END()  return ___res___;}
    
#define __TCX_JSON2STRUCT_DEF_NAMED_MEMBER(_StrFromKeyName,_Member)\
    ___res___ = ___res___&&tcx::json::Structify(__out_pStruct._Member, __v__[_StrFromKeyName]);


// Macro Interfaces----------------------------------------------------------------------------------------

// Define the Enum for Enum to JSON
#define T_E2J_BEGIN(_Enum_Tp)             __TCX_ENUM2JSON_DEF_BEGIN(_Enum_Tp)
#define T_E2J_END()                       __TCX_ENUM2JSON_DEF_END()
#define T_E2J_DEF(_Key,_Value)            __TCX_ENUM2JSON_DEF_MEMBER(_Key,_Value)

// Define the Enum for JSON to Enum
#define T_J2E_BEGIN(_Enum_Tp)             __TCX_JSON2ENUM_DEF_BEGIN(_Enum_Tp)
#define T_J2E_END()                       __TCX_JSON2ENUM_DEF_END()
#define T_J2E_DEF(_Key,_Value)            __TCX_JSON2ENUM_DEF_MEMBER(_Key,_Value)

// Define the Struct for Struct to JSON
#define T_S2J_BEGIN(_Struct_Tp)           __TCX_STRUCT2JSON_DEF_BEGIN(_Struct_Tp)
#define T_S2J_END()                       __TCX_STRUCT2JSON_DEF_END()
#define T_S2J_DEF(_Member)                __TCX_STRUCT2JSON_DEF_NAMED_MEMBER(#_Member,_Member)
#define T_S2J_DEF_NAMED(_Name,_Member)    __TCX_STRUCT2JSON_DEF_NAMED_MEMBER(_StrOutKeyName,_Member)

// Define the Struct for JSON to Struct
#define T_J2S_BEGIN(_Struct_Tp)           __TCX_JSON2STRUCT_DEF_BEGIN(_Struct_Tp)
#define T_J2S_END()                       __TCX_JSON2STRUCT_DEF_END()
#define T_J2S_DEF(_Member)                __TCX_JSON2STRUCT_DEF_NAMED_MEMBER(#_Member,_Member)
#define T_J2S_DEF_NAMED(_Name,_Member)    __TCX_JSON2STRUCT_DEF_NAMED_MEMBER(_StrOutKeyName,_Member)

// Write Inline style JSON into this Macro
// returns tcx::json::Value
#define T_INLINE_JSON(...)                __T_C_X_INLINE_JSON(__VA_ARGS__)
// Write JSON in str
// returns tcx::json::Value
#define T_JSON_STRING(str)                __T_C_X_JSON_STR(str)

// paser process
/*                                                              
*                              <------------ , ----------------
*                              |                               |        
*                    ---- [ -> [WS_4] ---> [VAL_2] - [WS_5] -------> |(end [,])
*                   |          |                (end [])       |     |
*                   |          ------------------------------->      |
*                   |                      (end {})                  |
*   [OBJ_START] --------> { -> [WS_1] --------------------- } ---> [OBJ_FINISH] --------> [END]
*                                 |                         |
*                                 V                         |(end {"":,})
*         --> , --------------------------> [STR]----       |
*        |                                           |      |
*        |      <------------------------------- <---       |
*        |     |                                            |
*        |      ------> [WS_2] ---> : ---> [VAL_1] - [WS_3] -
*        |                                                  |
*        ---------------------------------------------------
*/    




#endif
// #endif