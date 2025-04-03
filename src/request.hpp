/**
 * author: brando
 * date: 4/3/25
 */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <bflibcpp/object.hpp>
#include <bflibcpp/data.hpp>

class Request : public BF::Object {
public:
	Request(const BF::Data * data);
	virtual ~Request();

private:
	BF::Data _message;
};

#endif // REQUEST_HPP

