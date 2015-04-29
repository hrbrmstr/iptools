#include <Rcpp.h>
#include "dns.h"
using namespace Rcpp;

Function message("message"); // lets us use R's message() function

//' @title Returns the IP addresses associated with a hostname.
//' @description takes in a vector of hostnames and returns the IP addresses from
//' each hostname's DNS entries.
//' for each one.
//'
//' @param hostnames a vector of hostnames.
//'
//' @return a list the length of \code{hostnames}, containing the IP addresses from
//' each hostname's DNS entries. In the event that a hostname cannot be resolved,
//' the list element will consist of a length-1 character vector containing
//' "Not resolved".
//'
//' @examples
//' Takes in a hostname and returns all IP DNS entries for it. Not vectorized.
//'
//' \dontrun{
//' # One hostname
//' hostname_to_ip("dds.ec")
//' ## [1] "162.243.111.4"
//'
//' # Multiple hostnames
//' hostname_to_ip(c("dds.ec","ironholds.org"))
//' #[[1]]
//' #[1] "162.243.111.4"
//' #[[2]]
//' #[1] "104.131.2.226"
//' }
//' @export
//[[Rcpp::export]]
std::list < std::vector < std::string > > hostname_to_ip(std::vector < std::string > hostnames){
  dns_resolve dns_inst;
  return dns_inst.multi_hostname_to_dns(hostnames);
}

//' @title Return the hostname associated with particular IP addresses
//' @description the opposite, in some ways, of \code{\link{hostname_to_ip}},
//' \code{ip_to_hostname} consumes a vector of IP addresses and provides a list of
//' the hostnames that those IPs resolve to.
//'
//' @param ip_addresses a vector of IP addresses.
//'
//' @return a list, each entry containing a vector of hostnames for the equivalent input
//' IP address (mostly, this will only be one hostname but not always). If the IP cannot be
//' resolved, the list element will be the string "Invalid IP address".
//'
//' @seealso \code{\link{hostname_to_ip}}, for the reverse operation.
//'
//' @examples
//' \dontrun{
//' ip_to_hostname("162.243.111.4")
//' [[1]]
//' [1] "dds.ec"
//' }
//'
//' @export
//[[Rcpp::export]]
std::list < std::vector < std::string > > ip_to_hostname(std::vector < std::string > ip_addresses){
  dns_resolve dns_inst;
  return dns_inst.multi_ip_to_dns(ip_addresses);
}

//' @title convert between numeric and dotted-decimal IPv4 forms.
//' @description \code{ip_to_numeric} takes IP addresses stored
//' in their human-readable representation ("192.168.0.1")
//' and converts it to a numeric representation (3232235521).
//' \code{numeric_to_ip} performs the same operation, but in reverse.
//' Due to limitations in R's support for colossally
//' big numbers, this currently only works for IPv4 IP addresses.
//'
//' @param ip_addresses a vector of IP addresses, in their numeric or dotted-decimal
//' form.
//'
//' @return For \code{ip_to_numeric}: a vector containing the numeric representation of \code{ip_addresses}.
//' If an IP is invalid (either because it's an Ipv6 address, or isn't an IP address
//' at all) the returned value for that IP will be -1.
//'
//' For \code{numeric_to_ip}: a vector containing the dotted-decimal representation of \code{ip_addresses},
//' as character strings. If a value cannot be resolved to an IPv4 address, it will appear as an empty
//' string.
//'
//' @examples
//' #Convert your local, internal IP to its numeric representation.
//' ip_to_numeric("192.168.0.1")
//' #[1] 3232235521
//'
//' #And back
//' numeric_to_ip(3232235521)
//' #[1] 192.168.0.1"
//' @rdname ip_numeric
//' @export
// [[Rcpp::export]]
std::vector < unsigned int > ip_to_numeric(std::vector < std::string > ip_addresses){

  unsigned int input_size = ip_addresses.size();
  std::vector < unsigned int > output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    try{
      output[i] = boost::asio::ip::address_v4::from_string(ip_addresses[i]).to_ulong();
    } catch (...) {
      output[i] = -1;
    }
  }

  return output;
}

//' @rdname ip_numeric
//' @export
// [[Rcpp::export]]
std::vector < std::string > numeric_to_ip (std::vector < unsigned int > ip_addresses){
  unsigned int input_size = ip_addresses.size();
  std::vector < std::string > output(input_size);

  for(unsigned int i = 0; i < input_size; i++){
    try{
      output[i] = boost::asio::ip::address_v4(ip_addresses[i]).to_string();
    } catch (...) {
      output[i] = "";
    }
  }

  return output;
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