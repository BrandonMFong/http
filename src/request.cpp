/**
 * author: brando
 * date: 4/3/25
 */

#include "request.hpp"
#include <bflibcpp/bflibcpp.hpp>

#include <regex>

using namespace BF;

Request::Request(const Data * data) : _message(data == NULL ? 0 : (const char *) data->buffer(), data == NULL ? 0 : data->size()) {
}

Request::~Request() {
}

RequestMethod Request::method() const {
	std::regex methodRegex(R"((GET|POST|PUT|DELETE|HEAD|OPTIONS|CONNECT|TRACE|PATCH)\s+)");
	std::smatch match;
	if (!std::regex_search(this->_message, match, methodRegex)) {
		return kRequestMethodNone;
	}

	std::string matchStr = match[1].str();
	if (matchStr == "GET") {
		return kRequestMethodGet;
	} else if (matchStr == "POST") {
		return kRequestMethodPost;
	} else {
		return kRequestMethodNone;
	}
}

String Request::target() const {
	return "";
}
String Request::protocol() const {
	return "";
}

String Request::host() const {
	return "";
}

