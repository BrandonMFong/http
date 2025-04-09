/**
 * author: brando
 * date: 4/9/25
 */

#include "resource.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;

String _rootFolder;

bool Resource::setRootFolder(const BF::String & rootFolder) {
	_rootFolder = rootFolder;
	return true;
}

Data * Resource::copyContentForTarget(const String & target) {
	Data * res = NULL;

	return res;
}

