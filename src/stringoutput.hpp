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

   template<template<typename, typename...> class T, typename C, typename... Args>
   void display_diff(T<C, Args...> a, T<C, Args...> b, C delIndicator = '-', char delChar = '-')
   {

      auto a_cur = a.begin(), b_cur = b.begin();

      std::ostringstream as, bs;

      as << "[ ";
      bs << "[ ";

      while(a_cur != a.end() && b_cur != b.end()) {
         if(*a_cur == delIndicator) {
            as << delChar;
            bs << colorStrings[CYAN] << *b_cur << colorStrings[NORMAL];
         } else if(*b_cur == delIndicator) {
            as << colorStrings[CYAN] << *a_cur << colorStrings[NORMAL];
            bs << delChar;
         } else if(*a_cur != *b_cur) {
            as << colorStrings[RED] << *a_cur << colorStrings[NORMAL];
            bs << colorStrings[RED] << *b_cur << colorStrings[NORMAL];
         } else {
            as << *a_cur;
            bs << *b_cur ;
         }
         a_cur++; b_cur++;
         if(a_cur == a.end() || b_cur == b.end()) {
            as << " ]";
            bs << " ]";
         } else {
            as << ", ";
            bs << ", ";
         }
      }


      std::cout << "a = " << as.str() << std::endl;
      std::cout << "b = " << bs.str() << std::endl;
      std::cout << std::endl;
      return;
   }

   template<>
   void display_diff(std::string a, std::string b, char delIndicator, char delChar)
   {
     auto a_cur = a.begin(), b_cur = b.begin();

      std::ostringstream as, bs;

      while(a_cur != a.end() && b_cur != b.end()) {
         if(*a_cur == delChar) {
            as << *a_cur;
            bs << colorStrings[CYAN] << *b_cur << colorStrings[NORMAL];
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
