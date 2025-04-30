/**
 * author: brando
 * date: 4/3/25
 */

#include "response.hpp"
#include "resource.hpp"
#include "log.hpp"
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

String __ResponseTargetGetContentType(URL & target) {
	if (!strcmp(target.extension(), "html")) {
		return "text/html";
	} else if (!strcmp(target.extension(), "js")) {
		return "text/javascript";
	} else if (!strcmp(target.extension(), "css")) {
		return "text/css";
	} else if (!strcmp(target.extension(), "ico")) {
		return "image/x-icon";
	} else if (!strcmp(target.extension(), "jpg") || !strcmp(target.extension(), "jpeg")
			|| !strcmp(target.extension(), "jfif") || !strcmp(target.extension(), "pjpeg")
			|| !strcmp(target.extension(), "pjp")) {
		return "image/jpeg";
	} else if (!strcmp(target.extension(), "pdf")) {
		return "application/pdf";
	} else {
		return "text/plain";
	}
}

void Response::handleRequestGET(const Request * request, Response * response) {
	if (!request || !response) return;
	
	String target = request->targetPath();

	URL targetURL(Resource::getRootFolder());
	targetURL.append(target);
	
	if (BFFileSystemPathIsDirectory(targetURL.abspath())) {
		targetURL.append("index.html");
	}
	
	if (!Resource::targetValid(targetURL)) {
		response->_statusCode = 403;
		response->_content = new Data("HTTP/1.1 403 Forbidden");
		response->_contentType = "text/plain";

	} else if (BFFileSystemPathIsFile(targetURL.abspath())) {
		response->_content = Data::fromFile(targetURL);
		response->_contentType = __ResponseTargetGetContentType(targetURL);
	} else {
		response->_statusCode = 404;
		response->_content = new Data("404 Not Found");
		response->_contentType = "text/plain";
	}
}

const Data * Response::createData() const {
	String content;
	this->writeStatusLine(content);
	this->writeHeader(content);

	content.append("\r\n");

	if (!this->_content) {
		LOG_DEBUG("%s:%d - null content", __func__, __LINE__);
	} else {
		content.append(*this->_content);
	}

	return new Data(content);
}

void Response::writeStatusLine(String & content) const {
	if (this->_statusCode == 404) {
		content.append("HTTP/1.1 %d Not Found\r\n", this->_statusCode);
	} else {
		content.append("HTTP/1.1 %d Ok\r\n", this->_statusCode);
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
	content.append("Server: brando's http server\r\n");

	char buf[HTTP_DATE_BUFSIZE];
	__ResponseGenerateDateString(buf);
	content.append("Date: %s\r\n", buf);

	content.append("Content-Length: %d\r\n", this->_content ? this->_content->size() : 0);

	content.append("Content-Type: %s\r\n", this->_contentType.c_str());
}

