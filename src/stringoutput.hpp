#ifndef STRINGOUTPUT_HPP
#define STRINGOUTPUT_HPP

#include <iostream>
#include <sstream>
#include <string>

namespace stringAlgorithms {
   // useful for printing vectors  
   template<typename T>
   std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
   {
      os << "{ ";
      for(auto i = v.begin(); i != v.end(); i++) {
         os << *i; 
         if(i+1 != v.end()) {
            os << ", ";
         } 
      }
      os << " }";
      return os;
   }

   // useful for printing matricies
   template<typename T>
   std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& m)
   {
      for(auto &i : m) {
         std::cout << i << std::endl;
      }
      return os;
   }

   template<template<typename, typename...> class T, typename C, typename... Args>
   void displayDiff(T<C, Args...> a, T<C, Args...> b, C delChar = '-')
   {
      enum outputColors { BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, NORMAL };
      char colorStrings[][6] = {
         "\033[30m",
         "\033[31m",
         "\033[32m",
         "\033[33m",
         "\033[34m",
         "\033[35m",
         "\033[36m",
         "\033[37m",
         "\033[0m"
      };

      auto a_cur = a.begin(), b_cur = b.begin();

      std::ostringstream as, bs;

      while(a_cur != a.end() && b_cur != b.end()) {
         if(*a_cur == delChar) {
            as << *a_cur;
            bs << colorStrings[CYAN] << *b_cur;
         } else if(*b_cur == delChar) {
            as << colorStrings[CYAN] << *a_cur << colorStrings[NORMAL];
            bs << *b_cur;
         } else if(*a_cur != *b_cur) {
            as << colorStrings[RED] << *a_cur << colorStrings[NORMAL];
            bs << colorStrings[RED] << *b_cur << colorStrings[NORMAL];
         } else {
            as << *a_cur;
            bs << *b_cur;
         }
         a_cur++; b_cur++;
      }


      std::cout << "a = " << as.str() << std::endl;
      std::cout << "b = " << bs.str() << std::endl;
      std::cout << std::endl;
      return;
   }
} // stringAlgorithm
#endif
