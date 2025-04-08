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

String Request::method() const {
	std::regex methodRegex(R"((GET|POST|PUT|DELETE|HEAD|OPTIONS|CONNECT|TRACE|PATCH)\s+)");
	std::smatch match;

    if (std::regex_search(this->_message, match, methodRegex)) {
        return match[1].str(); // The second capturing group contains the target
    } else {
        return ""; // Or throw an exception if no target is found
    }
}

String Request::target() const {
	std::regex targetRegex(R"((?:GET|POST|PUT|DELETE|HEAD|OPTIONS|CONNECT|TRACE|PATCH)\s+([^\s]+)\s+HTTP/\d\.\d)");
    std::smatch match;

    if (std::regex_search(this->_message, match, targetRegex)) {
        return match[1].str(); // The second capturing group contains the target
    } else {
        return ""; // Or throw an exception if no target is found
    }
}

String Request::protocol() const {
    std::regex protocolRegex(R"((?:GET|POST|PUT|DELETE|HEAD|OPTIONS|CONNECT|TRACE|PATCH)\s+[^\s]+\s+(HTTP/\d\.\d))");
    std::smatch match;

    if (std::regex_search(this->_message, match, protocolRegex)) {
        return match[1].str(); // The second capturing group contains the protocol
    } else {
        return ""; // Or throw an exception if no protocol is found
    }
}

String Request::host() const {
	return "";
}

