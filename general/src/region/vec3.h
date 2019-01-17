#ifndef general_region_vec3_h
#define general_region_vec3_h

#include <cmath>

namespace region {
    template <typename T>
    struct vec3 {
        T x;
        T y;
        T z;
        
        vec3(const T newX = 0, const T newY = 0, const T newZ = 0) : x(newX), y(newY), z(newZ) {
            
        }
        
        template<typename destT>
        vec3<destT> convertTo() const {            
            return vec3<destT>(x, y, z);
        }
        
        void normalize() {
            T sum = length();
            x /= sum;
            y /= sum;
            z /= sum;
        }
        
        T length() {
            return std::sqrt(x * x + y * y + z * z);
        }
       
        vec3 operator*=(const T c) {
            x *= c;
            y *= c;
            z *= c;
            
            return *this;
        }
        
        vec3 operator*(const T c) {
            vec3 result = *this;
            result *= c;
            return result;
        }                   
        
        vec3 operator*=(const vec3<T> v) {
            vec3 tmp;
            tmp.x = y * v.z - z * v.y;
            tmp.y = z * v.x - x * v.z;
            tmp.z = x * v.y - y * v.x;
            
            *this = tmp;            
            return tmp;
        }
        
        vec3 operator*(const vec3<T> v) {
            vec3 result = *this;
            result *= v;
            return result;
        }
        
        vec3 operator+=(const T c) {
            x += c;
            y += c;
            z += c;
            
            return *this;
        }
        
        vec3 operator+(const T c) {
            vec3 result = *this;
            result += c;
            return result;
        }
        
        vec3 operator+=(const vec3<T> v) {
            x += v.x;           
            y += v.y;
            z += v.z;
            return *this;
        }
        
        vec3 operator-=(const vec3<T> v) {
            x -= v.x;           
            y -= v.y;
            z -= v.z;
            return *this;
        }
        
        vec3 operator+(const vec3<T> v) {
            vec3 result = *this;
            result += v;
            return result;
        }
        
        bool operator==(const vec3<T> v) {
            return x == v.x && y == v.y && z == v.y;
        }
        
        bool operator!=(const vec3<T> v) {
            return x != v.x || y != v.y || z != v.z;
        }
    };
}

#endif
