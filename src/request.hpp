/**
 * author: brando
 * date: 4/3/25
 */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <bflibcpp/object.hpp>
#include <bflibcpp/hashmap.hpp>
#include <bflibcpp/data.hpp>
#include <string>

#ifdef LINUX
#include <linux/limits.h>
#endif
#include <limits.h>

typedef enum {
	kRequestMethodNone = 0,
	kRequestMethodGet,
	kRequestMethodPost,
} RequestMethod;

class Request : public BF::Object {
public:
	Request(const BF::Data * data);
	virtual ~Request();

	const BF::String & method() const;
	const BF::String & target() const;
	const BF::String & protocol() const;

	const BF::HashMap<BF::String, BF::String> & header() const;
	const BF::String & body() const;

	// returns path without query string
	BF::String targetPath() const;

	// returns query data
	BF::HashMap<BF::String, BF::String> targetQuery() const;

private:
	void parse();

	std::string _message;

	// status line
	BF::String _method;
	BF::String _target;
	BF::String _protocol;

	BF::HashMap<BF::String, BF::String> _header;

	BF::String _body;
};

#endif // REQUEST_HPP

