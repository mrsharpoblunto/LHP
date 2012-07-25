//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LHP_SERVER_REQUEST_HANDLER_HPP
#define LHP_SERVER_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>
#include "IApplication.hpp"
#include "ApplicationController.hpp"

namespace LHP {
namespace Server {

class Response;
class Request;

/// The common handler for all incoming requests.
class request_handler
  : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler(const std::string& doc_root,IApplication &application);

  /// Handle a request and produce a reply.
  void handle_request(Request& req, Response& rep);

private:
  /// The directory containing the files to be served.
  std::string doc_root_;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);

  LHP::ApplicationController _applicationController;
};

} 
} 

#endif // LHP_SERVER_REQUEST_HANDLER_HPP
