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

Data * Resource::copyContentForFile(const String & file) {
	URL url(_rootFolder);

	LOG_DEBUG("%s:%d - url-> '%s'", __func__, __LINE__, url.abspath());


	if (!BFFileSystemPathExists(url.abspath())) {
		LOG_DEBUG("%s:%d - url above does not exist", __func__, __LINE__);
		return NULL;
	}

	return Data::fromFile(url);
}

