#ifndef NEEDLEMANWUNSCH_HPP
#define NEEDLEMANWUNSCH_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

#ifdef HAVE_CUNIT_CUNIT_H
#include <CUnit/Basic.h>
#endif

namespace stringAlgorithms {

   namespace {

      // useful for printing vectors  
      template<typename T>
      std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
      {
         os << "{ ";
         for(auto i = v.begin(); i != v.end(); i++) {
            os << *i; 
            if(i+1 != v.end()) {
               os << ", ";
            } else {
               os << " }";
            }
         }
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

   } // local namespace

   // Compute the Needleman-Wunsch score matrix on a type 
   // look at use of std::function on large strings sometime
   template<typename P = int16_t, typename I>
   std::vector<std::vector<P>> nw_score_matrix(I xb, I xe, I yb, I ye, 
      std::function<P (typename std::iterator_traits<I>::value_type, typename std::iterator_traits<I>::value_type)> &&score_function, P ID = -1)
   {

      auto x_size = std::distance(xb, xe);
      auto y_size = std::distance(yb, ye);

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

   namespace scoring {
      struct plus_minus_one {
         template<typename R = int16_t, typename T>
         R operator()(const T &x, const T &y) { return x == y ? 1 : -1; }
      };
   }

   template<typename P = int16_t, typename I, typename BI>
   void NeedlemanWunsch(I xb, I xe, I yb, I ye, BI wb, BI zb,  
      std::function<P (typename std::iterator_traits<I>::value_type, typename std::iterator_traits<I>::value_type)> &&score_function, 
      const typename std::iterator_traits<I>::value_type &&delChar = '-', P ID = -1)
   {
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

      scoring::plus_minus_one matchScoring;

      std::vector<std::vector<int16_t>> matrix = nw_score_matrix(x.begin(), x.end(), y.begin(), y.end(), std::function<int16_t(char, char)>(matchScoring));
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

   void nw_test(void)
   {
      std::string x = "GATTACA";
      std::string y = "GCATGCU";



      std::vector<std::string> expectedResult = {
         "G-ATTACA",
         "GCA-TGCU" 
      };

      std::vector<std::string> result(2, std::string());
   
      scoring::plus_minus_one matchScoring;

      NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(result[0]), std::back_inserter(result[1]), std::function<int16_t(char, char)>(matchScoring));

      CU_ASSERT(result == expectedResult);
      if(result != expectedResult) {
         std::cout << std::endl;
         std::cout << "result[0] = " << result[0] << std::endl;
         std::cout << "result[1] = " << result[1] << std::endl;
      }
   }

#endif

} // namespace stringAlgorithms

#endif