/**
 * author: brando
 * date: 4/3/25
 */

#include "response.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;

Response::Response() {
}

Response::~Response() {
}

Response * Response::fromRequest(const Request * request) {
	String s = "HTTP/1.1 200 OK\n Content-Type: text/html\n <html><body>Hello World</body></html>";

	Response * res = new Response;
	if (!res) return NULL;

	res->_message.alloc(s.length(), (const unsigned char *) s.c_str());
	return res;
}

const Data * Response::data() const {
	return &this->_message;
}

