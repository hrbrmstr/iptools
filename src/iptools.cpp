#include <Rcpp.h>
#include <boost/asio.hpp>

using namespace Rcpp;

Function message("message"); // lets us use R's message() function

//' Returns all \code{A} records associated with a hostname
//'
//' Takes in a hostname and returns all IP DNS entries for it. Not vectorized.
//'
//' @param hostname character string of host to lookup
//' @return vector IP addresses
//' @examples
//' \dontrun{
//' # forward
//' gethostbyname("dds.ec")
//' ## [1] "162.243.111.4"
//'
//' # multiple return values
//' gethostbyname("google.com")
//' ##  [1] "2607:f8b0:4006:806::100e" "74.125.226.14"
//' ##  [3] "74.125.226.8"             "74.125.226.3"
//' ##  [5] "74.125.226.6"             "74.125.226.4"
//' ##  [7] "74.125.226.9"             "74.125.226.0"
//' ##  [9] "74.125.226.2"             "74.125.226.1"
//' ## [11] "74.125.226.5"             "74.125.226.7"
//' }
//[[Rcpp::export]]
std::vector< std::string > gethostbyname(std::string hostname) {

  // setup storage for our return value

  std::vector<std::string> addresses;

  boost::asio::io_service io_service;

  // we're dealing with network/connectivity 'stuff' + you never know
  // when DNS queries will fail, so we need to handle exceptional cases

  try {

    // setup the resolver query

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(hostname, "");

    // prepare response iterator

    boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    boost::asio::ip::tcp::endpoint endpoint;

    // example of using a c-ish while loop to iterate through possible multiple resoponses

    while (destination != end) {
      endpoint = *destination++;
      addresses.push_back(endpoint.address().to_string());

    }

  } catch(boost::system::system_error& error) {
    message( "Hostname not found" );
  }

  return(addresses);

}

//' Returns all \code{PTR} records associated with an IPv4 address
//'
//' Takes in an IPv4 address and returns all IP reverse DNS entries for it. Not vectorized.
//'
//' @param ipv4 character string (dotted-decimal) IPv4 address
//' @return vector of hostnames (will usually be just a single host)
//' @examples
//' \dontrun{
//' # reverse
//' gethostbyaddr(gethostbyname("dds.ec"))
//' ## [1] "162.243.111.4"
//' }
//[[Rcpp::export]]
std::vector< std::string > gethostbyaddr(std::string ipv4) {

  // setup storage for our return value

  std::vector<std::string> hostnames;

  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::io_service io_service;

  // we're dealing with network/connectivity 'stuff' + you never know
  // when DNS queries will fail, so we need to handle exceptional cases

  try {

    // setup the resolver query (for PTR record)

    boost::asio::ip::address_v4 ip = boost::asio::ip::address_v4::from_string(ipv4);
    endpoint.address(ip);
    boost::asio::ip::tcp::resolver resolver(io_service);

    // prepare response iterator

    boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(endpoint);
    boost::asio::ip::tcp::resolver::iterator end;

    // example of using a for-loop to iterate through possible multiple resoponses

    for (int i=1; destination != end; destination++, i++) {
       hostnames.push_back(destination->host_name());
    }

  } catch(boost::system::system_error& error) {
    message( "Address not found" );
  }

  return(hostnames);

}

//' Character (dotted-decimal) IPv4 Address Conversion to long integer
//'
//' Convert IP addresses in character (dotted-decimal) notation to long integers
//'
//' @param ip input character vector of IPv4 addresses (dotted-decimal)
//' @return vector of equivalent long integer IP addresses
//' @examples
//' \dontrun{
//' ip2long("24.0.5.11")ghb
//' ip2long(c("24.0.5.11", "211.3.77.96"))
//' }
// [[Rcpp::export]]
NumericVector ip2long (CharacterVector ip) {

  int ipCt = ip.size(); // how many elements in vector

  NumericVector ipInt(ipCt); // allocate new numeric vector

  // CONVERT ALL THE THINGS!
  for (int i=0; i<ipCt; i++) {
    ipInt[i] = boost::asio::ip::address_v4::from_string(ip[i]).to_ulong();
  }

  return(ipInt);
}


//' Intger IPv4 Address Conversion to Character
//'
//' Converts IP addresses in long integer format to character (dotted-decimal) notation
//'
//' @param ip input numeric (long integer) vector
//' @return vector of equivalent character (dotted-decimal) IP addresses
//' @examples
//' \dontrun{
//' long2ip(402654475)
//' long2ip(c(402654475, 3540208992))
//' }
// [[Rcpp::export]]
CharacterVector long2ip (NumericVector ip) {

  int ipCt = ip.size();

  CharacterVector ipStr(ipCt); // allocate new character vector

  // CONVERT ALL THE THINGS!
  for (int i=0; i<ipCt; i++) {
    ipStr[i] = boost::asio::ip::address_v4(ip[i]).to_string();
  }

  return(ipStr);

}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//' IPv4 CIDR to long integer range
//'
//' Converts IPv4 CIDR (e.g. "192.168.1.0/24") to vector containing the minimum and maximum range integer values
//'
//' @param cidr IPv4 CIDR (str) dotted-decimal-slash-integer
//' @return vector containing the minimum and maximum range integer values or \code{c(NA,NA)}
//' @examples
//' \dontrun{
//' long2ip(cidr_range("192.168.1.0/24"))
//' ## [1] "192.168.1.0"   "192.168.1.255"
//'
//' long2ip(cidr_range("8.0.0.0/8"))
//' ## [1] "8.0.0.0"       "8.255.255.255"
//'
//' cr <- cidr_range("172.18.0.0/28")
//' sapply(cr[1]:cr[2], long2ip)
//' ##  [1] "172.18.0.0"  "172.18.0.1"  "172.18.0.2"  "172.18.0.3"
//' ##  [5] "172.18.0.4"  "172.18.0.5"  "172.18.0.6"  "172.18.0.7"
//' ##  [9] "172.18.0.8"  "172.18.0.9"  "172.18.0.10" "172.18.0.11"
//' ## [13] "172.18.0.12" "172.18.0.13" "172.18.0.14" "172.18.0.15"
//' }
// [[Rcpp::export]]
NumericVector cidr_range(std::string cidr) {

  unsigned int first_ip, last_ip;
  int slash_val;
  char cidr_copy[24];
  char *slash_pos;

  // no asio functions to help us here, so might as well turn to
  // raw C code for what we need

  strncpy(cidr_copy, cidr.c_str(), 24); // safe copy

  slash_pos = strchr(cidr_copy, '/'); // find the "/"

  // no "/" == not a valid CIDR
  if (NULL == slash_pos) { return NumericVector::create(NA_REAL, NA_REAL); }

  // replace "/" with string termination and advance the pointer to
  // the CIDR component

  *slash_pos++ = '\0';

  slash_val = atoi(slash_pos); // convert the CIDR mask to an integer

  // convert presentation to network format
  // if it fails, it's not a valid IPv4 address

  if (1 != inet_pton(AF_INET, cidr_copy, &first_ip)) {
    return NumericVector::create(NA_REAL, NA_REAL);
  }

  first_ip = ntohl(first_ip); // network byte order to host byte order

  unsigned int mask = ~0;

  if (slash_val < 32)  {
    last_ip = first_ip | (mask >> slash_val);
  } else { // special case where CIDR mask was 32 (a single IPv4)
    last_ip = first_ip;
  }

  return NumericVector::create(first_ip, last_ip);

}


//' Test if IPv4 addresses are in a CIDR block
//'
//' Takes a vector of character IPv4 addresses and a character CIDR and
//' returs a logical vector indicating whether an IP address falls within
//' the specified CIDR
//'
//' @param ip character vector of IPv4 addresses
//' @param cidr atomic character vector (IPv4 CIDR spec)
//' @return logical vector of equivalent character (dotted-decimal) IP addresses
//' @examples
//' \dontrun{
//' table(ip_in_cidr(cidr_ips("192.168.0.0/23"), "192.168.1.0/24"))
//'
//' ## FALSE  TRUE
//' ##  256   256
//' }
// [[Rcpp::export]]
LogicalVector ip_in_cidr(CharacterVector ip, std::string cidr) {

  unsigned int first_ip;
  int slash_val;
  char cidr_copy[24];
  char *slash_pos;

  int ipCt = ip.size();

  // no asio functions to help us here, so might as well turn to
  // raw C code for what we need

  strncpy(cidr_copy, cidr.c_str(), 24); // safe copy

  slash_pos = strchr(cidr_copy, '/'); // find the "/"

  // no "/" == not a valid CIDR
  if (NULL == slash_pos) { message("NULL") ; return LogicalVector(ipCt, false); }

  // replace "/" with string termination and advance the pointer to
  // the CIDR component

  *slash_pos++ = '\0';

  slash_val = atoi(slash_pos); // convert the CIDR mask to an integer

  first_ip = boost::asio::ip::address_v4::from_string(std::string(cidr_copy)).to_ulong();

  unsigned int mask = ~(0xffffffff >> slash_val);
  unsigned int cidr_int = first_ip & mask ;

  LogicalVector bv = LogicalVector(ipCt) ;

  for (int i=0; i<ipCt; i++) {
    bv[i] = (boost::asio::ip::address_v4::from_string(ip[i]).to_ulong() & mask) == cidr_int;
  }

  return(bv) ;

}
