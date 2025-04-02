/**
 * author: brando
 * date: 2/26/25
 */

#include <cstdio>
#include <bflibcpp/bflibcpp.hpp>
#include <bfnet/bfnet.hpp>
#include <iostream>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF::Net;
using namespace std;

void __LogCallbackBFNet(const char * buf) { \
	cout << "bfnet: " << buf << endl;
}

void __NewConnection(Connection * sc) {
	cout << "new connection made" << endl;
}

void __PacketReceive(Envelope * envelope) {
	cout << "envelope: " << (const char *) envelope->data()->buffer() << endl;
}

int main() {
	int error = 0;
	Log::SetCallback(__LogCallbackBFNet);
	Socket * skt = Socket::create(SOCKET_MODE_SERVER, "0.0.0.0", 8080, &error);
	if (!error) {
		skt->setInStreamCallback(__PacketReceive);
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

	return error;
}

