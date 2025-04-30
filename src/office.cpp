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
#include <unistd.h>

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

/**
 * requests will come in asynchronously from clients, we will queue 
 * each one and have our worker threads pick off the queue
 *
 * we have n worker threads, working concurrently on every available request
 *
 * i don't see a performance boost on the client side between 2 and 4 threads
 */
const unsigned char numWorkerThreads = 2;
BFThreadAsyncID _tidRequestQueue[numWorkerThreads];

BFLock _queueSema;

void Office::envelopeReceive(Envelope * envelope) {
	int threshold = 25;
	while (_incomingRequests.get<int>([=] (auto & q) {
		return q.size();
	}) >= threshold) {
		usleep(500);
	}

	_incomingRequests.get([=] (Queue<Envelope *> & q) {
		BFRetain(envelope);
		q.push(envelope);
	});
	BFLockRelease(&_queueSema);
}

void __IncomingRequestsWorkerThread(void * in) {
	const BFThreadAsyncID tid = BFThreadAsyncGetID();
	while (!BFThreadAsyncIsCanceled(tid)) {
		_incomingRequests.lock();
		if (_incomingRequests.unsafeget().empty()) {
			_incomingRequests.unlock();
			BFLockWait(&_queueSema);
		} else {
			Envelope * envelope = _incomingRequests.unsafeget().front();
			_incomingRequests.unsafeget().pop();
			_incomingRequests.unlock();
	
			if (envelope->data()->size() == 0) {
				BFRelease(envelope);
				continue;
			}

			Request * req = new Request(envelope->data());

			Response * resp = Response::fromRequest(req);
			if (resp) {
				const Data * respData = resp->createData();	
				envelope->connection()->queueData(respData);

				BFRelease(respData);
			}

			envelope->connection()->closeConnection();

			BFRelease(resp);
			BFRelease(req);
			BFRelease(envelope);
		}
	}
}

void Office::start() {
	BFLockCreate(&_queueSema);

	for (int i = 0; i < numWorkerThreads; i++) {
		_tidRequestQueue[i] = BFThreadAsync(__IncomingRequestsWorkerThread, NULL);
	}
}

void Office::stop() {
	for (int i = 0; i < numWorkerThreads; i++) {
		BFLockRelease(&_queueSema);
		BFThreadAsyncCancel(_tidRequestQueue[i]);
	}
	
	for (int i = 0; i < numWorkerThreads; i++) {
		BFLockRelease(&_queueSema);
		BFThreadAsyncWait(_tidRequestQueue[i]);
		BFThreadAsyncDestroy(_tidRequestQueue[i]);
	}

	// flush q
	_incomingRequests.lock();
	while (!_incomingRequests.unsafeget().empty()) {
		Envelope * e = _incomingRequests.unsafeget().front();
		_incomingRequests.unsafeget().pop();
		BFRelease(e);
	}
	_incomingRequests.unlock();
	BFLockDestroy(&_queueSema);
}

