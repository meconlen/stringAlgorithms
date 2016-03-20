#include <config.h>

#include <string>

#include "needlemanwunsch.hpp"

using stringAlgorithms::NeedlemanWunsch;
using stringAlgorithms::displayDiff;
using stringAlgorithms::scoring::plus_minus_one;

int main(int argc, char *argv[])
{
   std::string x = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string y = "ATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string z = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCT"; 

   std::string s, t;

   plus_minus_one matchScoring;   

   NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(matchScoring));
   displayDiff(s, t);
   s.erase(); t.erase();

   NeedlemanWunsch(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(matchScoring));
   displayDiff(s, t);
   s.erase(); t.erase();

   NeedlemanWunsch(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(matchScoring));
   displayDiff(s, t);

   return 0;
}
