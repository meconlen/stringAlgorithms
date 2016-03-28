#include <config.h>

#include <string>

#include "lcs.hpp"

using stringAlgorithms::longest_common_subsequence;
int main(int argc, char *argv[])
{
   std::string x = "XMJYAUZ";
   std::string y = "MZJAWXU";
   std::string s;

   // auto zero_one = [](const char &x, const char &y) -> int16_t { return x == y ? 1 : 0; };

   // NeedlemanWunsch(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s), std::back_inserter(t), zero_one, 0);
   // display_diff(s, t);
   // display_longest_common_subsequence(s, t);
   // s.erase(); t.erase();

   longest_common_subsequence(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(s));
   std::cout << "s = " << s << std::endl;

   return 0;
}
