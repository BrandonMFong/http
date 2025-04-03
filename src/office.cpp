/**
 * author: brando
 * date: 4/3/25
 */

#include "office.hpp"
#include <cstdio>
#include <bflibcpp/bflibcpp.hpp>
#include <bfnet/bfnet.hpp>
#include <iostream>

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

void Office::envelopeReceive(Envelope * envelope) {
	_incomingRequests.lock();

	BFRetain(envelope);
	_incomingRequests.unsafeget().push(envelope);

	_incomingRequests.unlock();
	BFLockRelease(&_queueSema);
}

void __IncomingRequestsWorkerThread(void * in) {
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

void Office::start() {
	BFLockCreate(&_queueSema);
	_tidRequestQueue = BFThreadAsync(__IncomingRequestsWorkerThread, NULL);
}

void Office::stop() {
	BFThreadAsyncCancel(_tidRequestQueue);
	BFThreadAsyncWait(_tidRequestQueue);
	BFThreadAsyncDestroy(_tidRequestQueue);

	BFLockDestroy(&_queueSema);
}

