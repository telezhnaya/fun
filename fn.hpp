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
        
        template<typename T>
        using function_type = typename std::enable_if<std::is_convertible<T, Ret>::value, Ret>::type;
        
        template<typename T>
        using free_function = function_type<T>(*)(Args...);
        
        struct holder {
            holder() {}
            virtual ~holder() {}
            virtual Ret invoke(Args&&... args) = 0;
            
            virtual std::unique_ptr<holder> clone() = 0;
        
            holder(holder const&) = delete;
            void operator = (holder const&) = delete;
        };
        
        class function_holder : public holder {
            free_function<Ret> function_;
        public:
            function_holder(free_function<Ret> func) : function_(func) {}
            function_holder(function_holder const& x) : function_(x.function_) {}
            virtual Ret invoke(Args&&... args) {
                return function_(std::forward<Args>(args)...);
            }
            
            virtual std::unique_ptr<holder> clone() {
                return std::unique_ptr<holder>(new function_holder(function_));
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
            
            virtual std::unique_ptr<holder> clone() {
                return std::unique_ptr<holder>(new functor_holder(*functor_.get())); //вообще неведомая хрень
            }
        };
        
        
        
        std::unique_ptr<holder> function_;
        
    public:
        
        function() noexcept : function_(nullptr) {}
        
        function(std::nullptr_t fn) noexcept : function_(nullptr) {}
        
        template<typename T>
        function(free_function<T> fn) : function_(std::make_unique<function_holder>(fn)) {}
        
        //function(free_function fn) : function_(std::make_unique<function_holder>(fn)) {}
        
        template <typename Struct>
        function(Struct const& fn) : function_(std::make_unique<functor_holder<Struct>>(fn)) {}
        
        function(function&& x) = default;
        
        function(function const& x) { //
            if (x.function_ == nullptr) {
                function_ = nullptr;
            } else {
                function_ = x.function_.get()->clone();
            }
        }
        
        function& operator= (function const& x) { //самой сбе я не сделала, но уже работает. не знаю, надо ли делать
            if (x.function_ == nullptr) {
                function_ = nullptr;
            } else {
                function_ = x.function_.get()->clone();
            }
            return *this;
        }
        
        Ret operator() (Args&&... args) const {
            if(!function_) throw bad_function_call();
            return function_->invoke(std::forward<Args>(args)...);
        }
        
        operator bool() const {
            return (bool)function_;
        }
        
        void swap(function& a) {
            std::swap(*this, a);
        }
        
    };
    
    template<typename T>
    void swap(function<T>& a, function<T>& b) {
        std::swap(a, b);
    }
}

#endif