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

// component: 0=path, 1=query
String __RequestTargetParse(String & target, int component) {
	int error = component != 0 && component != 1 ? 1 : 0;
	String res;
	char * target_copy = NULL;

	if (!error) {
		target_copy = target.cStringCopy();
		if (!target_copy) {
			res = target;
			error = 1;
		}
	}

	const char * del = "?";
	char * sub = NULL;
	int i = 0;
	while (!error && component >= 0) {
		sub = strtok(i == 0 ? target_copy : NULL, del);
		if (!sub) {
			res = target;
			error = 1;
		}

		component--;
		i++;
	}

	if (!error) {
		res = sub;
	}

	BFFree(target_copy);

	return res;

}

String Request::targetPath() const {
	String target = this->target();
	return __RequestTargetParse(target, 0);
}

int __RequestQueryPairParse(HashMap<String, String> & map, const char * pair) {
	if (!pair) return 1;

	char * pair_copy = BFStringCopyString(pair);
	const char * del = "=";
	String key = strtok(pair_copy, del);
	String value = strtok(NULL, del);

	map.insert(key, value);

	BFFree(pair_copy);

	return 0;
}

HashMap<String, String> Request::targetQuery() const {
	HashMap<String, String> res;
	String target = this->target();
	String queryString = __RequestTargetParse(target, 1);
	char * query_copy = queryString.cStringCopy();
	
	const char * del = "&";
	char * sub = NULL;
	int i = 0;
	while ((sub = strtok(i++ == 0 ? query_copy : NULL, del)) != NULL) {
		__RequestQueryPairParse(res, sub);
	}

	BFFree(query_copy);

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

