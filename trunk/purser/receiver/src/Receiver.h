/*
 * receiver.h
 *
 *  Created on: 5 Feb 2013
 *      Author: hukka
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Socket.h"
#include "Daemon.h"

class Receiver: public Daemon
{
public:
	Receiver(const std::string& pidfile, const std::string& configfile):
		Daemon(pidfile, configfile) {}

	virtual int Run();

	void ListenPort(int port);

	SETTER_(int, Outport);

private:
	Socket mSocket;
	int mOutport;
};


#endif /* RECEIVER_H_ */
