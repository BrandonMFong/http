/**
 * author: brando
 * date: 4/3/25
 */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <bflibcpp/object.hpp>
#include <bflibcpp/data.hpp>
#include "request.hpp"

class Response : public BF::Object {
public:
	static Response * fromRequest(const Request *);
	virtual ~Response();
private:
	Response();

private:
	BF::Data _message;
};

#endif // RESPONSE_HPP

