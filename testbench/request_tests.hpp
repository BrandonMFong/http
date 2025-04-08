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

BFTEST_UNIT_FUNC(test_simpleClientRequest, 2 << 10, {
	String get_str = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
	Data get_buf(get_str);
	String post_str = "POST /submit.php HTTP/1.1\r\nHost: another.com\r\nContent-Length: 10\r\n\r\ndata=value";
	Data post_buf(post_str);
	String invalid_str = "Invalid Request Line";
	Data invalid_buf(invalid_str);

	Request * req = NULL;
	
	req = new Request(&get_buf);
	BF_ASSERT(req->method() == kRequestMethodGet);
	BFRelease(req);

	req = new Request(&post_buf);
	BF_ASSERT(req->method() == kRequestMethodPost);
	BFRelease(req);

	req = new Request(&invalid_buf);
	BF_ASSERT(req->method() == kRequestMethodNone);
	BFRelease(req);
})

BFTEST_COVERAGE_FUNC(request_tests, {
	BFTEST_LAUNCH(test_requestInit);
	BFTEST_LAUNCH(test_simpleClientRequest);

})

#endif // REQUEST_TESTS_HPP

