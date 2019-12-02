//#include "../utils/VectorUtils.h"
//
//#define BOOST_TEST_MODULE mytests
//#include <boost/test/included/unit_test.hpp>
//
////BOOST_AUTO_TEST_CASE(myTestCase)
////{
////  BOOST_TEST(VectorUtils::minValue(1, 2) == 1);
////  BOOST_TEST(true);
////}
#define BOOST_TEST_DYN_LINK
#include <boost/test/minimal.hpp>
#include "../utils/VectorUtils.h"

static void test_min()
{
	auto out = VectorUtils::minValue(1, 2);
	BOOST_REQUIRE(out == 1);
}

int test_main(int argc, char* argv[])
{
	test_min();
	return 0;
}