/**
 * author: brando
 * date: 2/26/25
 */

#include "office.hpp"
#include "log.hpp"
#include "resource.hpp"
#include <cstdio>
#include <bflibcpp/bflibcpp.hpp>
#include <bfnet/bfnet.hpp>
#include <iostream>

extern "C" {
#include <bflibc/bflibc.h>
}

#define ARGUMENT_ROOT "-root"

using namespace BF::Net;
using namespace BF;
using namespace std;

LOG_INIT;

void help(const char * toolname) {
	printf("usage: %s %s <path>\n", toolname, ARGUMENT_ROOT);
	printf("\n");
	printf("Arguments:\n");
	printf("  %s <path>\tThis is the root folder where we will look for resources\n", ARGUMENT_ROOT);
	printf("\nCopyright © 2025 Brando. All rights reserved.\n");
}

void __NewConnection(Connection * sc) {
	LOG_DEBUG("new connection made");
}

int __ReadArguments(int argc, char * argv[]) {
	if (argc == 1) {
		help(argv[0]);
		return -1;
	}

	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], ARGUMENT_ROOT)) {
			if (!Resource::setRootFolder(argv[++i])) {
				LOG_ERROR("'%s' is not accepted as a root folder", argv[i]);
			}
		}
	}

	return 0;
}

int main(int argc, char * argv[]) {
	LOG_OPEN;

	if (__ReadArguments(argc, argv)) {
		return -1;
	}

	int error = 0;
	Log::SetCallback(__LogCallbackBFNet);

	Office::start();
	Socket * skt = Socket::create(SOCKET_MODE_SERVER, "0.0.0.0", 8080, &error);
	if (!error) {
		skt->setInStreamCallback(Office::envelopeReceive);
		skt->setNewConnectionCallback(__NewConnection);
		skt->setBufferSize(1024 * 1024 * 100);
		error = skt->start();
	}
	
	if (!error) {
		cout << "Press any key to stop...";
		cin.get();
		error = skt->stop();

		cout << "Stopped..." << endl;
	}

	BFRelease(skt);
	Office::stop();

	LOG_CLOSE;

	return error;
}

