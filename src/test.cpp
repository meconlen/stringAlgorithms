#include <config.h>

#include <iostream>

#include "needlemanwunsch.hpp"
#include "hirschberg.hpp"
#ifdef HAVE_CUNIT_CUNIT_H
   #include <CUnit/Basic.h>
#endif

using namespace stringAlgorithms;

int main(int argc, char *argv[])
{
#ifdef HAVE_CUNIT_CUNIT_H
   CU_pSuite   nw_suite = nullptr,
               hirschberg_suite = nullptr;

   if(CUE_SUCCESS != CU_initialize_registry()) goto error1;

   if((nw_suite = CU_add_suite("Needleman-Wunsch Suite", init_nw_suite, clean_nw_suite)) == nullptr) goto error1;

   if((CU_add_test(nw_suite, "NeedleMan-Wunsch score matrix", nwScoreMatrix_test)) == nullptr) goto error1;
   if((CU_add_test(nw_suite, "Needleman-Wunsch", nw_test)) == nullptr) goto error1;

   if((hirschberg_suite = CU_add_suite("Hirschberg Suite", init_hirschberg_suite, clean_hirschberg_suite)) == nullptr) goto error1;
   if((CU_add_test(hirschberg_suite, "NeedleMan-Wunsch score row", hirschberg_nwScore_test)) == nullptr) goto error1;
   if((CU_add_test(hirschberg_suite, "Hirschberg", hirschberg_test)) == nullptr) goto error1;
   if((CU_add_test(hirschberg_suite, "Hirschberg 2", hirschberg_2_test)) == nullptr) goto error1;


   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
#else
   std::cout << "CUnit not available. Cannot run tests" << std::endl;
#endif
	return 0;
#ifdef HAVE_CUNIT_CUNIT_H
error1:
   CU_cleanup_registry();
#endif
   std::cerr << "Error setting up the tests via CUnit" << std::endl;
   return -1;
}
