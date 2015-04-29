#include <Rcpp.h>
#include <boost/asio.hpp>
using namespace Rcpp;

#ifndef __DNS_RESOLVE__
#define __DNS_RESOLVE__

class dns_resolve {

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

};

#endif