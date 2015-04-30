#include <Rcpp.h>
#include "asio_bindings.h"
using namespace Rcpp;

std::vector < std::string > asio_bindings::single_hostname_to_dns(std::string hostname,
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

std::list < std::vector < std::string > > asio_bindings::multi_hostname_to_dns(std::vector < std::string > hostnames){

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


std::vector < std::string > asio_bindings::single_ip_to_dns(std::string ip_address,
                                                          boost::asio::ip::tcp::resolver& resolver_ptr){
  std::vector < std::string > output;
  boost::asio::ip::tcp::endpoint endpoint;

  try{
    boost::asio::ip::address_v4 ip = boost::asio::ip::address_v4::from_string(ip_address);
    endpoint.address(ip);
    boost::asio::ip::tcp::resolver::iterator destination = resolver_ptr.resolve(endpoint);
    boost::asio::ip::tcp::resolver::iterator end;
    for (int i=1; destination != end; destination++, i++) {
      output.push_back(destination->host_name());
    }
  } catch (...){
    output.push_back("Invalid IP address");
  }

  return output;
}

bool asio_bindings::single_ip_in_range(std::string ip_address, std::string range){

  unsigned int first_ip;
  int slash_val;
  char range_copy[24];
  char *slash_pos;
  bool output;

  strncpy(range_copy, range.c_str(), 24); // safe copy
  slash_pos = strchr(range_copy, '/'); // find the "/"

  if(slash_pos == NULL){
    return output;
  }

  *slash_pos++ = '\0';
  slash_val = atoi(slash_pos);
  first_ip = boost::asio::ip::address_v4::from_string(std::string(range_copy)).to_ulong();
  unsigned int mask = ~(0xffffffff >> slash_val);
  unsigned int cidr_int = first_ip & mask ;

  output = ((boost::asio::ip::address_v4::from_string(ip_address).to_ulong() & mask) == cidr_int);

  return output;
}

std::list < std::vector < std::string > > asio_bindings::multi_ip_to_dns(std::vector < std::string > ip_addresses){

  std::list < std::vector < std::string > > output;
  std::vector < std::string > holding;
  try{

    boost::asio::ip::tcp::resolver dns_resolver(io_service);
    boost::asio::ip::tcp::resolver& resolver_ptr = dns_resolver;
    for (unsigned int i = 0; i < ip_addresses.size(); ++i){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      holding = single_ip_to_dns(ip_addresses[i], resolver_ptr);
      output.push_back(holding);
      holding.clear();
    }

  } catch(...){
    throw std::range_error("Service could not be created");
  }
  return output;
}

std::vector < unsigned int > asio_bindings::ip_to_numeric_(std::vector < std::string > ip_addresses){

  unsigned int input_size = ip_addresses.size();
  std::vector < unsigned int > output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    try{
      output[i] = boost::asio::ip::address_v4::from_string(ip_addresses[i]).to_ulong();
    } catch (...) {
      output[i] = -1;
    }
  }

  return output;
}

std::vector < std::string > asio_bindings::numeric_to_ip_ (std::vector < unsigned int > ip_addresses){
  unsigned int input_size = ip_addresses.size();
  std::vector < std::string > output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    try{
      output[i] = boost::asio::ip::address_v4(ip_addresses[i]).to_string();
    } catch (...) {
      output[i] = "";
    }
  }

  return output;
}

std::vector < std::string > asio_bindings::classify_ip_ (std::vector < std::string > ip_addresses){
  unsigned int input_size = ip_addresses.size();
  boost::asio::ip::address holding;

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    try{
      holding = boost::asio::ip::address::from_string(ip_addresses[i]);
      if(holding.is_v4()){
        ip_addresses[i] = "IPv4";
      } else if(holding.is_v6()){
        ip_addresses[i] = "IPv6";
      } else {
        ip_addresses[i] = "Invalid";
      }
    } catch(...){
      ip_addresses[i] = "Invalid";
    }

  }
  return ip_addresses;
}

std::vector < bool > asio_bindings::ip_in_range_(std::vector < std::string > ip_addresses, std::vector < std::string > ranges){

  if(ip_addresses.size() != ranges.size() || ranges.size() != 1){
    throw std::range_error("You must provide either one range, or a vector of ranges the same size as the IP addresses");
  }

  unsigned int input_size = ip_addresses.size();
  std::vector < bool > output(input_size);

  if(ranges.size() == 1){
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      output[i] = single_ip_in_range(ip_addresses[i], ranges[0]);
    }
  } else {
    for(unsigned int i = 0; i < input_size; i++){
      if((i % 10000) == 0){
        Rcpp::checkUserInterrupt();
      }
      output[i] = single_ip_in_range(ip_addresses[i], ranges[i]);
    }
  }

  return output;
}