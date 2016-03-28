#ifndef LCS_HPP
#define LCS_HPP

#include <iterator>

#include "hirschberg.hpp"

#ifdef HAVE_CUNIT_CUNIT_H
#include "stringoutput.hpp"
#include <CUnit/Basic.h>
#endif

namespace stringAlgorithms {

   // given two strings return the LCS
   
   template<typename I, typename BI>
   void longest_common_subsequence(I x_begin, I x_end, I y_begin, I y_end, BI z_back, 
      typename std::iterator_traits<I>::value_type deleted_value = '-')
   {
      typedef typename std::iterator_traits<I>::value_type I_type;

      std::vector<I_type>  s, t;

      auto zero_one = [](const char &x, const char &y) -> int16_t { return x == y ? 1 : 0; };
      Hirschberg(x_begin, x_end, y_begin, y_end, std::back_inserter(s), std::back_inserter(t), zero_one, 0);
      for(auto s_cur = s.begin(), t_cur = t.begin(); s_cur != s.end() && t_cur != t.end(); s_cur++, t_cur++) {
         if(*s_cur == *t_cur) *z_back = *s_cur;
      }
   }

#ifdef HAVE_CUNIT_CUNIT_H

   int init_lcs_suite(void)
   {
      return 0;
   }

   int clean_lcs_suite(void)
   {
      return 0;
   }

   void lcs_test(void)
   {
      std::string x = "XMJYAUZ";
      std::string y = "MZJAWXU";
      std::string s;
      std::string result = "MJAU";

      longest_common_subsequence(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s));

      CU_ASSERT(s == result);
      return;

   }
#endif // HAVE_CUNIT_CUNIT_H
} // stringAlgorithms 

#endif