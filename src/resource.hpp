/**
 * author: brando
 * date: 4/9/25
 *
 * handles resource requests from GET
 */

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <bflibcpp/data.hpp>
#include <bflibcpp/url.hpp>

namespace Resource {
	bool setRootFolder(const BF::String & rootFolder);
	const BF::String & getRootFolder();
	bool targetValid(const BF::URL & target);
}

#endif // RESOURCE_HPP

