/**
 * author: brando
 * date: 4/3/25
 */

#include "request.hpp"
#include "log.hpp"
#include <bflibcpp/bflibcpp.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

#include <regex>

using namespace BF;

Request::Request(const Data * data) : _message(data == NULL ? 0 : (const char *) data->buffer(), data == NULL ? 0 : data->size()) {
	LOG_WRITE("Request length = %ld", _message.size());
	LOG_WRITE("Request content = \n%s", _message.c_str());
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

String Request::targetPath() const {
	String target = this->target();
	char * target_copy = BFStringCopyString(target.c_str());
	if (!target_copy) {
		return target;
	}

	const char * del = "?";
	char * sub = strtok(target_copy, del);
	if (!sub) {
		return target;
	}

	String res = sub;
	BFFree(target_copy);

	return res;
}

HashMap<String, String> Request::targetQuery() const {
	HashMap<String, String> res;
	String target = this->target();
	char * target_copy = BFStringCopyString(target.c_str());
	if (!target_copy) {
		return res;
	}

	const char * del = "?";
	if (strtok(target_copy, del) == NULL) {
		return res;
	}

	char * sub = strtok(target_copy, del);
	if (!sub) {
		return res;
	}

	BFFree(target_copy);

	return res;
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

