#include <Rcpp.h>
#include "dns.h"
using namespace Rcpp;

std::vector < std::string > dns_resolve::single_hostname_to_dns(std::string hostname,
                                                                boost::asio::ip::tcp::resolver& resolver_ptr){
  std::vector < std::string > output;

  try{
    boost::asio::ip::tcp::resolver::query query(hostname, "");
    boost::asio::ip::tcp::resolver::iterator destination = resolver_ptr.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    boost::asio::ip::tcp::endpoint endpoint;

    while (destination != end){
      endpoint = *destination++;
      output.push_back(endpoint.address().to_string());
    }
  } catch(...){
    output.push_back("Not resolved");
  }

  return output;
}

std::list < std::vector < std::string > >dns_resolve::multi_hostname_to_dns(std::vector < std::string > hostnames){

  unsigned int in_size = hostnames.size();
  std::list < std::vector < std::string > > output;
  std::vector < std::string > holding;
  try{
    boost::asio::ip::tcp::resolver dns_resolver(io_service);
    boost::asio::ip::tcp::resolver& resolver_ptr = dns_resolver;
    for (unsigned int i = 0; i < in_size; ++i){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      holding = single_hostname_to_dns(hostnames[i], resolver_ptr);
      output.push_back(holding);
      holding.clear();
    }
  } catch (...){
    throw std::range_error("Service could not be created");
  }

  return output;
}