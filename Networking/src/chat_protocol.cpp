#include "Networking/chat_protocol.h"

#include "boost/beast/core/detail/base64.hpp"
#include <iostream>
#include <string>

namespace Syncopy {

const std::string SendAuth(const std::string& login, const std::string& password) {
  return "AUTH " + login + ":" + password + "\n"; }

std::string SendSay(std::string body) {
  std::string base64EncodedBody;
  base64EncodedBody.resize(
      boost::beast::detail::base64::encoded_size(body.size()));
  boost::beast::detail::base64::encode(&base64EncodedBody[0], body.c_str(),
                                       body.size());
  return "SAY " + base64EncodedBody + "\n";
}

Command GetCommandName(std::string message) {
  std::istringstream iss(message);
  std::string command;
  getline(iss, command, ' ');
  std::cout << "Command is: " << command << std::endl;
  return _hashit(command);
}

AuthMessage ParseAuth(std::string message) {
  AuthMessage credentials;
  std::istringstream iss(message);
  std::string command;
  std::string login;
  std::string password;
  getline(iss, command, ' ');
  getline(iss, login, ':');
  getline(iss, password, '\n');
  std::cout << "Login is: " << login << std::endl;
  std::cout << "Password is: " << password << std::endl;
  credentials.login = login;
  credentials.password = password;
  return credentials;
}

SayMessage ParseSay(std::string message) { return SayMessage(); }

Command _hashit(const std::string &inString) {
  if (inString == "AUTH")
    return AUTH;
  if (inString == "SAY")
    return SAY;
  return BAD_PATH;
}
} // namespace Syncopy