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
using namespace BF;
using namespace std;

/**
 * holds all incoming envelopes from open socket
 *
 * all envelopes are retained, so these should be released
 * once popped from queue
 */
Atomic<Queue<Envelope *>> _incomingRequests;
BFThreadAsyncID _tidRequestQueue = NULL;
BFLock _queueSema;

void __LogCallbackBFNet(const char * buf) { \
	cout << "bfnet: " << buf << endl;
}

void __NewConnection(Connection * sc) {
	cout << "new connection made" << endl;
}

void __PacketReceive(Envelope * envelope) {
	_incomingRequests.lock();

	BFRetain(envelope);
	_incomingRequests.unsafeget().push(envelope);

	_incomingRequests.unlock();
	BFLockRelease(&_queueSema);
}

void _IncomingRequestsWorkerThread(void * in) {
	while (!BFThreadAsyncIsCanceled(_tidRequestQueue)) {
		if (_incomingRequests.get().empty()) { 
			BFLockWait(&_queueSema);
		} else {
			_incomingRequests.lock();

			// get first item from the queue
			Envelope * envelope = _incomingRequests.unsafeget().front();

			// pop off
			_incomingRequests.unsafeget().pop();
	
			cout << "envelope: " << (const char *) envelope->data()->buffer() << endl;

			// release memory
			BFRelease(envelope);

			_incomingRequests.unlock();
		}
	}

}

int main() {
	int error = 0;
	Log::SetCallback(__LogCallbackBFNet);

	BFLockCreate(&_queueSema);
	_tidRequestQueue = BFThreadAsync(_IncomingRequestsWorkerThread, NULL);

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

	BFThreadAsyncCancel(_tidRequestQueue);
	BFThreadAsyncWait(_tidRequestQueue);
	BFThreadAsyncDestroy(_tidRequestQueue);

	BFLockDestroy(&_queueSema);

	return error;
}

