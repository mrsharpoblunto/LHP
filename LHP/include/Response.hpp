//
// reply.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LHP_SERVER_REPLY_HPP
#define LHP_SERVER_REPLY_HPP

#include <string>
#include <vector>
#include <string>
#include <boost/asio.hpp>

#pragma warning ( push )
#pragma warning( disable : 4996 )
#include <xutility>
#pragma warning ( pop )

#pragma warning ( push )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4996 )
#include <luabind/object.hpp>
#pragma warning( pop )

#include "header.hpp"

namespace LHP {
namespace Server {

/**
an HTTP response to send to a client
*/
class Response
{
public:

  /// The headers to be included in the reply.
  std::vector<Header> Headers;

  /// The content to be sent in the reply.
  std::string Content;

  /**
	HTTP response statuses
	*/
	enum ResponseStatus
	{
		Ok = 200,
		Created = 201,
		Accepted = 202,
		NoContent = 204,
		MultipleChoices = 300,
		MovedPermanently = 301,
		MovedTemporarily = 302,
		NotModified = 304,
		BadRequest = 400,
		Unauthorized = 401,
		Forbidden = 403,
		NotFound = 404,
		InternalServerError = 500,
		NotImplemented = 501,
		BadGateway = 502,
		ServiceUnavailable = 503
	} Status;

  void WriteContent(luabind::object const&);

  /// Convert the reply into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  std::vector<boost::asio::const_buffer> ToBuffers();

  /// Get a stock reply.
  static Response StockReply(ResponseStatus status);
};

} 
} 

#endif // LHP_SERVER_REPLY_HPP
