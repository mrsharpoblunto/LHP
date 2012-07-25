//
// reply.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"

#include "response.hpp"
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace LHP {
namespace Server {

namespace status_strings {

const std::string ok =
  "HTTP/1.0 200 OK\r\n";
const std::string created =
  "HTTP/1.0 201 Created\r\n";
const std::string accepted =
  "HTTP/1.0 202 Accepted\r\n";
const std::string no_content =
  "HTTP/1.0 204 No Content\r\n";
const std::string multiple_choices =
  "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently =
  "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily =
  "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified =
  "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request =
  "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized =
  "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden =
  "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found =
  "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error =
  "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented =
  "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway =
  "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable =
  "HTTP/1.0 503 Service Unavailable\r\n";

boost::asio::const_buffer to_buffer(Response::ResponseStatus status)
{
  switch (status)
  {
  case Response::Ok:
    return boost::asio::buffer(ok);
  case Response::Created:
    return boost::asio::buffer(created);
  case Response::Accepted:
    return boost::asio::buffer(accepted);
  case Response::NoContent:
    return boost::asio::buffer(no_content);
  case Response::MultipleChoices:
    return boost::asio::buffer(multiple_choices);
  case Response::MovedPermanently:
    return boost::asio::buffer(moved_permanently);
  case Response::MovedTemporarily:
    return boost::asio::buffer(moved_temporarily);
  case Response::NotModified:
    return boost::asio::buffer(not_modified);
  case Response::BadRequest:
    return boost::asio::buffer(bad_request);
  case Response::Unauthorized:
    return boost::asio::buffer(unauthorized);
  case Response::Forbidden:
    return boost::asio::buffer(forbidden);
  case Response::NotFound:
    return boost::asio::buffer(not_found);
  case Response::InternalServerError:
    return boost::asio::buffer(internal_server_error);
  case Response::NotImplemented:
    return boost::asio::buffer(not_implemented);
  case Response::BadGateway:
    return boost::asio::buffer(bad_gateway);
  case Response::ServiceUnavailable:
    return boost::asio::buffer(service_unavailable);
  default:
    return boost::asio::buffer(internal_server_error);
  }
}

} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

std::vector<boost::asio::const_buffer> Response::ToBuffers()
{
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(status_strings::to_buffer(Status));
  for (std::size_t i = 0; i < Headers.size(); ++i)
  {
    Header& h = Headers[i];
    buffers.push_back(boost::asio::buffer(h.Name));
    buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
    buffers.push_back(boost::asio::buffer(h.Value));
    buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  }
  buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  buffers.push_back(boost::asio::buffer(Content));
  return buffers;
}

void Response::WriteContent(luabind::object const& c)
{
	std::stringstream str;
	str << c;
	Content.append(str.str());
}

namespace stock_replies {

const char ok[] = "";
const char created[] =
  "<html>"
  "<head><title>Created</title></head>"
  "<body><h1>201 Created</h1></body>"
  "</html>";
const char accepted[] =
  "<html>"
  "<head><title>Accepted</title></head>"
  "<body><h1>202 Accepted</h1></body>"
  "</html>";
const char no_content[] =
  "<html>"
  "<head><title>No Content</title></head>"
  "<body><h1>204 Content</h1></body>"
  "</html>";
const char multiple_choices[] =
  "<html>"
  "<head><title>Multiple Choices</title></head>"
  "<body><h1>300 Multiple Choices</h1></body>"
  "</html>";
const char moved_permanently[] =
  "<html>"
  "<head><title>Moved Permanently</title></head>"
  "<body><h1>301 Moved Permanently</h1></body>"
  "</html>";
const char moved_temporarily[] =
  "<html>"
  "<head><title>Moved Temporarily</title></head>"
  "<body><h1>302 Moved Temporarily</h1></body>"
  "</html>";
const char not_modified[] =
  "<html>"
  "<head><title>Not Modified</title></head>"
  "<body><h1>304 Not Modified</h1></body>"
  "</html>";
const char bad_request[] =
  "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>";
const char unauthorized[] =
  "<html>"
  "<head><title>Unauthorized</title></head>"
  "<body><h1>401 Unauthorized</h1></body>"
  "</html>";
const char forbidden[] =
  "<html>"
  "<head><title>Forbidden</title></head>"
  "<body><h1>403 Forbidden</h1></body>"
  "</html>";
const char not_found[] =
  "<html>"
  "<head><title>Not Found</title></head>"
  "<body><h1>404 Not Found</h1></body>"
  "</html>";
const char internal_server_error[] =
  "<html>"
  "<head><title>Internal Server Error</title></head>"
  "<body><h1>500 Internal Server Error</h1></body>"
  "</html>";
const char not_implemented[] =
  "<html>"
  "<head><title>Not Implemented</title></head>"
  "<body><h1>501 Not Implemented</h1></body>"
  "</html>";
const char bad_gateway[] =
  "<html>"
  "<head><title>Bad Gateway</title></head>"
  "<body><h1>502 Bad Gateway</h1></body>"
  "</html>";
const char service_unavailable[] =
  "<html>"
  "<head><title>Service Unavailable</title></head>"
  "<body><h1>503 Service Unavailable</h1></body>"
  "</html>";

std::string to_string(Response::ResponseStatus status)
{
  switch (status)
  {
  case Response::Ok:
    return ok;
  case Response::Created:
    return created;
  case Response::Accepted:
    return accepted;
  case Response::NoContent:
    return no_content;
  case Response::MultipleChoices:
    return multiple_choices;
  case Response::MovedPermanently:
    return moved_permanently;
  case Response::MovedTemporarily:
    return moved_temporarily;
  case Response::NotModified:
    return not_modified;
  case Response::BadRequest:
    return bad_request;
  case Response::Unauthorized:
    return unauthorized;
  case Response::Forbidden:
    return forbidden;
  case Response::NotFound:
    return not_found;
  case Response::InternalServerError:
    return internal_server_error;
  case Response::NotImplemented:
    return not_implemented;
  case Response::BadGateway:
    return bad_gateway;
  case Response::ServiceUnavailable:
    return service_unavailable;
  default:
    return internal_server_error;
  }
}

} // namespace stock_replies

Response Response::StockReply(ResponseStatus status)
{
  Response rep;
  rep.Status = status;
  rep.Content = stock_replies::to_string(status);
  rep.Headers.resize(2);
  rep.Headers[0].Name = "Content-Length";
  rep.Headers[0].Value = boost::lexical_cast<std::string>(rep.Content.size());
  rep.Headers[1].Name = "Content-Type";
  rep.Headers[1].Value = "text/html";
  return rep;
}

} 
} 
