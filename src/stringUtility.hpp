#ifndef STRINGUTILITY_HPP
#define STRINGUTILITY_HPP

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


} // stringAlgorithms

#endif // STRINGUTILITY_HPP