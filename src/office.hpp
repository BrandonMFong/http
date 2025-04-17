/**
 * author: brando
 * date: 4/3/25
 */

#ifndef OFFICE_HPP
#define OFFICE_HPP

#include <bfnet/envelope.hpp>

namespace Office {

void envelopeReceive(BF::Net::Envelope * envelope);
void start();
void stop();

}

#endif // OFFICE_HPP

