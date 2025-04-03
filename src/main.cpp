/**
 * author: brando
 * date: 2/26/25
 */

#include "office.hpp"
#include <cstdio>
#include <bflibcpp/bflibcpp.hpp>
#include <bfnet/bfnet.hpp>
#include <iostream>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF::Net;
using namespace BF;
using namespace std;

void __LogCallbackBFNet(const char * buf) { \
	cout << "bfnet: " << buf << endl;
}

void __NewConnection(Connection * sc) {
	cout << "new connection made" << endl;
}

int main() {
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
	}

	BFRelease(skt);
	Office::stop();

	return error;
}

