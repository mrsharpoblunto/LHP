//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LHP_SERVER_HEADER_HPP
#define LHP_SERVER_HEADER_HPP

#include <string>

namespace LHP {
namespace Server {

struct Header
{
  std::string Name;
  std::string Value;
};

} 
} 

#endif // LHP_SERVER_HEADER_HPP
