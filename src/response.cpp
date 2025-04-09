/**
 * author: brando
 * date: 4/3/25
 */

#include "response.hpp"
#include "resource.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;

Response::Response() {
}

Response::~Response() {
	BFRelease(this->_body);
}

Response * Response::fromRequest(const Request * request) {
	if (!request) return NULL;

	String s = "HTTP/1.1 200 OK\n Content-Type: text/html\n <html><body>Hello World</body></html>";

	Response * res = new Response;
	if (!res) return NULL;

	if (request->method() == "GET") {
		String target = request->target();
		res->_body = Resource::copyContentForTarget(target);
	}

	return res;
}

const Data * Response::data() const {
	return NULL;
}

