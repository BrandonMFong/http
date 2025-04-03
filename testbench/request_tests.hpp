/**
 * author: Brando
 * date: 4/3/25
 */

#ifndef REQUEST_TESTS_HPP
#define REQUEST_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <bflibcpp/bflibcpp.hpp>
#include "request.hpp"

extern "C" {
#include <bflibc/bflibc.h>
#include <bftest/bftest.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_requestInit, 1, {
	Data d;
	Request * req = new Request(&d);
	BFRelease(req);
})

BFTEST_COVERAGE_FUNC(request_tests, {
	BFTEST_LAUNCH(test_requestInit);
})

#endif // REQUEST_TESTS_HPP

