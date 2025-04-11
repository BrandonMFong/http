/**
 * author: brando
 * date: 4/3/25
 */

#include "response.hpp"
#include "resource.hpp"
#include <bflibcpp/bflibcpp.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

Response::Response() { }

Response::~Response() {
	BFRelease(this->_body);
}

Response * Response::fromRequest(const Request * request) {
	if (!request) return NULL;

	Response * res = new Response;
	if (!res) return NULL;
	res->_statusCode = 200;

	if (request->method() == "GET") {
		Response::handleRequestGET(request, res);
	}

	return res;
}

void Response::handleRequestGET(const Request * request, Response * response) {
	if (!request || !response) return;
	
	String target = request->target();
	URL url(Resource::getRootFolder());
	url.append(target);
	if (BFFileSystemPathIsFile(url.abspath())) {
		response->_body = Resource::copyContentForTarget(request->target());
	} else {
		response->_statusCode = 404;
	}
}

const Data * Response::createData() const {
	String content;
	this->writeStatusLine(content);
	if (this->_statusCode == 404) {
	}
	return NULL;
}

void Response::writeStatusLine(String & content) const {

}

