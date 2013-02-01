//============================================================================
// Name        : sender.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <string>

#include <stdlib.h>

#include "Socket.h"
#include "Log.h"
#include "MyException.h"

using namespace std;


int main(int argc, char** argv)
{
	cout << "hello" << endl;

	// READ COMMAND LINE
    int port = 1235;

	for (int i = 0; i < argc; i++)
	{
		string arg = argv[i];

		if (arg == "--port" && i+1 < argc)
		{
			port = atoi(argv[i+1]);
		}
	}

	try
	{
		Socket socket;
		socket.ConnectToHost("localhost", port);


		Message mes;
		mes.setPhone("+79115361051");
		mes.setText("Hello from home");

		socket.SendMessage(mes);
	}
	catch (MyException& e)
	{
		cout << e.what() << endl;
	}



}
