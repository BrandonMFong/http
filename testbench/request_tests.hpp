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

BFTEST_UNIT_FUNC(test_simpleClientRequest, 2 << 8, {
	String get_str = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
	Data get_buf(get_str);
	String post_str = "POST /submit.php HTTP/1.1\r\nHost: another.com\r\nContent-Length: 10\r\n\r\ndata=value";
	Data post_buf(post_str);
	String head_str = "HEAD /static/image.png HTTP/1.0\r\n\r\n";
	Data head_buf(head_str);
	String invalid_str = "Invalid Request Line";
	Data invalid_buf(invalid_str);

	Request * req = NULL;
	
	req = new Request(&get_buf);
	BF_ASSERT(req->method() == "GET");
	BF_ASSERT(req->target() == "/index.html");
	BF_ASSERT(req->protocol() == "HTTP/1.1");
	BFRelease(req);

	req = new Request(&post_buf);
	BF_ASSERT(req->method() == "POST");
	BF_ASSERT(req->target() == "/submit.php");
	BF_ASSERT(req->protocol() == "HTTP/1.1");
	BFRelease(req);

	req = new Request(&head_buf);
	BF_ASSERT(req->method() == "HEAD");
	BF_ASSERT(req->target() == "/static/image.png");
	BF_ASSERT(req->protocol() == "HTTP/1.0");
	BFRelease(req);

	req = new Request(&invalid_buf);
	BF_ASSERT(req->method() == "");
	BF_ASSERT(req->target().empty());
	BF_ASSERT(req->protocol().empty());
	BFRelease(req);
})

BFTEST_UNIT_FUNC(test_requestTargetPathAndQuery, 1, {
	String str = "GET /assets/fonts/fontawesome-webfont.ttf?v=4.6.3 HTTP/1.1\r\n\
	Host: 10.0.0.82:8080\r\n\
	User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:137.0) Gecko/20100101 Firefox/137.0\r\n\
	Accept: application/font-woff2;q=1.0,application/font-woff;q=0.9,*/*;q=0.8\r\n\
	Accept-Language: en-US,en;q=0.5\r\n\
	Accept-Encoding: gzip, deflate\r\n\
	Connection: keep-alive\r\n\
	Referer: http://10.0.0.82:8080/assets/css/font-awesome.min.css\r\n\
	";
	Data buf(str);

	Request req(&buf);
	BF_ASSERT(req.targetPath() == "/assets/fonts/fontawesome-webfont.ttf");
})

BFTEST_COVERAGE_FUNC(request_tests, {
	BFTEST_LAUNCH(test_requestInit);
	BFTEST_LAUNCH(test_simpleClientRequest);
	BFTEST_LAUNCH(test_requestTargetPathAndQuery);
})

#endif // REQUEST_TESTS_HPP

