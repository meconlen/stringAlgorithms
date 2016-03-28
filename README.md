# stringAlgorithms

## Introduction 

Implmentation of the following algorithms in C++

* Needleman-Wunsch
* Hirschberg
# Longest Common Subsequence (using Hirschberg)

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

The display_diff() function will display the two strings hilighting the differences via cyan and red coloring on most
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

### NeedlemanWunsch

    template<typename I, typename BI, typename F>
    void NeedlemanWunsch(I x_begin, I x_end, I y_begin, I y_end, BI w_back, BI z_back, F &&score_function, typename PP<F, I>::type ID = -1,
       const typename std::iterator_traits<I>::value_type deleted_value = '-')

#### Template Parameters 

+ I is a sequential iterator 
+ BI is a back inserter
+ F is a callable type 

#### Function Parameters

+ x_begin, x_end are the begin and end of the first container
+ y_begin, y_end are the begin and end of the second container
+ w_back and z_back are back inserters to containers which will contain the strings with indels 
+ score_function is a callable object which will take two elements pointed to by I and return the penalty for editing from one to the next 
+ ID is the penalty for an indel
+ deleted_value is the character to use to indicate a deletion. If the iterators do not point to a char then you should probably override this

### Hirschberg

    template<typename I, typename BI, typename F>
    void Hirschberg(I x_begin, I x_end, I y_begin, I y_end, BI &&w_back, BI &&z_back, F &&score_function, typename PP<F, I>::type ID = -1,
       const typename std::iterator_traits<I>::value_type deleted_value = '-')

#### Template Parameters

+ I is a sequential iterator
+ BI is a back inserter 
+ F is a callable type

#### Function Parameters 

+ x_begin, x_end are the begin and end of the first container
+ y_begin, y_end are the begin and end of the second container
+ w_back and z_back are back inserters to containers which will contain the strings with indels 
+ score_function is a callable object which will take two elements pointed to by I and return the penalty for editing from one to the next 
+ ID is the penalty for an indel
+ deleted_value is the character to use to indicate a deletion. If the iterators do not point to a char then you should probably override this


### longest_common_subsequence

Note: I implemented this using Hirschberg since by the time the CPU disadvantage becomes substantial the memory advantage is probably worthwhile
You can easily replace it with Needleman-Wunsch

    template<typename I, typename BI>
    void longest_common_subsequence(I x_begin, I x_end, I y_begin, I y_end, BI z_back, 
       typename std::iterator_traits<I>::value_type deleted_value = '-')

#### Template Parameters

+ I is a sequential iterator
+ BI is a back inserter 

#### Function Parameters

+ x_begin, x_end are the begin and end of the first container
+ y_begin, y_end are the begin and end of the second container
+ z_back is a back inserter to store the longest common subsequence 
+ deleted_value is the value used to indicate deletions  

### display_diff

   template<template<typename, typename...> class T, typename C, typename... Args>
   void display_diff(T<C, Args...> a, T<C, Args...> b, C deleted_indicator = '-', char deleted_value = '-')

#### Template Parameters

+ T is the container type 
+ C is the type of the delChar which indicates a deletion 
+ Args is a place holder for the structures additional arguments 

#### Function Parameters

+ a and b are containers 
+ deleted_indicator is the value that indicates an indel
+ deleted_value is the value output when a indel is indicated 


This depends on how C is output and is likely suboptimal for non-character types unless you overload the output function 

## Notes

When computing the score algorithm for Needleman Wunsch the matrix is a std::vector<std::vector<T>> where T is the return type of the callable type F. It's important to ensure that the return type has enough space to store the score of the largest magnitude that might be computed. 

## Version 

The version will be held below 1.0 until both Needleman-Wunsch and Hirschberg's are implemented.  
