//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "response.hpp"
#include "request.hpp"

namespace LHP {
namespace Server {

request_handler::request_handler(const std::string& doc_root,IApplication &application)
  : doc_root_(doc_root)
  , _applicationController(doc_root)
{
	application.OnCreate(_applicationController);
}

void request_handler::handle_request(Request& req, Response& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.Uri, request_path))
  {
    rep = Response::StockReply(Response::BadRequest);
    return;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = Response::StockReply(Response::BadRequest);
    return;
  }

  // If path ends in slash (i.e. is a directory) then add "Index".
  if (request_path[request_path.size() - 1] == '/')
  {
	  req.Uri+= "Index";
  }

  if (!_applicationController.HandleRequest(req,rep)) {
		//if the LHP handler doesn't handle the request, then assume its a static file request
	  // Determine the file extension.
	  std::size_t last_slash_pos = request_path.find_last_of("/");
	  std::size_t last_dot_pos = request_path.find_last_of(".");
	  std::string extension;
	  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
	  {
		extension = request_path.substr(last_dot_pos + 1);
	  }

	  // Open the file to send back.
	  std::string full_path = doc_root_ + request_path;
	  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	  if (!is)
	  {
		rep = Response::StockReply(Response::NotFound);
		return;
	  }

	  // Fill out the reply to be sent to the client.
	  rep.Status = Response::Ok;
	  char buf[512];
	  while (is.read(buf, sizeof(buf)).gcount() > 0)
		rep.Content.append(buf, static_cast<unsigned int>(is.gcount()));
	  rep.Headers.resize(2);
	  rep.Headers[0].Name = "Content-Length";
	  rep.Headers[0].Value = boost::lexical_cast<std::string>(rep.Content.size());
	  rep.Headers[1].Name = "Content-Type";
	  rep.Headers[1].Value = mime_types::extension_to_type(extension);
  }
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} 
} 
