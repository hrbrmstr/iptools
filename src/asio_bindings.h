#include <Rcpp.h>
#include <boost/asio.hpp>
using namespace Rcpp;

#ifndef __ASIO_BINDINGS__
#define __ASIO_BINDINGS__

class asio_bindings {

private:

  boost::asio::io_service io_service;

  std::vector < std::string > single_hostname_to_dns(std::string hostname, boost::asio::ip::tcp::resolver& resolver_ptr);

  std::vector < std::string > single_ip_to_dns(std::string ip_address, boost::asio::ip::tcp::resolver& resolver_ptr);

public:

  std::list < std::vector < std::string > > multi_hostname_to_dns(
      std::vector < std::string > hostnames
  );

  std::list < std::vector < std::string > > multi_ip_to_dns(
      std::vector < std::string > ip_addresses
  );

  std::vector < unsigned int > ip_to_numeric_(std::vector < std::string > ip_addresses);

  std::vector < std::string > numeric_to_ip_ (std::vector < unsigned int > ip_addresses);

  std::vector < std::string > classify_ip_ (std::vector < std::string > ip_addresses);
};

#endif