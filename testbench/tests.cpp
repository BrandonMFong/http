/**
 * author: brando
 * date: 4/3/25
 */

#include "request_tests.hpp"
#include "resource_tests.hpp"
#include "log.hpp"

LOG_INIT;
BFTEST_SUITE_FUNC({
	BFTEST_SUITE_LAUNCH(request_tests);
	BFTEST_SUITE_LAUNCH(resource_tests);
})

