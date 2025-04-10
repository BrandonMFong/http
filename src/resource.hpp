/**
 * author: brando
 * date: 4/9/25
 *
 * handles resource requests from GET
 */

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <bflibcpp/data.hpp>

namespace Resource {
	BF::Data * copyContentForTarget(const BF::String & target);
	bool setRootFolder(const BF::String & rootFolder);
	const BF::String & getRootFolder();
}

#endif // RESOURCE_HPP

