/**
 * author: brando
 * date: 4/3/25
 */

#include "response.hpp"

Response::Response() {
}

Response::~Response() {
}

Response * Response::fromRequest(const Request * request) {
	return new Response;
}

