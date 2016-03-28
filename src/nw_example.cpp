#include <config.h>

#include <string>

#include "needlemanwunsch.hpp"
#include "stringoutput.hpp"

using stringAlgorithms::NeedlemanWunsch;
using stringAlgorithms::display_diff;

int main(int argc, char *argv[])
{
   std::string x = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string y = "ATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string z = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCT"; 

   std::string s, t;

   auto plus_minus_one_lambda = [](const char &x, const char &y) -> int16_t { return x == y ? 1 : -1; };

   struct plus_minus_one_functor {
      int16_t operator()(char a, char b) { return a == b ? 1 : -1; };
   };

   plus_minus_one_functor pmf;

   // lambda
   NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one_lambda);
   display_diff(s, t);
   s.erase(); t.erase();

   // functor
   NeedlemanWunsch(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), pmf);
   display_diff(s, t);
   s.erase(); t.erase();

   // std::function
   NeedlemanWunsch(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(pmf));
   display_diff(s, t);

   return 0;
}
