//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LHP_SERVER_SERVER_HPP
#define LHP_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "request_handler.hpp"
#include "IApplication.hpp"

namespace LHP {
namespace Server {


/// The top-level class of the HTTP server.
class HttpServer
  : private boost::noncopyable
{
public:
  /**
   Construct the server to listen on the specified TCP address and port, and
   serve up files from the given directory using the logic provided by the supplied application
   */
  explicit HttpServer(const std::string& address, const std::string& port, std::size_t threadPoolSize,
	  const std::string& doc_root, IApplication &application);

  /// Run the server's io_service loop.
  void Run();

  /// Stop the server.
  void Stop();

private:
  /// Handle completion of an asynchronous accept operation.
  void handle_accept(const boost::system::error_code& e);

  /// The number of threads that will call io_service::run().
  std::size_t thread_pool_size_;

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The next connection to be accepted.
  connection_ptr new_connection_;

  /// The handler for all incoming requests.
  request_handler request_handler_;
};

} 
} 

#endif // LHP_SERVER_SERVER_HPP
