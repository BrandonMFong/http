/**
 * author: brando * date: 2/26/25
 */

#include "office.hpp"
#include "log.hpp"
#include "resource.hpp"
#include <cstdio>
#include <bflibcpp/bflibcpp.hpp>
#include <bfnet/bfnet.hpp>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" {
#include <bflibc/bflibc.h>
}

#define ARGUMENT_ROOT "-root"
#define ARGUMENT_PORT "-port"
#define ARGUMENT_VERSION "--version"

#define VERSION_STRING "0.1"

using namespace BF::Net;
using namespace BF;
using namespace std;

LOG_INIT;

uint16_t _port = 8080;
BFLock _appRunSema;
bool _showVersion = false;

void help(const char * toolname) {
	printf("usage: %s <args>\n", toolname);
	printf("\n");
	printf("Arguments:\n");
	printf("  %s <path>\tThis is the root folder where we will look for resources\n", ARGUMENT_ROOT);
	printf("  %s <num>\tSpecifies listening port, default is %d\n", ARGUMENT_PORT, _port);
	printf("  %s \tShows version\n", ARGUMENT_VERSION);
	printf("\nCopyright © 2025 Brando. All rights reserved.\n");
}

void __NewConnection(Connection * sc) {
	LOG_DEBUG("new connection made");
}

void __ShowVersion() {
	printf("%s\n", VERSION_STRING);
}

int __ReadArguments(int argc, char * argv[]) {
	if (argc == 1) {
		help(argv[0]);
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], ARGUMENT_ROOT)) {
			if (++i < argc && !Resource::setRootFolder(argv[i])) {
				LOG_ERROR("'%s' is not accepted as a root folder", argv[i]);
			}
		} else if (!strcmp(argv[i], ARGUMENT_PORT)) {
			if (++i < argc) {
				_port = atoi(argv[i]);
			}
		} else if (!strcmp(argv[i], ARGUMENT_VERSION)) {
			_showVersion = true;
		}
	}

	return 0;
}

void __HandleSignal(int signum) {
	BFLockRelease(&_appRunSema);
}

int main(int argc, char * argv[]) {
	LOG_OPEN;
	BFDefer([&](){
		LOG_CLOSE;
	});

	if (BFLockCreate(&_appRunSema)) {
		LOG_ERROR("couldn't create semaphore");
		return -1;
	}
	BFDefer([&](){
		BFLockDestroy(&_appRunSema);
	});

	if (__ReadArguments(argc, argv)) {
		return -1;
	}

	if (_showVersion) {
		__ShowVersion();
		return 0;
	}

	int error = 0;
	Log::SetCallback(__LogCallbackBFNet);

	Office::start();
	BFDefer([](){
		Office::stop();
	});

	const char * ipaddr = "0.0.0.0";
	LOG_WRITE("creating socket at %s:%u", ipaddr, _port);
	Socket * skt = Socket::create(SOCKET_MODE_SERVER, ipaddr, _port, &error);
	BFDefer([&](){
		BFRelease(skt);
	});
	if (error) {
		LOG_ERROR("couldn't create socket, error=%d", error);
		return error;
	}
	
	skt->setInStreamCallback(Office::envelopeReceive);
	skt->setNewConnectionCallback(__NewConnection);
	skt->setBufferSize(1024 * 1024 * 100);
	if (skt->start()) {
		LOG_ERROR("couldn't start listening on socket");
		return -1;
	}
	BFDefer([&](){
		skt->stop();
	});

	signal(SIGINT, __HandleSignal);  // For Ctrl+C
    signal(SIGTERM, __HandleSignal); // For 'kill' command
    signal(SIGHUP, __HandleSignal);  // For terminal hangup
	
	BFLockWait(&_appRunSema);

	return 0;
}

