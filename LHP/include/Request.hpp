//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LHP_SERVER_REQUEST_HPP
#define LHP_SERVER_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace LHP {
namespace Server {

/**
A request received from a client.
*/
class Request
{
public:
  std::string Method;
  std::string Uri;
  int HttpVersionMajor;
  int HttpVersionMinor;
  std::vector<Header> Headers;
  std::string PostData;
};

} 
} 

#endif // LHP_SERVER_REQUEST_HPP
