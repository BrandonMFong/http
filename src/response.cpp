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

#define HTTP_DATE_BUFSIZE 128

using namespace BF;

Response::Response() { }

Response::~Response() {
	BFRelease(this->_content);
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
		response->_content = Resource::copyContentForTarget(request->target());
		response->_contentType = "text/html; charset=utf-8";
	} else {
		response->_statusCode = 404;
		response->_content = new Data("<!DOCTYPE html><html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>Sorry, the page you are looking for does not exist.</p></body></html>");
		response->_contentType = "text/html; charset=utf-8";
	}
}

const Data * Response::createData() const {
	String content;
	this->writeStatusLine(content);
	this->writeHeader(content);

	return NULL;
}

void Response::writeStatusLine(String & content) const {
	if (this->_statusCode == 404) {
		content.append("HTTP/1.1 %d Not Found\n", this->_statusCode);
	} else {
		content.append("HTTP/1.1 %d Ok\n", this->_statusCode);
	}
}

void __ResponseGenerateDateString(char * buf) {
	if (!buf) return;

	time_t now = time(NULL);
	if (now == (time_t)(-1)) {
		return;
    }

	struct tm timeinfo_gmt;
	if (gmtime_r(&now, &timeinfo_gmt) == NULL) {
		return;
	}

	strftime(
		buf,
		HTTP_DATE_BUFSIZE,
		"%a, %d %b %Y %H:%M:%S GMT",
		&timeinfo_gmt);
}

void Response::writeHeader(String & content) const {
	content.append("Server: brando's http server\n");

	char buf[HTTP_DATE_BUFSIZE];
	__ResponseGenerateDateString(buf);
	content.append("Date: %s", buf);

	content.append("Content-Length: %d\n", this->_content ? this->_content->size() : 0);

	content.append("Content-Type: %s\n", this->_contentType.c_str());
}

