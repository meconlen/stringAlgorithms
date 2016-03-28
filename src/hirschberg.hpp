#ifndef HIRSCHBERG_HPP
#define HIRSCHBERG_HPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#ifdef HAVE_CUNIT_CUNIT_H
#include "stringoutput.hpp"
#include <CUnit/Basic.h>
#endif

#include "stringUtility.hpp"
#include "needlemanwunsch.hpp"

namespace stringAlgorithms {

   template<typename I, typename F>
   std::vector<typename PP<F, I>::type>
   nwScore(I x_begin, I x_end, I y_begin, I y_end, F &&score_function, typename PP<F, I>::type ID = -1)
   {

      typedef typename PP<F, I>::type P;

      auto x_size = std::distance(x_begin, x_end);
      auto y_size = std::distance(y_begin, y_end);

      std::vector<P>    top(y_size + 1);
      std::vector<P>    bottom(y_size + 1);

      top[0] = 0;
      for(auto j = 1; j <= y_size; j++) top[j] = top[j-1] + ID;

      auto x_cur = x_begin;
      for(auto i = 1; i <= x_size; i++) {
         bottom[0] = top[0] + ID;
         auto y_cur = y_begin;
         for(auto j = 1; j <= y_size; j++) {
            P score_sub = top[j-1] + score_function(*x_cur, *y_cur);
            P score_del = top[j] + ID;
            P score_ins = bottom[j-1] + ID;
            bottom[j] = std::max({score_sub, score_del, score_ins});
            y_cur++;
         }
         x_cur++;
         std::swap(top, bottom);
      }
      return top;
   }

   template<typename I>
   int_fast64_t partition(I left_begin, I left_end, I right_begin, I right_end) {
      typedef typename std::iterator_traits<I>::value_type I_type;
      std::reverse_iterator<I> reverse_right_begin(right_end);
      std::reverse_iterator<I> reverse_right_end(right_begin);

      std::vector<I_type> vector_sum(std::distance(left_begin, left_end));

      std::transform(left_begin, left_end, reverse_right_begin, vector_sum.begin(), [](const I_type &a, const I_type &b) -> I_type { return a + b; });

      auto max_p = std::max_element(vector_sum.begin(), vector_sum.end());
      return std::distance(vector_sum.begin(), max_p);
   }

   template<typename I, typename BI, typename F>
   void Hirschberg(I x_begin, I x_end, I y_begin, I y_end, BI &&w_back, BI &&z_back, F &&score_function, typename PP<F, I>::type ID = -1,
      const typename std::iterator_traits<I>::value_type deleted_value = '-')
   {
      int_fast64_t x_size = std::distance(x_begin, x_end);
      int_fast64_t y_size = std::distance(y_begin, y_end);

      if(x_size == 0) {
         std::fill_n(w_back, y_size, deleted_value);
         std::copy(y_begin, y_end, z_back);
      } else if(y_size == 0) {
         std::copy(x_begin, x_end, w_back);
         std::fill_n(z_back, x_size, deleted_value);
      } else if(x_size == 1 || y_size == 1) {
         NeedlemanWunsch(x_begin, x_end, y_begin, y_end, w_back, z_back, score_function, ID, deleted_value);
      } else {
         auto x_mid = x_size / 2;

         auto ScoreL = nwScore(x_begin, x_begin + x_mid, y_begin, y_end, score_function, ID);
         
         std::vector<typename std::iterator_traits<I>::value_type> y_reverse;
         std::copy(y_begin, y_end, back_inserter(y_reverse));
         std::reverse(y_reverse.begin(), y_reverse.end());

         std::vector<typename std::iterator_traits<I>::value_type> x_reverse;
         std::copy(x_begin + x_mid, x_end, back_inserter(x_reverse));
         std::reverse(x_reverse.begin(), x_reverse.end());


         auto ScoreR = nwScore(x_reverse.begin(), x_reverse.end(), y_reverse.begin(), y_reverse.end(), score_function, ID);

         auto y_mid = partition(ScoreL.begin(), ScoreL.end(), ScoreR.begin(), ScoreR.end());

         Hirschberg(x_begin, x_begin + x_mid, y_begin, y_begin + y_mid, w_back, z_back, score_function, ID, deleted_value);
         Hirschberg(x_begin + x_mid, x_end, y_begin + y_mid, y_end, w_back, z_back, score_function, ID, deleted_value);

      }
   }


#ifdef HAVE_CUNIT_CUNIT_H

   int init_hirschberg_suite(void) 
   {
      return 0;
   }

   int clean_hirschberg_suite(void)
   {
      return 0;
   }

   void hirschberg_nwScore_test(void)
   {

      std::string x = "GATTACA";
      std::string y = "GCATGCU";

      auto matrix = nwScore(x.begin(), x.end(), y.begin(), y.end(), scoring::plus_minus_one);

      decltype(matrix) expectedResult = { -7, -5, -3, -1, -2, -2,  0,  0 };

      CU_ASSERT(matrix == expectedResult);
      if(matrix != expectedResult) std::cout << std::endl << matrix << std::endl;

      return;
   }

   void hirschberg_test(void)
   {
      std::string x = "GATTACA";
      std::string y = "GCATGCU";

      std::vector<std::string> expectedResult = {
         "G-ATTACA",
         "GCA-TGCU" 
      };

      std::vector<std::string> result(2, std::string());

      Hirschberg(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), scoring::plus_minus_one);

      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
      return;
   }


   void hirschberg_2_test(void)
   {
      std::string x = "AGTACGCA";
      std::string y = "TATGC";

      std::vector<std::string> expectedResult = {
         "AGTACGCA",
         "--TATGC-"
      };
      std::vector<std::string> result(2, std::string());

      Hirschberg(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), scoring::plus_minus_one);

      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
      return;

   }

#endif // HAVE_CUNIT_CUNIT_H

} // stringAlgorithms

#endif