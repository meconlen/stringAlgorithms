#ifndef NEEDLEMANWUNSCH_HPP
#define NEEDLEMANWUNSCH_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "stringoutput.hpp"

#ifdef HAVE_CUNIT_CUNIT_H
#include <CUnit/Basic.h>
#endif

namespace stringAlgorithms {

   namespace scoring {

      auto plus_minus_one = [](const char &x, const char &y) -> int16_t { return x == y ? 1 : -1; };
   }

   // given a function F that takes two iterators I, PP<F, I>::type is the type of the return type of the function 
   // this can be used to deduce the return type of the function to determine what the type of the score matrix 
   // elements will be 

      template<typename F, typename I>
      struct PP {
         typedef typename std::decay<typename std::result_of<
            F(typename std::iterator_traits<I>::value_type, typename std::iterator_traits<I>::value_type)>::type
         >::type type;
      };

   // Compute the Needleman-Wunsch score matrix on a type 
   // look at use of std::function on large strings sometime
   
   template<typename I, typename F>
   std::vector<std::vector< typename PP<F, I>::type >> nw_score_matrix(I xb, I xe, I yb, I ye, 
      F &&score_function, typename PP<F, I>::type ID = -1, typename PP<F, I>::type MAX = 0)
   {

      typedef typename PP<F,I>::type P;
      
      auto x_size = std::distance(xb, xe);
      auto y_size = std::distance(yb, ye);

      // detect if computation will overflow for type P
      // This assumes that things fit inside a uint64_t. 

      if(MAX > 0) {
         uint64_t    maxAdjustment = MAX;
         uint64_t    maxScore = std::min({ std::abs(std::numeric_limits<P>::max()), std::abs(std::numeric_limits<P>::min()) });
         uint64_t    maxStringLength = std::max({ x_size, y_size }); 
         if(maxAdjustment * maxScore <= maxStringLength) {
            throw std::overflow_error("Score type not large enough for Needleman-Wunsch score");
         }
      }
      std::vector<std::vector<P>> score(x_size + 1, std::vector<P>(y_size + 1, 0));

      score[0][0] = 0;
      for(auto j = 1; j <= y_size; j++) score[0][j] = score[0][j-1] + ID; 

      auto x_cur = xb;
      for(auto i = 1; i <= x_size; i++) {
         score[i][0] = score[i-1][0] + ID;
         auto y_cur = yb;
         for(auto j = 1; j <= y_size; j++) {
            P scoreSub = score[i-1][j-1] + score_function(*x_cur, *y_cur);
            P scoreDel = score[i-1][j] + ID;
            P scoreIns = score[i][j-1] + ID;
            score[i][j] = std::max({scoreSub, scoreDel, scoreIns});
            y_cur++;
         }
         x_cur++;
      }

      return score;
   }

   template<typename I, typename BI, typename F>
   void NeedlemanWunsch(I xb, I xe, I yb, I ye, BI wb, BI zb,  
      F &&score_function, 
      const typename std::iterator_traits<I>::value_type &&delChar = '-', typename PP<F, I>::type ID = -1)
   {
      typedef typename PP<F,I>::type P;

      std::vector<std::vector<typename std::iterator_traits<I>::value_type>> result(2, std::vector<typename std::iterator_traits<I>::value_type>());

      std::vector<std::vector<P>> score = nw_score_matrix(xb, xe, yb, ye, std::move(score_function), ID);

      int_fast64_t   i = std::distance(xb, xe);
      int_fast64_t   j = std::distance(yb, ye);

      auto x_cur = xe - 1;
      auto y_cur = ye - 1;

      // work our way back to [0][0] constructing the strings in reverse order
      // we first see if we arrived by an match/substitution, if so up left
      // then we see if we arrived via deletion and if so move up 
      // then we must left 

      while(i != 0 || j != 0) {
         if((i > 0 && j > 0) && score[i][j] == score[i-1][j-1] + score_function(*x_cur, *y_cur)) {
            result[0].push_back(*x_cur);
            result[1].push_back(*y_cur);
            i--; j--;
            x_cur--; y_cur--;
         }
         else if((i > 0) && (score[i][j] == score[i-1][j] + ID)) {
            result[0].push_back(*x_cur);
            result[1].push_back(delChar);
            i--;     
            x_cur--;   
         }
         else {
            result[0].push_back(delChar);
            result[1].push_back(*y_cur);
            j--;
            y_cur--;      
         }
      }
      for(auto &i : result) std::reverse(i.begin(), i.end()); 

      std::copy(result[0].begin(), result[0].end(), wb);
      std::copy(result[1].begin(), result[1].end(), zb);

      return;
   }

#ifdef HAVE_CUNIT_CUNIT_H

   int init_nw_suite(void)
   {
      return 0;
   }

   int clean_nw_suite(void)
   {
      return 0;
   }

   void nwScoreMatrix_test(void)
   {
      // This test taken from 
      // https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm
      std::string x = "GATTACA";
      std::string y = "GCATGCU";

      std::vector<std::vector<int16_t>> matrix = nw_score_matrix(x.begin(), x.end(), y.begin(), y.end(), scoring::plus_minus_one);
      std::vector<std::vector<int16_t>> expectedResult = {  {  0, -1, -2, -3, -4, -5, -6, -7 },
                                                            { -1,  1,  0, -1, -2, -3, -4, -5 },
                                                            { -2,  0,  0,  1,  0, -1, -2, -3 },
                                                            { -3, -1, -1,  0,  2,  1,  0, -1 },
                                                            { -4, -2, -2, -1,  1,  1,  0, -1 },
                                                            { -5, -3, -3, -1,  0,  0,  0, -1 },
                                                            { -6, -4, -2, -2, -1, -1,  1,  0 },
                                                            { -7, -5, -3, -1, -2, -2,  0,  0 }  };

      CU_ASSERT(matrix == expectedResult);
      if(matrix != expectedResult) std::cout << std::endl << matrix << std::endl;
      return;
   }


   // used as a function pointer to test being passed to NeedlemanWunsch
   int16_t nw_test_pmofp(char a, char b) {
      return a == b ? 1 : -1;
   }


   void nw_test(void)
   {
      std::string x = "GATTACA";
      std::string y = "GCATGCU";



      std::vector<std::string> expectedResult = {
         "G-ATTACA",
         "GCA-TGCU" 
      };

      std::vector<std::string> result(2, std::string());
   
      // Either use a back_inserter() or declare a string of length x.size() + y.size() for each as the result could be all indels

      // tests with a lambda
      NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), scoring::plus_minus_one);
      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
      result[0].erase(); result[1].erase();

      struct plus_minus_one_functor {
         int16_t operator()(char a, char b) { return a == b ? 1 : -1; };
      };

      // test with a functor
      plus_minus_one_functor pmf;

      NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), pmf);
      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
      result[0].erase(); result[1].erase();


      // test std::function
      auto pmf_function = std::function<int16_t(char, char)>(pmf);
      NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), pmf_function);
      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
      result[0].erase(); result[1].erase();


      // function pointer
      NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), nw_test_pmofp);
      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
      result[0].erase(); result[1].erase();


   }

#endif

} // namespace stringAlgorithms

#endif