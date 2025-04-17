/**
 * author: brando
 * date: 4/9/25
 */

#include "resource.hpp"
#include "log.hpp"
#include <bflibcpp/bflibcpp.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

#ifdef LINUX
#include <linux/limits.h>
#endif
#include <limits.h>

using namespace BF;

String _rootFolder;

const BF::String & Resource::getRootFolder() {
	return _rootFolder;
}

bool Resource::setRootFolder(const String & rootFolder) {
	URL url(rootFolder);
	if (!BFFileSystemPathExists(url.abspath())) {
		return false;
	}
	_rootFolder = rootFolder;
	return true;
}

