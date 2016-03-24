# stringAlgorithms

## Introduction 

Implmentation of the following algorithms in C++

* Needleman-Wunsch
* Hirschberg (eventually)

## Installation 

The algorithms are templated code which only requiring copying the C++ header files 

To build the tests and examples you will need a system with autotools and CUnit. 

    $ autoreconf -i 
    $ ./configure 
    $ make

## Usage 

The following code will produce two strings, s and t, from x and y with the edits to convert one string to the next. 
The indels are indicated by a '-' in the string where the insertion needs to be made by the character in the other string.
The scoring function for match/changed characters is defined by plus_minus_one and can be any callable object
(functor, function pointer, lambda, std::function). Lambda's appear to be the fastest by a good margin though 
locally defined functors are reasonably cloes.  

The displayDiff() function will display the two strings hilighting the differences via cyan and red coloring on most
terminals. 


    #inclde "needlemanwunsch.hpp"
    #include "stringoutput.hpp"

    using stringAlgorithms::NeedlemanWunsch;
    using stringAlgorithms::displayDiff;
    using stringAlgorithms::scoring::plus_minus_one;

    std::string x = "...";
    std::string y = "...";

    std::string s, t;

    NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one);
    displayDiff(s, t);


## Definitions 

    template<typename I, typename BI, typename F>
    void NeedlemanWunsch(I xb, I xe, I yb, I ye, BI wb, BI zb,  
       F &&score_function, const typename std::iterator_traits<I>::value_type &&delChar = '-', typename PP<F, I>::type ID = -1)

+ I is a sequential iterator 
+ BI is a back inserter
+ F is a callable type 
+ delChar is the character to use to indicate a deletion. If the iterators do not point to a char then you should probably override this
+ ID is the penalty for an indel

And

    template<template<typename, typename...> class T, typename C, typename... Args>
    void displayDiff(T<C, Args...> a, T<C, Args...> b, C delChar = '-')

* T is the container type 
* C is the type of the delChar which indicates a deletion 
* Args is a place holder for the structures additional arguments 

This depends on how C is output and is likely suboptimal for non-character types unless you overload the output function 

## Notes

When computing the score algorithm for Needleman Wunsch the matrix is a std::vector<std::vector<T>> where T is the return type of the callable type F. It's important to ensure that the return type has enough space to store the score of the largest magnitude that might be computed. 

## Version 

The version will be held below 1.0 until both Needleman-Wunsch and Hirschberg's are implemented.  
