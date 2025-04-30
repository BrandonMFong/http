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

#include <sstream>

using namespace BF;

Request::Request(const Data * data) : 
	_message(data == NULL ? 0 : (const char *) data->buffer(), data == NULL ? 0 : data->size()) {
	LOG_DEBUG("Request length = %ld", _message.size());
	LOG_WRITE("Request content = \n%s", _message.c_str());

	this->parse();
}

Request::~Request() { }

void __RequestParseStatusLine(std::string & in, String & method, String & target, String & protocol) {
	std::stringstream ss(in);
	std::string buf;
	char del = ' ';

	if (getline(ss, buf, del)) {
		method = buf;
	}

	if (getline(ss, buf, del)) {
		target = buf;
	}

	if (getline(ss, buf, del)) {
		protocol = buf;
	}
}

void __RequestParseHeader(std::string & in, HashMap<String, String> & header) {
	std::string del = ": ";
	auto pos = in.find(del);
	std::string key, value;

	if (pos == std::string::npos) {
		return;
	}
	
	key = in.substr(0, pos);
	in.erase(0, pos + del.length());
	pos = in.find(del);
	
	value = in.substr(0, pos);

	header.insert(key, value);
}

void Request::parse() {
	if (this->_message.empty()) {
		return;
	}

	std::stringstream ss(this->_message);
	std::string buf;
	char del = '\n';
	int statusLineParsed = false;
	int headerParsed = false;
	while (getline(ss, buf, del)) {
		buf.erase(
			std::remove_if(
				buf.begin(),
				buf.end(),
				[](unsigned char c) {
					return c == '\t' || c == '\n' || c == '\r'; 
				}
			),
			buf.end()
		);

		if (!statusLineParsed) {
			__RequestParseStatusLine(buf, this->_method, this->_target, this->_protocol);
			statusLineParsed = true;
		} else if (!headerParsed) {
			if (buf.empty()) {
				headerParsed = true;
			} else {
				__RequestParseHeader(buf, this->_header);
			}
		} else { // read the body
			this->_body.append(buf);
		}
	}
}

const String & Request::method() const {
	return this->_method;
}

const String & Request::target() const {
	return this->_target;
}

const String & Request::protocol() const {
	return this->_protocol;
}

const HashMap<String, String> & Request::header() const {
	return this->_header;
}

const String & Request::body() const {
	return this->_body;
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

