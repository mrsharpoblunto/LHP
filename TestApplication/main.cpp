// LHP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

#include "HttpServer.hpp"
#include "TestApplication.hpp"

std::function<void(void)> StopServerHandler;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch (ctrl_type)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    StopServerHandler();
    return TRUE;
  default:
    return FALSE;
  }
}

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 4)
    {
      std::cerr << "Usage: http_server <address> <port> <threads>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    http_server 0.0.0.0 80 1 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    http_server 0::0 80 1 .\n";
      return 1;
    }

	//create an instance of our web application
	TestApplication application;

    // Initialise server.
    std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);
    LHP::Server::HttpServer server(
		argv[1], 
		argv[2], 
		num_threads,
		"./Content/",
		application);

    // Set console control handler to allow server to be stopped.
	StopServerHandler = [&server]() { server.Stop(); };
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

    // Run the server until stopped.
    server.Run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
	return 1;
  }

  return 0;
}

