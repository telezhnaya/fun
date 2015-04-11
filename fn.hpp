//
//  fn.hpp
//  std_function
//
//  Created by Olya Telezhnaya on 05.04.15.
//  Copyright (c) 2015 1. All rights reserved.
//

#ifndef STD_FUNCTION_HPP
#define STD_FUNCTION_HPP

#include <memory>
#include <utility>

namespace fn {
    
    namespace details {
        
    }
    
    struct bad_function_call : std::exception {
        const char* what() const noexcept {
            return "bad functiion called";
        }
    };
    
    template <typename Unused>
    class function;
    
    template <typename Ret, typename... Args>
    class function <Ret (Args...)> {
        
        using free_function = Ret(*)(Args...);
        
        template<typename T>
        using functor_function_t = Ret (T::*)(Args...); //какое-то сложноватое название
        
        struct holder {
            holder() {}
            virtual ~holder() {}
            virtual Ret invoke(Args&&... args) = 0;
        };
        
        class function_holder : public holder {
            free_function function_;
        public:
            function_holder(free_function func) : function_(func) {}
            function_holder(function_holder const& x) : function_(x.function_) {}
            virtual Ret invoke(Args&&... args) {
                return function_(std::forward<Args>(args)...);
            }
        };
        
        template <typename Struct>
        class functor_holder : public holder {
            std::unique_ptr<Struct> functor_;
        public:
            functor_holder(Struct const& functor) : functor_(std::make_unique<Struct>(functor)) {}
            functor_holder(functor_holder const& x) : functor_(new Struct(x.functor_)) {}
            virtual Ret invoke(Args&&... args) {
                return functor_->operator()(std::forward<Args>(args)...);
            }
        };
        
        std::unique_ptr<holder> function_;
        
    public:
        
        function() noexcept : function_(nullptr) {}
        
        function(std::nullptr_t fn) noexcept : function_(nullptr) {}
        
        function(free_function fn) : function_(std::make_unique<function_holder>(fn)) {}
        
        template <typename Struct>
        function(Struct& fn) : function_(std::make_unique<functor_holder<Struct>>(fn)) {}
        
        function(function&& x) = default;
        
        //function(function const& x) {}
        
        //function& operator= (function const& a) {}
        
        Ret operator() (Args&&... args) const {
            if(!function_) throw bad_function_call();
            return function_->invoke(std::forward<Args>(args)...);
        }
        
        operator bool() const {
            return (bool)function_;
        }
        
        //void swap(function a) {}
        
    };
    
  //  void swap(function<class T> a, function<class T> b) {}
}

#endif