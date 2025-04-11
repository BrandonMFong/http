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

	/**
	 * the http response in raw text we will send back
	 *
	 * caller owns data
	 */
	const BF::Data * createData() const;
private:
	Response();
	
	static void handleRequestGET(const Request * request, Response * response);
	void writeStatusLine(BF::String & content) const;

	BF::Data * _body;
	int _statusCode = 0;
};

#endif // RESPONSE_HPP

