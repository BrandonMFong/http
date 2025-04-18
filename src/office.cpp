/**
 * author: brando
 * date: 4/3/25
 */

#include "office.hpp"
#include "request.hpp"
#include "response.hpp"
#include "log.hpp"
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
		if (_incomingRequests.get<bool>([] (Queue<Envelope *> & q) {
			return q.empty();
		})) { 
			BFLockWait(&_queueSema);
		} else {
			_incomingRequests.lock();
			Envelope * envelope = _incomingRequests.unsafeget().front();
			_incomingRequests.unsafeget().pop();
			_incomingRequests.unlock();
	
			if (envelope->data()->size() == 0) {
				continue;
			}

			Request * req = new Request(envelope->data());

			Response * resp = Response::fromRequest(req);
			if (resp) {
				const Data * respData = resp->createData();	
				envelope->connection()->queueData(respData);

				BFRelease(respData);
			}

			BFRelease(resp);
			BFRelease(req);
			BFRelease(envelope);
		}
	}
}

void Office::start() {
	BFLockCreate(&_queueSema);
	_tidRequestQueue = BFThreadAsync(__IncomingRequestsWorkerThread, NULL);
}

void Office::stop() {
	BFThreadAsyncCancel(_tidRequestQueue);
	BFLockRelease(&_queueSema);
	BFThreadAsyncWait(_tidRequestQueue);
	BFThreadAsyncDestroy(_tidRequestQueue);

	BFLockDestroy(&_queueSema);
}

