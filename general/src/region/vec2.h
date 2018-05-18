#ifndef general_region_vec2_h
#define general_region_vec2_h

#include <cmath>
#include <type_traits>

namespace region {
    template <typename T>
    struct vec2 {
        T x;
        T y;
        
        vec2(const T newX = 0, const T newY = 0) : x(newX), y(newY) {
            
        }
        
        template<typename destT>
        vec2<destT> convert_to() const {            
            return vec2<destT>(x, y);
        }
        
        template<typename destT>
        destT cast_to() const {
            static_assert(sizeof(destT) == sizeof(vec2<T>));
            static_assert(std::is_trivially_copyable<vec2<T>>::value);
            return *((destT*)this);
        }
        
        template<typename srcT>
        static vec2<T> cast_from(const srcT c) {
            static_assert(sizeof(srcT) == sizeof(vec2<T>));
            static_assert(std::is_trivially_copyable<vec2<T>>::value);
            return (vec2<T>)c;
        }
        
        void normalize() {
            T sum = length();
            x /= sum;
            y /= sum;
        }
        
        T length() {
            return std::sqrt(x * x + y * y);
        }
       
        vec2 operator*=(const T c) {
            x *= c;
            y *= c;
            
            return *this;
        }
        
        vec2 operator*(const T c) {
            vec2 result = *this;
            result *= c;
            return result;
        }                   
                
        vec2 operator*(const vec2<T> v) {
            vec2 result = *this;
            result *= v;
            return result;
        }
        
        vec2 operator+=(const T c) {
            x += c;
            y += c;
            
            return *this;
        }
        
        vec2 operator+(const T c) {
            vec2 result = *this;
            result += c;
            return result;
        }
        
        vec2 operator+=(const vec2<T> v) {
            x += v.x;           
            y += v.y;
            return *this;
        }
        
        vec2 operator+(const vec2<T> v) {
            vec2 result = *this;
            result += v;
            return result;
        }
    };
}

#endif

