#include <config.h>

#include <cctype>
#include <string>

#include <unistd.h>

#include "hirschberg.hpp"
#include "stringoutput.hpp"

using stringAlgorithms::Hirschberg;
using stringAlgorithms::display_diff;
using stringAlgorithms::scoring::plus_minus_one;

int16_t plus_minus_one_p(const char &x, const char &y) {
   return x == y ? 1 : -1;
}

// interestingly, if you put this definition inside main() then it will be almost as fast as a functor
// at least with clang
struct plus_minus_one_functor {
   int16_t operator()(char a, char b) { return a == b ? 1 : -1; };
};

int main(int argc, char *argv[])
{

   int16_t  count = 1000;
   int      c;

   enum function_type { POINTER, FUNCTOR, LAMBDA, STD_FUNCTION };

   function_type f_type = LAMBDA;

   while((c = getopt(argc, argv, "pflsn:")) != -1) {
      switch(c) {
         case 'p':
            f_type = POINTER;
            break;
         case 'f':
            f_type = FUNCTOR;
            break;
         case 'l':
            f_type = LAMBDA;
            break;
         case 's':
            f_type = STD_FUNCTION;
            break;
         case 'n':
            count = atoi(optarg);
            break;
      }
   }

   std::string x = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string y = "ATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAAATTCCTGCTTACCGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTCACAGACAGCCTGAGACAGTTCTTACGGAAACACCCCAGGACACAATTGAATTAAACAGATTGAATTTAGAATCTTCCAA";
   std::string z = "GAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCTGAATTTATGCTTATAGTTTAAATCCTTTCCTCTGGTCTCCCTTTGAATCATTATGTGAAATAGGTGAAAAGCCAGATCCTGACCAAACATTTAAATTCACATCTTTACAGAACTTTAGCAACTGTCTGCCCAACTCTTGCACAACACAAGTACCTAATCATAGTTTATCT"; 

   plus_minus_one_functor pmf;

   auto pmf_function = std::function<int16_t(char, char)>(plus_minus_one);


   switch(f_type) {
      case LAMBDA : 
         for(int16_t i=0; i < count; i++) {
            std::string s, t;

            // plus_minus_one matchScoring;   

            Hirschberg(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one);
            s.erase(); t.erase();

            Hirschberg(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one);
            s.erase(); t.erase();

            Hirschberg(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one);
            s.erase(); t.erase();
         }
         break;
      case POINTER:
         for(int16_t i=0; i < count; i++) {
            std::string s, t;

            // plus_minus_one matchScoring;   

            Hirschberg(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one_p);
            s.erase(); t.erase();

            Hirschberg(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one_p);
            s.erase(); t.erase();

            Hirschberg(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one_p);
            s.erase(); t.erase();
         }
         break;
      case FUNCTOR:
        for(int16_t i=0; i < count; i++) {
            std::string s, t;

            // plus_minus_one matchScoring;   

            Hirschberg(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), pmf);
            s.erase(); t.erase();

            Hirschberg(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), pmf);
            s.erase(); t.erase();

            Hirschberg(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), pmf);
            s.erase(); t.erase();
         }
         break;
      case STD_FUNCTION:
        for(int16_t i=0; i < count; i++) {
            std::string s, t;

            // plus_minus_one matchScoring;   

            Hirschberg(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), pmf_function);
            s.erase(); t.erase();

            Hirschberg(x.begin(), x.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), pmf_function);
            s.erase(); t.erase();

            Hirschberg(y.begin(), y.end(), z.begin(), z.end(), std::back_inserter(s), std::back_inserter(t), pmf_function);
            s.erase(); t.erase();
         }
         break;   
   }

   return 0;
}