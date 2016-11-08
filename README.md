# Vector Wrapper #
![Image Build status](https://travis-ci.org/KingDuckZ/vectorwrapper.svg?branch=master)

## Motivation ##
### Short version ###
A vector wrapper is useful if used as an adapter between vectors from different libraries. It removes the need to explicitly construct a vector of type B when you have a vector of type A.
It's roughly equivalent to using a reinterpret_cast on your vectors, but the code tries really hard to make sure the cast is valid in all cases, wrapping safety checks nicely behind a clean interface. Implicit constructors also allow to convert easily from one type to the other (if enabled at build time).

### Long version ###
We all need vectors in our code, and there are quite a few implementations out there. They all do their job, and in terms of interface they are pretty similar to each other, but not *exactly* the same thing.
While this is not an issue in its own, it can become a problem when your code depends on external libraries that will provide their own vector implementation. So you might have your own implementation, MyVector, and the library will give you Ogre::Vector3, which is what you must use when you invoke Ogre's functions. If you also depend on openCV you will have yet another vector around. Microsoft Kinect SDK also imposes you its own, barebone vector implementation.

While through discipline you can manage such cases more or less well, there is one major issue that I couldn't work around: your own library code. I like to write code I think I will re-use at some point in the future in libraries separate from the main program. Odds are at some point you will need some maths that are not covered by the library vector you are using. Unless it's something really particular, I will probably try to write it in some library so that other projects can benefit from my new formula. The good thing about low level libraries is that they have very few dependencies, or none at all, and can thus be carried around and adapted to a variety of projects. It's all nice and good, but how would you add your new function taking a Ogre::Vector3 into a library that depends on nothing without bringing in the whole Ogre3D library?

This is where my class comes into play.

## Practical example ##
So what's the real benefit of using `Vec<Ogre::Vector3>` instead of just `Ogre::Vector3`, for example? Can't I just assign/cast manually?

Sure you can, but it's more verbose. And as your codebase grows, you will have more and more boundaries where you have to convert from one vector to the other just to call a function from this or that library. Check test/unit/example.cpp for a head-to-head comparison of the two methods.

## Implementation ##
The basic idea is that all vectors will somehow give you access to some x, y, z coordinates. You will do that very frequently in your code, maybe on different platforms and in time critical parts of your code, so virtual calls (or equivalent) are not an option. This library is entirely templated, and will let you wrap any type inside a Vec. In fact, you could even just use this wrapper as your vector of choice, using an `std::array<float, 3>` as the wrapped type and forget how it's working under the hood.

Unfortunately the fact that you use `Vec<std::array<float, 3>>` and `Vec<Ogre::Vector3>` also means that you still have several types of vectors in your program. The good news is that conversion is automated (no more `Ogre::Vector3(other_vec.X, other_vec.Y, other_vec.Z)` in your code) and often it will not even be necessary, as casting will suffice.

You still need to type some code in order to get started using the vector wrapper class, which is basically your custom implementation of a struct listing informations about the wrapped type. That's not a lot of code.

## Features ##
### Build time configuration ###
* VWR_WITH_IMPLICIT_CONVERSIONS (default: not defined) Enable implicit conversions between wrappers of vectors of different type (see Automated conversions).

### Automated conversion ###
You can assign or construct a `Vec<A>` from a `Vec<B>`, provided they have the same dimensions and the assignemnt operator is able to convert B's scalar type to that of A. You need to define VWR_WITH_IMPLICIT_CONVERSIONS for this to work.

### Manual conversions ###
You can convert one vector to the other using vector_cast. For example, auto b = vector_cast<float2>(a); will create a temporary of type float2 the whose elements are a static_cast'ed copy of the elements in a and assign it to b. You need to include vectorwrapper/vector_cast.hpp.

### Access the wrapped type ###
Through the `data()` method you can always obtain a ref to the wrapped type.

### Dimension change ###
Depending on the wrapped type, Vec can provide methods to get a Vec of lower or higher dimensions. For example, `Vec<Ogre::Vector3>` can optionally expose a `xy()` method that is equivalent to `Vec<Ogre::Vector2>(vec3.x(), vec3.y())`.

### Casting ###
A must have some fundamental properties before it can be casted to B:
* casting won't result in a misaligned object
* the casted object won't go past the memory reserved to the original object (in other words sizeof(B) <= sizeof(A) - offset of the first coordinate in A)
* B must be a standard layout struct
* B has no data before the first coordinate
* Coordinates in B appear in the same order and at the same offsets as they appear in A
* B is exactly as large as dimensions * sizeof(scalar_type), but you can disable this constraint by setting cast_ignore_trailing_properties=1 in B's VectorWrapperInfo

For example you could conceptually wrap a type A:

    struct A {
        int something;
        float x, y, z;
        std::string something_else;
    };

and still have it castable to type B:

    struct B {
        float x, y, z;
    };

Vectors must still have the same dimensions. `cast<type>()` is only available when you define `offset_x` (see <u>Usage</u>). Either way, to cast (reinterpret_cast actually, be careful about the strict aliasing rule) you can just do the following:

    typedef Vec<A> avec;
    typedef Vec<B> bvec;
    
    avec a(0);

    some_function(bvec(a)); //creates a temporary
    some_function(a.cast<bvec>()); //reinterpret_cast

## Usage ##
In this example we will adapt `std::array<float, N>` and `Ogre::VectorN`. In your real code, write the VectorWrapperInfo specializations for your own wrapped types in some header file, and just include it in your code to use Vec with those types.

    //Include Vec's header
    #include "vectorwrapper.hpp"
    #include <array>
    #include <cstddef>
    
    template <>
    struct VectorWrapperInfo<std::array<float, 3>> {
        enum {
            //Tell Vec this is a 3D vector
            dimensions = 3
        };
        
        //The inner type is a float
        typedef float scalar_type;
        
        //Not required by Vec but useful for us
        typedef std::array<float, 3> vector_type;
        
        //This will make the xy(), xz() and yz() methods available
        typedef std::array<float, 2> lower_vector_type;
        
        static scalar_type& get_at (size_t parIndex, vector_type& parVector) {
            //parIndex is already asserted to be < dimensions
            //so you can always assume it's valid.
            return parVector[parIndex];
        }
    };
    
    template <>
    struct VectorWrapperInfo<std::array<float, 2>> {
        enum {
            dimensions = 2
        };
        
        typedef float scalar_type;
        typedef std::array<float, 2> vector_type;
        typedef std::array<float, 3> higher_vector_type;
        typedef std::array<float, 1> lower_vector_type;
        
        static scalar_type& get_at (size_t parIndex, vector_type& parVector) {
            return parVector[parIndex];
        }
    };
    
    //Not really useful I suppose, but this is just an example
    template <>
    struct VectorWrapperInfo<float> {
        enum {
            dimensions = 1
        };
        
        typedef float scalar_type;
        typedef float vector_type;
        typedef std::array<float, 2> higher_vector_type;
        
        static scalar_type& get_at (size_t, vector_type& parVector) {
            return parVector;
        }
    };
    
    template <>
    struct VectorWrapperInfo<Ogre::Vector3> {
        enum {
            dimensions = 3
        };
        
        typedef Ogre::Real scalar_type;
        typedef Ogre::Vector3 vector_type;
        typedef Ogre::Vector2 lower_vector_type;
        
        //If you want, you can implement the get_at() method as we did
        //for std::array, returning *(&parVector.x + parIndex)
        //but you can let Vec do that for you and just provide some
        //offsets. By doing this, you also give a hint to Vec about
        //what the wrapped type's layout is, so it will enable
        //certain casts that would be disabled if you provided
        //the get_at() method instead.
        enum {
            offset_x = offsetof(Ogre::Vector3, x),
            offset_y = offsetof(Ogre::Vector3, y),
            offset_z = offsetof(Ogre::Vector3, z)
        };
    };
    
    template <>
    struct VectorWrapperInfo<Ogre::Vector2> {
        enum {
            dimensions = 2
        };
        
        typedef Ogre::Real scalar_type;
        typedef Ogre::Vector2 vector_type;
        typedef Ogre::Vector3 higher_vector_type;
        
        enum {
            offset_x = offsetof(Ogre::Vector3, x),
            offset_y = offsetof(Ogre::Vector3, y)
        };
    };
    
    typedef Vec<std::array<float, 3>> avec3;
    typedef Vec<std::array<float, 2>> avec2;
    typedef Vec<float> avec1;
    typedef Vec<Ogre::Vector3> ovec3;
    typedef Vec<Ogre::Vector2> ovec2;

## Limitations ##
* 4D vectors are not currently supported
* Read-only vectors are not supported (not sure if such a thing is really needed)
* I haven't profiled this code yet (please be patient), but while I don't want it to slow your program down, I know it won't be comparable to any super optimized vector library. This is not a vector library to begin with. Supporting SSE would be nice, but there is no guarantee the wrapped type has been created with SSE and optimizations in mind. Writing functions for both cases (SIMD-ready structures and scalar structures) would essentially require implementing every function twice. I think it's just a better idea to take an already made vector library and use vectorwrapper as it was intended - to wrap the vector classes that come from that library.
