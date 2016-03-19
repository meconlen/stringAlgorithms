#include <config.h>

#include <iostream>
#include <string>

#include "needlemanwunsch.hpp"

using namespace stringAlgorithms;

void displayDiff(std::string a, std::string b) 
{
   auto a_cur = a.begin(), b_cur = b.begin();

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

   std::string a_out, b_out;

   while(a_cur != a.end() && b_cur != b.end()) {
      if(*a_cur == '-') {
         a_out.push_back(*a_cur);
         b_out.append(colorStrings[CYAN]);
         b_out.push_back(*b_cur);
         b_out.append(colorStrings[NORMAL]);
      } else if(*b_cur == '-') {
         b_out.push_back(*b_cur);
         a_out.append(colorStrings[CYAN]);
         a_out.push_back(*a_cur);
         a_out.append(colorStrings[NORMAL]);
      } else if(*a_cur != *b_cur) {
         a_out.append(colorStrings[RED]);
         a_out.push_back(*a_cur);
         a_out.append(colorStrings[NORMAL]);
         b_out.append(colorStrings[RED]);
         b_out.push_back(*b_cur);
         b_out.append(colorStrings[NORMAL]);
      } else {
         a_out.push_back(*a_cur);
         b_out.push_back(*b_cur);
      }
      a_cur++; b_cur++;
   }

   std::cout << "a = " << a_out << std::endl;
   std::cout << "b = " << b_out << std::endl;
   return;
}


int main(int argc, char *argv[])
{
   std::string x = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string y = "ATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string z = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCT"; 

   std::string s, t;

   scoring::plus_minus_one matchScoring;   

   NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(matchScoring));
   displayDiff(s, t);
   s = ""; t = "";

   NeedlemanWunsch(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(matchScoring));
   displayDiff(s, t);
   s = ""; t = "";

   NeedlemanWunsch(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), std::function<int16_t(char, char)>(matchScoring));
   displayDiff(s, t);

   return -0;
}
