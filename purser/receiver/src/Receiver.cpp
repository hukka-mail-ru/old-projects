//============================================================================
// Name        : daemon.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "Receiver.h"
#include "MyException.h"
#include "Log.h"

using namespace std;



int Receiver::Run()
{
	// LISTEN
	while (true)
	{
		try
		{
			mSocket.Open();

			// Get new message
			Message mes = mSocket.ReceiveMessage();

			Message reply;
			reply.SetPhone("+79119089209");
			reply.SetText("This is a normal reply");

			mSocket.SendMessage(reply);

			mSocket.Close();
		}
		catch (MyException& e)
		{
			Log::Write(e);
		}
	}

	// STOP LISTENING
	try
	{
		mSocket.StopListen();
	}
	catch (MyException& e)
	{
		Log::Write(e);
		return -1;
	}

	return 0;
}

void Receiver::ListenPort(int port)
{
	mSocket.Listen(port);
	PRINT_LOG << "Listen: " << port << "\n";
}


int main(int argc, char** argv)
{
	try
	{
		string logfile = "/var/log/receiver.log";

		// READ COMMAND LINE
		string pidfile = "/var/run/receiver.pid";
		int port = 1234;

		for (int i = 0; i < argc; i++)
		{
			string arg = argv[i];

			if (arg == "--pidfile" && i+1 < argc)
			{
				pidfile = argv[i+1];
			}
			if (arg == "--logfile" && i+1 < argc)
			{
				logfile = argv[i+1];
			}
			else if (arg == "--port" && i+1 < argc)
			{
				port = atoi(argv[i+1]);
			}
		}

		Log::SetLogFile(logfile);

		Receiver receiver(pidfile);
		receiver.ListenPort(port);

		cout << "Log : " << logfile << endl;
		cout << "PID : " << pidfile << endl;
		cout << "Port: " << port << endl;
		cout << "Ready." << endl;

		// daemon loop
		return receiver.Daemonize();
	}
	catch (MyException& e)
	{
		Log::Write(e);
		return -1;
	}
}
