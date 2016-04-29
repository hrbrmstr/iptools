// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(AsioHeaders)]]

#include <Rcpp.h>
#include <stdio.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <asio.hpp>
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif

#include "asio_bindings.h"

using namespace Rcpp;

#ifdef WIN64
#define IS_WINDOWS
#elif defined WIN32
#define IS_WINDOWS
#endif

#ifdef IS_WINDOWS
/* http://stackoverflow.com/questions/15370033/how-to-use-inet-pton-with-the-mingw-compiler */
#define NS_INADDRSZ  4
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ   2

int inet_pton4(const char *src, unsigned int *dst) {
  uint8_t tmp[NS_INADDRSZ], *tp;

  int saw_digit = 0;
  int octets = 0;
  *(tp = tmp) = 0;

  int ch;
  while ((ch = *src++) != '\0') {
    if (ch >= '0' && ch <= '9') {
      uint32_t n = *tp * 10 + (ch - '0');

      if (saw_digit && *tp == 0) return 0;

      if (n > 255) return 0;

      *tp = n;
      if (!saw_digit) {
        if (++octets > 4) return 0;
        saw_digit = 1;
      }

    } else if (ch == '.' && saw_digit) {
      if (octets == 4) return 0;
      *++tp = 0;
      saw_digit = 0;
    } else return 0;
  }
  if (octets < 4) return 0;

  memcpy(dst, tmp, NS_INADDRSZ);
  return 1;
}

int inet_pton6(const char *src, unsigned int *dst) {
    static const char xdigits[] = "0123456789abcdef";
    uint8_t tmp[NS_IN6ADDRSZ];

    uint8_t *tp = (uint8_t*) memset(tmp, '\0', NS_IN6ADDRSZ);
    uint8_t *endp = tp + NS_IN6ADDRSZ;
    uint8_t *colonp = NULL;

    /* Leading :: requires some special handling. */
    if (*src == ':') {
        if (*++src != ':') return 0;
    }

    const char *curtok = src;
    int saw_xdigit = 0;
    uint32_t val = 0;
    int ch;
    while ((ch = tolower(*src++)) != '\0') {
        const char *pch = strchr(xdigits, ch);
        if (pch != NULL) {
            val <<= 4;
            val |= (pch - xdigits);
            if (val > 0xffff)
                return 0;
            saw_xdigit = 1;
            continue;
        }
        if (ch == ':') {
            curtok = src;
            if (!saw_xdigit) {
                if (colonp)
                    return 0;
                colonp = tp;
                continue;
            } else if (*src == '\0') {
                return 0;
            }
            if (tp + NS_INT16SZ > endp) return 0;
            *tp++ = (uint8_t) (val >> 8) & 0xff;
            *tp++ = (uint8_t) val & 0xff;
            saw_xdigit = 0;
            val = 0;
            continue;
        }
        if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) &&
                inet_pton4(curtok, (unsigned int*) tp) > 0) {
            tp += NS_INADDRSZ;
            saw_xdigit = 0;
            break; /* '\0' was seen by inet_pton4(). */
        }
        return 0;
    }
    if (saw_xdigit) {
        if (tp + NS_INT16SZ > endp)
            return 0;
        *tp++ = (uint8_t) (val >> 8) & 0xff;
        *tp++ = (uint8_t) val & 0xff;
    }
    if (colonp != NULL) {
        /*
         * Since some memmove()'s erroneously fail to handle
         * overlapping regions, we'll do the shift by hand.
         */
        const int n = tp - colonp;

        if (tp == endp)
            return 0;

        for (int i = 1; i <= n; i++) {
            endp[-i] = colonp[n - i];
            colonp[n - i] = 0;
        }
        tp = endp;
    }
    if (tp != endp)
        return 0;

    memcpy(dst, tmp, NS_IN6ADDRSZ);

    return 1;
}

int inet_pton(int af, const char *src, unsigned int *dst){
  switch (af) {
  case AF_INET:
    return inet_pton4(src, dst);
  case AF_INET6:
    return inet_pton6(src, dst);
  default:
    return -1;
  }
}
#endif

std::vector < std::string > asio_bindings::single_hostname_to_dns(std::string hostname,
                                                                  asio::ip::tcp::resolver& resolver_ptr){
  std::vector < std::string > output;

  try{
    asio::ip::tcp::resolver::query query(hostname, "");
    asio::ip::tcp::resolver::iterator destination = resolver_ptr.resolve(query);
    asio::ip::tcp::resolver::iterator end;
    asio::ip::tcp::endpoint endpoint;

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
    asio::ip::tcp::resolver dns_resolver(io_service);
    asio::ip::tcp::resolver& resolver_ptr = dns_resolver;
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
                                                          asio::ip::tcp::resolver& resolver_ptr){
  std::vector < std::string > output;
  asio::ip::tcp::endpoint endpoint;

  try{

    asio::ip::address ip = asio::ip::address::from_string(ip_address);
    endpoint.address(ip);
    asio::ip::tcp::resolver::iterator destination = resolver_ptr.resolve(endpoint);
    asio::ip::tcp::resolver::iterator end;
    for (int i=1; destination != end; destination++, i++) {
      output.push_back(destination->host_name());
    }

  } catch(...){
    output.push_back("Invalid IP address");
  }

  return output;
}

bool asio_bindings::single_ip_in_range(std::string ip_address, std::string range){

  unsigned int first_ip, mask;
  int slash_val;
  char range_copy[24];
  char *slash_pos;
  bool output = false;

  strncpy(range_copy, range.c_str(), 24); // safe copy
  slash_pos = strchr(range_copy, '/'); // find the "/"

  if(slash_pos == NULL){
    return output;
  }

  *slash_pos++ = '\0';

  try{

    slash_val = atoi(slash_pos);
    first_ip = asio::ip::address_v4::from_string(std::string(range_copy)).to_ulong();
    // shifting by 32 bits is undefined
    mask = slash_val == 32 ? 0xffffffff : ~(0xffffffff >> slash_val);
    unsigned int cidr_int = first_ip & mask ;

    output = ((asio::ip::address_v4::from_string(ip_address).to_ulong() & mask) == cidr_int);

  } catch(...){
    output = false;
  }

  return output;
}

std::vector < std::string > asio_bindings::calculate_ip_range(std::string range){

  unsigned int first_ip, last_ip;
  int slash_val;
  char cidr_copy[24];
  char *slash_pos;
  std::vector < std::string > output;

  strncpy(cidr_copy, range.c_str(), 24);
  slash_pos = strchr(cidr_copy, '/');
  if (slash_pos == NULL){
    output.push_back("Invalid");
    output.push_back("Invalid");
  } else {
    *slash_pos++ = '\0';
    slash_val = atoi(slash_pos);

    if (1 != inet_pton(AF_INET, cidr_copy, &first_ip)) {
      output.push_back("Invalid");
      output.push_back("Invalid");
    } else {
      first_ip = ntohl(first_ip);
      unsigned int mask = ~0;

      if (slash_val < 32)  {
        last_ip = first_ip | (mask >> slash_val);
      } else { // special case where CIDR mask was 32 (a single IPv4)
        last_ip = first_ip;
      }
      output.push_back(asio::ip::address_v4(first_ip).to_string());
      output.push_back(asio::ip::address_v4(last_ip).to_string());
    }
  }

  return output;
}

bool asio_bindings::validate_single_range(std::string range){
  bool output = false;
  size_t slash_loc = range.find("/");
  std::string holding;
  long int range_val;
  asio::ip::address converted_ip;

  if(slash_loc == std::string::npos){
    return output;
  }
  holding = range.substr(slash_loc+1);
  range = range.substr(0,slash_loc);

  try{

    converted_ip = asio::ip::address::from_string(range);
    if(!converted_ip.is_v4()){
      return output;
    }
    range_val = atoi(holding.c_str());
    if(range_val >= 1 && range_val <= 32){
      output = true;
    }
  } catch(...){
    return output;
  }

  return output;
}
std::list < std::vector < std::string > > asio_bindings::multi_ip_to_dns(std::vector < std::string > ip_addresses){

  std::list < std::vector < std::string > > output;
  std::vector < std::string > holding;
  try{

    asio::ip::tcp::resolver dns_resolver(io_service);
    asio::ip::tcp::resolver& resolver_ptr = dns_resolver;
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

std::vector < std::string > asio_bindings::expand_ipv6_(std::vector < std::string > ip_addresses) {

  unsigned int input_size = ip_addresses.size();
  std::vector < std::string > output(input_size);
  asio::ip::address_v6::bytes_type v;
  char str[50];

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    try{
      //      output[i] = asio::ip::address_v6::from_string(ip_addresses[i]).to_string();
      v = asio::ip::address_v6::from_string(ip_addresses[i]).to_bytes();
      (void)sprintf(str, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
       (uint8_t)v[0], (uint8_t)v[1], (uint8_t)v[2], (uint8_t)v[3], (uint8_t)v[4], (uint8_t)v[5], (uint8_t)v[6], (uint8_t)v[7], (uint8_t)v[8], (uint8_t)v[9], (uint8_t)v[10], (uint8_t)v[11], (uint8_t)v[12], (uint8_t)v[13], (uint8_t)v[14], (uint8_t)v[15]);
      output[i] = std::string(str);
    } catch (...) {
      output[i] = "";
    }
  }

  return output;
}

std::vector < unsigned long > asio_bindings::v6_scope_(std::vector < std::string > ip_addresses){

  unsigned int input_size = ip_addresses.size();
  std::vector < unsigned long > output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    try{
      output[i] = asio::ip::address_v6::from_string(ip_addresses[i]).scope_id();
    } catch (...) {
      Rcout << "error" << std::endl;
      output[i] = -1;
    }
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
      output[i] = asio::ip::address_v4::from_string(ip_addresses[i]).to_ulong();
    } catch (...) {
      output[i] = 0;
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
      output[i] = asio::ip::address_v4(ip_addresses[i]).to_string();
    } catch (...) {
      output[i] = "";
    }
  }

  return output;
}

CharacterVector asio_bindings::classify_ip_(CharacterVector ip_addresses){
  unsigned int input_size = ip_addresses.size();
  asio::ip::address holding;
  CharacterVector output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(ip_addresses[i] == NA_STRING){
      output[i] = NA_STRING;
    } else {
      try{
        holding = asio::ip::address::from_string(ip_addresses[i]);
        if(holding.is_v4()){
          output[i] = "IPv4";
        } else if(holding.is_v6()){
          output[i] = "IPv6";
        } else {
          output[i] = NA_STRING;
        }
      } catch(...){
        output[i] = NA_STRING;
      }
    }

  }
  return output;
}

LogicalVector asio_bindings::is_multicast_ (CharacterVector ip_addresses){

  unsigned int input_size = ip_addresses.size();
  LogicalVector output(input_size);
  asio::ip::address holding;

  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(ip_addresses[i] == NA_STRING){
      output[i] = NA_LOGICAL;
    } else {
      try{
        holding = asio::ip::address::from_string(ip_addresses[i]);
        if(holding.is_multicast()){
          output[i] = true;
        } else {
          output[i] = false;
        }
      } catch(...){
        output[i] = NA_LOGICAL;
      }
    }
  }
  return output;
}

std::vector < bool > asio_bindings::ip_in_range_(std::vector < std::string > ip_addresses, std::vector < std::string > ranges){

  if(ip_addresses.size() != ranges.size() && ranges.size() != 1){
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

DataFrame asio_bindings::calculate_range_(std::vector < std::string > ranges){
  int input_size = ranges.size();
  std::vector < std::string > min_holding(input_size);
  std::vector < std::string > max_holding(input_size);
  std::vector < std::string > holding;

  for(unsigned int i = 0; i < ranges.size(); i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    holding = calculate_ip_range(ranges[i]);
    min_holding[i] = holding[0];
    max_holding[i] = holding[1];
    holding.clear();
  }

  return DataFrame::create(_["minimum_ip"] = min_holding,
                           _["maximum_ip"] = max_holding,
                           _["min_numeric"] = ip_to_numeric_(min_holding),
                           _["max_numeric"] = ip_to_numeric_(max_holding),
                           _["range"] = ranges,
                           _["stringsAsFactors"] = false);
}

std::vector < bool > asio_bindings::validate_range_(std::vector < std::string > ranges){

  unsigned int input_size = ranges.size();
  std::vector < bool > output(input_size);

  for(unsigned int i = 0; i < ranges.size(); i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    output[i] = validate_single_range(ranges[i]);
  }
  return output;
}


std::vector < std::string > asio_bindings::tokenise_xff(std::string x_forwarded_for){
  std::vector < std::string > output;
  std::string holding;
  std::stringstream strm(x_forwarded_for);
  while(strm.good()){
    getline(strm, holding, ',');
    output.push_back(holding);
  }
  return output;
}

std::vector < std::string > asio_bindings::xff_normalise(std::vector < std::string > ip_addresses,
                                          std::vector < std::string > x_forwarded_for){

  unsigned int input_size = ip_addresses.size();
  if(input_size != x_forwarded_for.size()){
    throw std::range_error("the ip_addresses and x_forwarded_for vectors must be the same size");
  }

  asio::ip::address ip_check;
  std::vector < std::string > holding;
  for(unsigned int i = 0; i < input_size; i++){
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }
    if(x_forwarded_for[i] != "-"){
      holding = tokenise_xff(x_forwarded_for[i]);
      for(unsigned int x = 0; x < holding.size(); x++){
        try{
          ip_check = asio::ip::address::from_string(holding[x]);
          if(ip_check.is_v4() || ip_check.is_v6()){
            ip_addresses[i] = holding[x];
            break;
          }
        } catch(...){

        }
      }
    }
  }

  return ip_addresses;
}