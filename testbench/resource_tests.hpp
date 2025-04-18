/**
 * author: Brando
 * date: 4/17/25
 */

#ifndef RESOURCE_TESTS_HPP
#define RESOURCE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <bflibcpp/bflibcpp.hpp>
#include "resource.hpp"
#include <libgen.h>

extern "C" {
#include <bflibc/bflibc.h>
#include <bftest/bftest.h>
}

using namespace BF;

BFTEST_UNIT_FUNC(test_resourceRootFolder, 2 << 10, {
	char tmp[PATH_MAX];
	URL url = __FILE__;
	strcpy(tmp, url.abspath());

	char * rootFolder = dirname(tmp);
	Resource::setRootFolder(rootFolder);

	BF_ASSERT(Resource::targetValid(url.abspath()));

	char * parent = dirname(rootFolder);
	URL parentURL(parent);
	BF_ASSERT(!Resource::targetValid(parentURL.abspath()));

	URL challenge(rootFolder);
	challenge.append("hello");
	challenge.append("..");
	challenge.append("..");
	BF_ASSERT(!Resource::targetValid(challenge.standardURL().abspath()));
})

BFTEST_COVERAGE_FUNC(resource_tests, {
	BFTEST_LAUNCH(test_resourceRootFolder);
})

#endif // RESOURCE_TESTS_HPP

