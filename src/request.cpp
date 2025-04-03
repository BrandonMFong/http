/**
 * author: brando
 * date: 4/3/25
 */

#include "request.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;

Request::Request(const Data * data) {
	this->_message.alloc(data->size(), (const unsigned char *) data->buffer());
}

Request::~Request() {
}

