#ifndef NEEDLEMANWUNSCH_HPP
#define NEEDLEMANWUNSCH_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


#ifdef HAVE_CUNIT_CUNIT_H
#include "stringoutput.hpp"
#include <CUnit/Basic.h>
#endif

#include "stringUtility.hpp"

namespace stringAlgorithms {

   // Compute the Needleman-Wunsch score matrix on a type 
   // look at use of std::function on large strings sometime
   
   template<typename I, typename F>
   std::vector<std::vector< typename PP<F, I>::type >> 
   nw_score_matrix(I x_begin, I x_end, I y_begin, I y_end, F &&score_function, typename PP<F, I>::type ID = -1)
   {

      typedef typename PP<F,I>::type P;
      
      auto x_size = std::distance(x_begin, x_end);
      auto y_size = std::distance(y_begin, y_end);

      std::vector<std::vector<P>> score(x_size + 1, std::vector<P>(y_size + 1, 0));

      score[0][0] = 0;
      for(auto j = 1; j <= y_size; j++) score[0][j] = score[0][j-1] + ID; 

      auto x_cur = x_begin;
      for(auto i = 1; i <= x_size; i++) {
         score[i][0] = score[i-1][0] + ID;
         auto y_cur = y_begin;
         for(auto j = 1; j <= y_size; j++) {
            P score_sub = score[i-1][j-1] + score_function(*x_cur, *y_cur);
            P score_del = score[i-1][j] + ID;
            P score_ins = score[i][j-1] + ID;
            score[i][j] = std::max({score_sub, score_del, score_ins});
            y_cur++;
         }
         x_cur++;
      }

      return score;
   }

   template<typename I, typename BI, typename F>
   void NeedlemanWunsch(I x_begin, I x_end, I y_begin, I y_end, BI w_back, BI z_back, F &&score_function, typename PP<F, I>::type ID = -1,
      const typename std::iterator_traits<I>::value_type deleted_value = '-')
   {
      typedef typename PP<F,I>::type P;
      typedef typename std::iterator_traits<I>::value_type I_type;

      std::vector<std::vector<I_type>> result(2, std::vector<I_type>());

      std::vector<std::vector<P>> score = nw_score_matrix(x_begin, x_end, y_begin, y_end, std::move(score_function), ID);

      int_fast64_t   i = std::distance(x_begin, x_end);
      int_fast64_t   j = std::distance(y_begin, y_end);

      auto x_cur = x_end - 1;
      auto y_cur = y_end - 1;

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
            result[1].push_back(deleted_value);
            i--;     
            x_cur--;   
         }
         else {
            result[0].push_back(deleted_value);
            result[1].push_back(*y_cur);
            j--;
            y_cur--;      
         }
      }
      for(auto &i : result) std::reverse(i.begin(), i.end()); 

      std::copy(result[0].begin(), result[0].end(), w_back);
      std::copy(result[1].begin(), result[1].end(), z_back);

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