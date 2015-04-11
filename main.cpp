//
//  main.cpp
//  std_function
//
//  Created by Olya Telezhnaya on 05.04.15.
//  Copyright (c) 2015 1. All rights reserved.
//

// Tests for fn::function
//
// Based on tests for Boost.Function:
// <http://www.boost.org/doc/libs/1_55_0/libs/function/test/>

#include <memory>
#include <string>
#include <cassert>
#include <functional>
#include <type_traits>
#include <iostream>

#include "fn.hpp"

//#define USE_STD
#if !defined(USE_STD)
namespace n = fn;
#else
namespace n = std;
#endif

size_t res;
std::string res_str;

struct Func
{
    size_t operator() ()
    {
        res = 70;
        return res;
    }
};

struct ConstFunc
{
    size_t operator() () const
    {
        res = 71;
        return res;
    }
};

void void_f_void()
{
    res = 10;
}

int int_f_void()
{
    return 11;
}

std::string string_f_void()
{
    return "string_f_void";
}

int const const_int_f_void()
{
    return 31;
}

std::string const string_const_f_void()
{
    return "string_const_f_void";
}

void * void_ptr_f_void()
{
    return reinterpret_cast<void *>(20);
}

int * int_ptr_f_void()
{
    return reinterpret_cast<int *>(21);
}

std::string * string_ptr_f_void()
{
    return reinterpret_cast<std::string *>(22);
}

std::string const * const string_const_ptr_const_f_void()
{
    return reinterpret_cast<std::string *>(52);
}

std::string & string_ref_f_void()
{
    return res_str;
}

std::string const & string_const_ref_f_void()
{
    return res_str;
}

int global_int;

struct write_five_obj { void operator()() const { global_int = 5; } };
// TODO: changed return type from 'int' to 'void'
struct write_three_obj { void operator()() const { global_int = 3; }}; //return 7;
static void write_five() { global_int = 5; }
static void write_three() { global_int = 3; }
struct generate_five_obj { int operator()() const { return 5; } };
struct generate_three_obj { int operator()() const { return 3; } };
static int generate_five() { return 5; }
static int generate_three() { return 3; }
static std::string identity_str(const std::string& s) { return s; }
static std::string string_cat(const std::string& s1, const std::string& s2) { return s1+s2; }
static int sum_ints(int x, int y) { return x+y; }

struct write_const_1_nonconst_2
{
    void operator()() { global_int = 2; }
    void operator()() const { global_int = 1; }
};

struct add_to_obj
{
    add_to_obj(int v) : value(v) {}
    
    int operator()(int x) const { return value + x; }
    
    int value;
};

static void
test_zero_args()
{
    typedef n::function<void ()> func_void_type;
    
    write_five_obj five;
    write_three_obj three;
    
    // Default construction
    func_void_type v1;
    assert(!v1);
    
    // Assignment to an empty function
    v1 = five;
    assert(v1);
    
    // Invocation of a function
    global_int = 0;
    v1();
    assert(global_int == 5);
    
    // clear() method
    v1 = func_void_type();
    assert(!v1);
    
    // Assignment to an empty function
    v1 = three;
    assert(v1);
    
    // Invocation and self-assignment
    global_int = 0;
    v1 = v1;
    v1();
    assert(global_int == 3);
    
    // Assignment to a non-empty function
    v1 = five;
    
    // Invocation and self-assignment
    global_int = 0;
    v1 = (v1);
    v1();
    assert(global_int == 5);
    
    // clear
    v1 = nullptr;
    assert(!v1);
    
    // Assignment to an empty function from a free function
    v1 = &write_five;
    assert(v1);
    
    // Invocation
    global_int = 0;
    v1();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v1 = write_three;
    assert(v1);
    
    // Invocation
    global_int = 0;
    v1();
    assert(global_int == 3);
    
    // Assignment
    v1 = five;
    assert(v1);
    
    // Invocation
    global_int = 0;
    v1();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v1 = &write_three;
    assert(v1);
    
    // Invocation
    global_int = 0;
    v1();
    assert(global_int == 3);
    
    // Construction from another function (that is empty)
    v1 = func_void_type();
    func_void_type v2(v1);
    assert(!v2 ? true : false);
    
    // Assignment to an empty function
    v2 = three;
    assert(v2);
    
    // Invocation
    global_int = 0;
    v2();
    assert(global_int == 3);
    
    // Assignment to a non-empty function
    v2 = (five);
    
    // Invocation
    global_int = 0;
    v2();
    assert(global_int == 5);
    
    v2 = func_void_type();
    assert(!v2);
    
    // Assignment to an empty function from a free function
    v2 = (write_five);
    assert(v2 ? true : false);
    
    // Invocation
    global_int = 0;
    v2();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v2 = write_three;
    assert(v2);
    
    // Invocation
    global_int = 0;
    v2();
    assert(global_int == 3);
}
    /*
    // Swapping
    v1 = five;
    swap(v1, v2);
    v2();
    assert(global_int == 5);
    v1();
    assert(global_int == 3);
    swap(v1, v2);
    v1 = func_void_type();
    
    // Assignment
    v2 = five;
    assert(v2);
    
    // Invocation
    global_int = 0;
    v2();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v2 = &write_three;
    assert(v2);
    
    // Invocation
    global_int = 0;
    v2();
    assert(global_int == 3);
    
    // Assignment to a function from an empty function
    v2 = v1;
    assert(!v2);
    
    // Assignment to a function from a function with a functor
    v1 = three;
    v2 = v1;
    assert(v1);
    assert(v2);
    
    // Invocation
    global_int = 0;
    v1();
    assert(global_int == 3);
    global_int = 0;
    v2();
    assert(global_int == 3);
    
    // Assign to a function from a function with a function
    v2 = write_five;
    v1 = v2;
    assert(v1);
    assert(v2);
    global_int = 0;
    v1();
    assert(global_int == 5);
    global_int = 0;
    v2();
    assert(global_int == 5);
    
    // Construct a function given another function containing a function
    func_void_type v3(v1);
    
    // Invocation of a function
    global_int = 0;
    v3();
    assert(global_int == 5);
    
    // clear() method
    v3 = func_void_type();
    assert(!v3 ? true : false);
    
    // Assignment to an empty function
    v3 = three;
    assert(v3);
    
    // Invocation
    global_int = 0;
    v3();
    assert(global_int == 3);
    
    // Assignment to a non-empty function
    v3 = five;
    
    // Invocation
    global_int = 0;
    v3();
    assert(global_int == 5);
    
    // clear()
    v3 = func_void_type();
    assert(!v3);
    
    // Assignment to an empty function from a free function
    v3 = &write_five;
    assert(v3);
    
    // Invocation
    global_int = 0;
    v3();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v3 = &write_three;
    assert(v3);
    
    // Invocation
    global_int = 0;
    v3();
    assert(global_int == 3);
    
    // Assignment
    v3 = five;
    assert(v3);
    
    // Invocation
    global_int = 0;
    v3();
    assert(global_int == 5);
    
    // Construction of a function from a function containing a functor
    func_void_type v4(v3);
    
    // Invocation of a function
    global_int = 0;
    v4();
    assert(global_int == 5);
    
    // clear() method
    v4 = func_void_type();
    assert(!v4);
    
    // Assignment to an empty function
    v4 = three;
    assert(v4);
    
    // Invocation
    global_int = 0;
    v4();
    assert(global_int == 3);
    
    // Assignment to a non-empty function
    v4 = five;
    
    // Invocation
    global_int = 0;
    v4();
    assert(global_int == 5);
    
    // clear()
    v4 = func_void_type();
    assert(!v4);
    
    // Assignment to an empty function from a free function
    v4 = &write_five;
    assert(v4);
    
    // Invocation
    global_int = 0;
    v4();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v4 = &write_three;
    assert(v4);
    
    // Invocation
    global_int = 0;
    v4();
    assert(global_int == 3);
    
    // Assignment
    v4 = five;
    assert(v4);
    
    // Invocation
    global_int = 0;
    v4();
    assert(global_int == 5);
    
    // Construction of a function from a functor
    func_void_type v5(five);
    
    // Invocation of a function
    global_int = 0;
    v5();
    assert(global_int == 5);
    
    // clear() method
    v5 = func_void_type();
    assert(!v5);
    
    // Assignment to an empty function
    v5 = three;
    assert(v5);
    
    // Invocation
    global_int = 0;
    v5();
    assert(global_int == 3);
    
    // Assignment to a non-empty function
    v5 = five;
    
    // Invocation
    global_int = 0;
    v5();
    assert(global_int == 5);
    
    // clear()
    v5 = func_void_type();
    assert(!v5);
    
    // Assignment to an empty function from a free function
    v5 = &write_five;
    assert(v5);
    
    // Invocation
    global_int = 0;
    v5();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v5 = &write_three;
    assert(v5);
    
    // Invocation
    global_int = 0;
    v5();
    assert(global_int == 3);
    
    // Assignment
    v5 = five;
    assert(v5);
    
    // Invocation
    global_int = 0;
    v5();
    assert(global_int == 5);
    
    // Construction of a function from a function
    func_void_type v6(&write_five);
    
    // Invocation of a function
    global_int = 0;
    v6();
    assert(global_int == 5);
    
    // clear() method
    v6 = func_void_type();
    assert(!v6);
    
    // Assignment to an empty function
    v6 = three;
    assert(v6);
    
    // Invocation
    global_int = 0;
    v6();
    assert(global_int == 3);
    
    // Assignment to a non-empty function
    v6 = five;
    
    // Invocation
    global_int = 0;
    v6();
    assert(global_int == 5);
    
    // clear()
    v6 = func_void_type();
    assert(!v6);
    
    // Assignment to an empty function from a free function
    v6 = &write_five;
    assert(v6);
    
    // Invocation
    global_int = 0;
    v6();
    assert(global_int == 5);
    
    // Assignment to a non-empty function from a free function
    v6 = &write_three;
    assert(v6);
    
    // Invocation
    global_int = 0;
    v6();
    assert(global_int == 3);
    
    // Assignment
    v6 = five;
    assert(v6);
    
    // Invocation
    global_int = 0;
    v6();
    assert(global_int == 5);
    
    // Const vs. non-const
    write_const_1_nonconst_2 one_or_two;
    const n::function<void ()> v7(one_or_two);
    n::function<void ()> v8(one_or_two);
    
    global_int = 0;
    v7();
    assert(global_int == 2);
    
    global_int = 0;
    v8();
    assert(global_int == 2);
    
    // Test construction from 0 and comparison to 0
    func_void_type v9(nullptr);
    assert(!v9);
    
    // Test return values
    typedef n::function<int ()> func_int_type;
    generate_five_obj gen_five;
    generate_three_obj gen_three;
    
    func_int_type i0(gen_five);
    
    assert(i0() == 5);
    i0 = gen_three;
    assert(i0() == 3);
    i0 = &generate_five;
    assert(i0() == 5);
    i0 = &generate_three;
    assert(i0() == 3);
    assert(i0 ? true : false);
    i0 = func_int_type();
    assert(!i0 ? true : false);
    
    // Test return values with compatible types
    typedef n::function<long ()> func_long_type;
    func_long_type i1(gen_five);
    
    assert(i1() == 5);
    i1 = gen_three;
    assert(i1() == 3);
    i1 = &generate_five;
    assert(i1() == 5);
    i1 = &generate_three;
    assert(i1() == 3);
    assert(i1? true : false);
    
    // doesn't work in MSVC2010
    //     i1 = func_int_type();
    //     assert(!i1? true : false);
} */

/*

static void
test_one_arg()
{
    std::negate<int> neg;
    
    n::function<int (int)> f1(neg);
    assert(f1(5) == -5);
    
    n::function<std::string (std::string)> id(&identity_str);
    assert(id("str") == "str");
    
    n::function<std::string (const char*)> id2(&identity_str);
    assert(id2("foo") == "foo");
    
    add_to_obj add_to(5);
    n::function<int (int)> f2(add_to);
    assert(f2(3) == 8);
    
    const n::function<int (int)> cf2(add_to);
    assert(cf2(3) == 8);
}

static void
test_two_args()
{
    n::function<std::string (const std::string&, const std::string&)> cat(&string_cat);
    assert(cat("str", "ing") == "string");
    
    n::function<int (short, short)> sum(&sum_ints);
    assert(sum(2, 3) == 5);
}

static void
test_emptiness()
{
    n::function<float ()> f1;
    assert(!f1);
    
    n::function<float ()> f2;
    f2 = f1;
    assert(!f2);
    
    // doesn't work in MSVC2010
    
    //     n::function<double ()> f3;
    //     f3 = f2;
    //     assert(!f3);
}

struct add_with_throw_on_copy {
    int operator()(int x, int y) const { return x+y; }
    
    add_with_throw_on_copy() {}
    
    add_with_throw_on_copy(const add_with_throw_on_copy&)
    {
        throw std::runtime_error("But this CAN'T throw");
    }
    
    add_with_throw_on_copy& operator=(const add_with_throw_on_copy&)
    {
        throw std::runtime_error("But this CAN'T throw");
    }
};

static void
test_ref()
{
    add_with_throw_on_copy atc;
    try {
        // TODO:
        //         n::function<int (int, int)> f(std::ref(atc));
        //         assert(f(1, 3) == 4);
    }
    catch(std::runtime_error e) {
        assert(!"Nonthrowing constructor threw an exception");
    }
}

static void dummy() {}

static void test_empty_ref()
{
    n::function<void()> f1;
    n::function<void()> f2(std::ref(f1));
    
    try {
        f2();
        assert(!"Exception didn't throw for reference to empty function.");
    }
    catch(n::bad_function_call const&)
    {
    }
    
    f1 = dummy;
    
    try {
        f2();
    }
    catch(std::bad_function_call e) {
        assert(!"Error calling referenced function.");
    }
}


static void test_exception()
{
    n::function<int (int, int)> f;
    try {
        f(5, 4);
        assert(false);
    }
    catch(n::bad_function_call) {
        // okay
    }
}

typedef n::function< void * (void * reader) > reader_type;
typedef std::pair<int, reader_type> mapped_type;

static void test_implicit()
{
    mapped_type m;
    m = mapped_type();
}

static void test_call_obj(n::function<int (int, int)> f)
{
    assert(f);
}

static void test_call_cref(const n::function<int (int, int)>& f)
{
    assert(f);
}

static void test_call()
{
    test_call_obj(std::plus<int>());
    test_call_cref(std::plus<int>());
}

struct big_aggregating_structure
{
    int disable_small_objects_optimizations[32];
    
    big_aggregating_structure()
    {
        ++global_int;
    }
    
    big_aggregating_structure(const big_aggregating_structure&)
    {
        ++global_int;
    }
    
    ~big_aggregating_structure()
    {
        --global_int;
    }
    
    void operator()()
    {
        ++global_int;
    }
    
    void operator()(int)
    {
        ++global_int;
    }
};

template <class FunctionT>
static void test_move_semantics()
{
    typedef FunctionT f1_type;
    
    big_aggregating_structure obj;
    
    f1_type f1 = obj;
    global_int = 0;
    f1();
    
    assert(f1);
    assert(global_int == 1);
    
    // Testing rvalue constructors
    f1_type f2(static_cast<f1_type&&>(f1));
    assert(!f1); // implementation defined
    assert(f2);
    assert(global_int == 1);
    f2();
    assert(global_int == 2);
    
    f1_type f3(static_cast<f1_type&&>(f2));
    assert(!f1);
    assert(!f2); // implementation defined
    assert(f3);
    assert(global_int == 2);
    f3();
    assert(global_int == 3);
    
    // Testing move assignment
    f1_type f4;
    assert(!f4);
    f4 = static_cast<f1_type&&>(f3);
    assert(!f1);
    assert(!f2);
    assert(!f3); // implementation defined
    assert(f4);
    assert(global_int == 3);
    f4();
    assert(global_int == 4);
    
    // Testing self move assignment
    f4 = static_cast<f1_type&&>(f4);
    assert(f4);
    assert(global_int == 4);
    
    // Testing, that no memory leaked when assigning to nonempty function
    f4 = obj;
    assert(f4);
    assert(global_int == 4);
    f1_type f5 = obj;
    assert(global_int == 5);
    f4 = static_cast<f1_type&&>(f5);
    assert(global_int == 4);
} */

int main() {
    test_zero_args();
    // Function
    {
        // Empty function
        
        n::function<void()> empty_fn_1;
        n::function<void(int)> empty_fn_2;
        n::function<void(int, double[5])> empty_fn_3;

        n::function<double()> empty_fn_4(nullptr);
        n::function<double(int[5])> empty_fn_5(nullptr);
        n::function<double(int[5], double[2])> empty_fn_6(nullptr);
        
        n::function<void()> const empty_fn_7;
        n::function<double(void*)> const empty_fn_8;
        n::function<double(void*, void *[5])> const empty_fn_9;
        
        assert(!empty_fn_1 && !bool(empty_fn_1));
        assert(!empty_fn_2 && !bool(empty_fn_2));
        assert(!empty_fn_3 && !bool(empty_fn_3));
        assert(!empty_fn_4 && !bool(empty_fn_4));
        assert(!empty_fn_5 && !bool(empty_fn_5));
        assert(!empty_fn_6 && !bool(empty_fn_6));
        assert(!empty_fn_7 && !bool(empty_fn_7));
        assert(!empty_fn_8 && !bool(empty_fn_8));
        assert(!empty_fn_9 && !bool(empty_fn_9));
    }
    
    {
        // Return values
        
        n::function<void()> f1(&void_f_void);
        static_assert(std::is_same<std::result_of<n::function<void()>()>::type, void>::value, "");
        assert(!!f1 && f1);
        f1();
    
        n::function<int()> f2(&int_f_void);
        static_assert(std::is_same<std::result_of<n::function<int()>()>::type, int>::value, "");
        assert(f2() == 11);
        
        n::function<std::string()> f3(&string_f_void);
        static_assert(std::is_same<std::result_of<n::function<std::string()>()>::type, std::string>::value, "");
        assert(f3() == "string_f_void");
        
        n::function<int const()> f4(&const_int_f_void);
        // With GCC result type is int, in MSVS2010 result type is const int
        //static_assert(std::is_same<std::result_of<n::function<int const()>()>::type, const int>::value, "");
        assert(f4() == 31);
    
        
        n::function<std::string const()> f5(&string_const_f_void);
        static_assert(std::is_same<std::result_of<n::function<std::string const()>()>::type, std::string const>::value, "");
        assert(f5() == "string_const_f_void");
        
        n::function<void *()> f6(&void_ptr_f_void);
        static_assert(std::is_same<std::result_of<n::function<void *()>()>::type, void *>::value, "");
        assert(f6() == reinterpret_cast<void *>(20));
        
        n::function<int * ()> f7(&int_ptr_f_void);
        static_assert(std::is_same<std::result_of<n::function<int *()>()>::type, int *>::value, "");
        assert(f7() == reinterpret_cast<void *>(21));
    
        n::function<std::string * ()> f8(&string_ptr_f_void);
        static_assert(std::is_same<std::result_of<n::function<std::string *()>()>::type, std::string *>::value, "");
        assert(f8() == reinterpret_cast<std::string *>(22));
        
        n::function<std::string const * const()> f9(&string_const_ptr_const_f_void);
        // With GCC result type is std::string const *, in MSVS2010 result type is std::string const * const
        //static_assert(std::is_same<std::result_of<n::function<std::string const * const()>()>::type, std::string const * const>::value, "");
        assert(f9() == reinterpret_cast<std::string *>(52));
    
        n::function<std::string &()> f10(&string_ref_f_void);
        static_assert(std::is_same<std::result_of<n::function<std::string &()>()>::type, std::string &>::value, "");
        assert(&f10() == &res_str);
        
        n::function<std::string const &()> f11(&string_const_ref_f_void);
        static_assert(std::is_same<std::result_of<n::function<std::string const &()>()>::type, std::string const &>::value, "");
        assert(&f11() == &res_str);
    
        Func F12;
        n::function<size_t ()> f12(F12);
        assert(f12() == 70);
        ConstFunc F13;
        n::function<size_t ()> f13(F13);
        static_assert(std::is_same<std::result_of<n::function<size_t ()>()>::type, size_t>::value, "");
        assert(f13() == 71);
    
        n::function<int()> f14(f2);
        assert(f14() == 11);
    }
    
    {
        // Arguments
        
        // TODO:
        // basic
        // reference
        // const reference
        // multiple arguments
        // function with default arguments
    }
    
    {
        // Calling empty function
        
        bool raised = false;
        try
        {
            n::function<void(int)> bad_func;
            bad_func(5);
        }
        catch (n::bad_function_call const &)
        {
            raised = true;
        }
        assert(raised);
        
        raised = false;
        try
        {
            n::function<void(int)> bad_func(nullptr);
            bad_func(6);
        }
        catch (n::bad_function_call const &)
        {
            raised = true;
        }
        assert(raised);
    }
    
    {
        // Constant operator()
        
        n::function<int()> const f2(&int_f_void);
        assert(f2() == 11);
    }
    /*
    {
        // Swap
        
        n::function<std::string(int)> f1([](int v)->std::string { return "f1: " + std::to_string(v); });
        assert(f1(123) == "f1: 123");
        
        n::function<std::string(int)> f2([](int v)->std::string { return "f2: " + std::to_string(v); });
        assert(f2(321) == "f2: 321");
        
        using std::swap;
        swap(f1, f2);
        
        assert(f1(11) == "f2: 11");
        assert(f2(22) == "f1: 22");
        
        f1.swap(f2);
        assert(f1(111) == "f1: 111");
        assert(f2(222) == "f2: 222");
    }
    
    {
        // Assign
        
        n::function<std::string(int)> f1([](int v)->std::string { return "f1: " + std::to_string(v); });
        assert(f1(123) == "f1: 123");
        
        n::function<std::string(int)> const f2([](int v)->std::string { return "f2: " + std::to_string(v); });
        assert(f2(321) == "f2: 321");
        
        f1 = f2;
        assert(f1(1) == "f2: 1");
        assert(f2(2) == "f2: 2");
    }
    
    // Tests from Boost.Function
    test_zero_args();
    test_one_arg();
    test_two_args();
    test_emptiness();
    test_ref();
    test_empty_ref();
    test_exception();
    test_implicit();
    test_call();
    test_move_semantics<n::function<void()> >(); */
}





