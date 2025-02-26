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

int main() {
	int error = 0;
	Socket * skt = Socket::create(SOCKET_MODE_SERVER, "127.0.0.1", 8080, &error);
	if (!error) {
		error = skt->start();
	}
	
	cout << "Press any key to stop...";
	cin.get();

	if (!error) {
		error = skt->stop();
	}

	BFRelease(skt);

	return error;
}

