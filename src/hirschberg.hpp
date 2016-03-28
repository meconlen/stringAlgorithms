#ifndef HIRSCHBERG_HPP
#define HIRSCHBERG_HPP

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
   nwScore(I xb, I xe, I yb, I ye, F && score_function, typename PP<F, I>::type ID = -1, typename PP<F, I>::type MAX = 0)
   {

      typedef typename PP<F, I>::type P;

      auto x_size = std::distance(xb, xe);
      auto y_size = std::distance(yb, ye);

      if(MAX > 0) {
         uint64_t    maxAdjustment = MAX;
         uint64_t    maxScore = std::min({ std::abs(std::numeric_limits<P>::max()), std::abs(std::numeric_limits<P>::min()) });
         uint64_t    maxStringLength = std::max({ x_size, y_size }); 
         if(maxAdjustment * maxScore <= maxStringLength) {
            throw std::overflow_error("Score type not large enough for Needleman-Wunsch score");
         }
      }


      std::vector<P>    top(y_size + 1);
      std::vector<P>    bottom(y_size + 1);

      top[0] = 0;
      for(auto j = 1; j <= y_size; j++) top[j] = top[j-1] + ID;

      auto x_cur = xb;
      for(auto i = 1; i <= x_size; i++) {
         bottom[0] = top[0] + ID;
         auto y_cur = yb;
         for(auto j = 1; j <= y_size; j++) {
            P scoreSub = top[j-1] + score_function(*x_cur, *y_cur);
            P scoreDel = top[j] + ID;
            P scoreIns = bottom[j-1] + ID;
            bottom[j] = std::max({scoreSub, scoreDel, scoreIns});
            y_cur++;
         }
         x_cur++;
         std::swap(top, bottom);
      }
      return top;
   }

   namespace {
      template<typename I>
      int_fast64_t partition(I lb, I le, I rb, I re) {
         auto L_cur = lb;
         auto R_cur = re - 1;
         auto ymid = 0;
         auto curMax = *L_cur + *R_cur;
         auto size = std::distance(lb, le);
         L_cur++; R_cur--;
         for(auto i = 1; i < size; i++) {
            auto curSum = *L_cur + *R_cur;
            if(curSum > curMax) {
               ymid = i;
               curMax = curSum;
            }
            L_cur++; R_cur--;
         }
         return ymid;
      }
   }

   template<typename I, typename BI, typename F>
   void Hirschberg(I xb, I xe, I yb, I ye, BI &&wb, BI &&zb,
      F &&score_function, const typename std::iterator_traits<I>::value_type delChar = '-', typename PP<F, I>::type ID = -1)
   {
      // typedef typename PP<F, I>::type P;

      int_fast64_t x_size = std::distance(xb, xe);
      int_fast64_t y_size = std::distance(yb, ye);
      if(x_size == 0) {
         std::fill_n(wb, y_size, delChar);
         std::copy(yb, ye, zb);
      } else if(y_size == 0) {
         std::copy(xb, xe, wb);
         std::fill_n(zb, x_size, delChar);
      } else if(x_size == 1 || y_size == 1) {
         NeedlemanWunsch(xb, xe, yb, ye, wb, zb, score_function, delChar, ID);
      } else {
         auto xmid = x_size / 2;

         auto ScoreL = nwScore(xb, xb+xmid, yb, ye, score_function, ID);
         
         std::vector<typename std::iterator_traits<I>::value_type> y_reverse;
         std::copy(yb, ye, back_inserter(y_reverse));
         std::reverse(y_reverse.begin(), y_reverse.end());

         std::vector<typename std::iterator_traits<I>::value_type> x_reverse;
         std::copy(xb+xmid, xe, back_inserter(x_reverse));
         std::reverse(x_reverse.begin(), x_reverse.end());


         auto ScoreR = nwScore(x_reverse.begin(), x_reverse.end(), y_reverse.begin(), y_reverse.end(), score_function, ID);

         auto ymid = partition(ScoreL.begin(), ScoreL.end(), ScoreR.begin(), ScoreR.end());

         Hirschberg(xb, xb+xmid, yb, yb+ymid, wb, zb, score_function, delChar, ID);
         Hirschberg(xb+xmid, xe, yb+ymid, ye, wb, zb, score_function, delChar, ID);

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