#include <config.h>

#include <string>
#include <vector>

#include "needlemanwunsch.hpp"
#include "stringoutput.hpp"

using stringAlgorithms::NeedlemanWunsch;
using stringAlgorithms::display_diff;
using stringAlgorithms::scoring::plus_minus_one;

int main(int argc, char *argv[])
{
   std::vector<uint64_t>   x = { 1, 2, 3, 4, 5 };
   std::vector<uint64_t>   y = { 1, 2, 0, 5};

   std::vector<uint64_t> s, t;

   uint64_t delIndicator = -1;

   // lambda
   NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), plus_minus_one, -1, delIndicator);
   display_diff(s, t, delIndicator);

   return 0;
}