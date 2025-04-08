/**
 * author: brando
 * date: 4/3/25
 */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <bflibcpp/object.hpp>
#include <bflibcpp/data.hpp>
#include <string>

typedef enum {
	kRequestMethodNone = 0,
	kRequestMethodGet,
	kRequestMethodPost,
} RequestMethod;

class Request : public BF::Object {
public:
	Request(const BF::Data * data);
	virtual ~Request();

	BF::String method() const;
	BF::String target() const;
	BF::String protocol() const;

	BF::String host() const;

private:
	std::string _message;
};

#endif // REQUEST_HPP

