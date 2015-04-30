#include <Rcpp.h>
#include "asio_bindings.h"
using namespace Rcpp;

//' @title Returns the IP addresses associated with a hostname.
//' @description takes in a vector of hostnames and returns the IP addresses from
//' each hostname's DNS entries. Compatible with both IPv4 and IPv6 addresses.
//'
//' @param hostnames a vector of hostnames.
//'
//' @return a list the length of \code{hostnames}, containing the IP addresses from
//' each hostname's DNS entries. In the event that a hostname cannot be resolved,
//' the list element will consist of a length-1 character vector containing
//' "Not resolved".
//'
//' @seealso \code{ip_to_hostname} for the opposite functionality - resolving
//' IP addresses to their associated hostname(s) - and \code{\link{ip_to_numeric}}
//' for converting IP addresses retrieved from \code{hostname_to_ip} into their
//' numeric representation.
//'
//' @examples
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
  asio_bindings asio_inst;
  return asio_inst.multi_hostname_to_dns(hostnames);
}

//' @title Return the hostname associated with particular IP addresses
//' @description the opposite, in some ways, of \code{\link{hostname_to_ip}},
//' \code{ip_to_hostname} consumes a vector of IP addresses and provides a list of
//' the hostnames that those IPs resolve to. Compatible with both IPv4 and
//' IPv6 addresses.
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
  asio_bindings asio_inst;
  return asio_inst.multi_ip_to_dns(ip_addresses);
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
//' form depending on the function.
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
  asio_bindings asio_inst;
  return asio_inst.ip_to_numeric_(ip_addresses);
}

//' @rdname ip_numeric
//' @export
// [[Rcpp::export]]
std::vector < std::string > numeric_to_ip (std::vector < unsigned int > ip_addresses){
  asio_bindings asio_inst;
  return asio_inst.numeric_to_ip_(ip_addresses);
}

//'@title Identify whether an IP address is IPv4 or IPv6
//'@description Identify the form (IPv4 or IPv6) of a vector
//'of IP addresses. This can also be used to validate IPs.
//'
//'@param ip_addresses a vector of IPv4 or IPv6 IP addresses.
//'
//'@return a vector containing the class of each input IP address; either
//'"IPv4", "IPv6" or, for IP addresses that were not valid, "Invalid".
//'
//'@seealso \code{\link{ip_to_hostname}} for resolving IP addresses to their
//'hostnames, and \code{\link{ip_to_numeric}} for converting (IPv4) IP addresses
//'to their numeric representation.
//'
//'@examples
//'
//'#IPv4
//'classify_ip("173.194.123.100")
//'[1] "IPv4"
//'
//'#IPv6
//'classify_ip("2607:f8b0:4006:80b::1004")
//'[1] "IPv6"
//'
//'#Invalid
//'classify_ip("East Coast Twitter is Best Twitter")
//'[1] "Invalid"
//'
//'@export
//[[Rcpp::export]]
std::vector < std::string > classify_ip(std::vector < std::string > ip_addresses){
  asio_bindings asio_inst;
  return asio_inst.classify_ip_(ip_addresses);
}

//' @title calculate the maximum and minimum IPs in an IP range
//' @description when provided with a vector of IP ranges
//' ("172.18.0.0/28"), \code{range_boundaries} calculates the
//' maximum and minimum IP addresses in that range.
//'
//' @param ranges a vector of IP ranges. Currently only IPv4 ranges
//' work.
//'
//' @return a list of character vectors, each one consisting of the maximum
//' and minimum IPs within the equivalent range.
//'
//' @examples
//' range_boundaries("172.18.0.0/28")
//' # [[1]]
//' # [1] "172.18.0.0"  "172.18.0.15"
//'
//' @seealso \code{\link{ip_in_range}} to calculate if an IP address
//' falls within a particular range, or \code{\link{ip_to_numeric}} to
//' convert the dotted-decimal notation of returned IP addresses to their
//' numeric representation.
//'
//' @export
// [[Rcpp::export]]
std::list < std::vector < std::string > > range_boundaries(std::vector < std::string > ranges){
  asio_bindings asio_inst;
  return asio_inst.calculate_range_(ranges);
}

//'@title check if IP addresses fall within particular IP ranges
//'@description \code{ip_in_range} checks whether a vector of IP
//'addresses fall within particular IP range(s).
//'
//'@param ip_addresses a vector of IP addresses
//'
//'@param ranges either a vector of ranges equal in length
//'to \code{ip_addresses}, or a single range. If the former,
//'\code{ip_in_range} will compare each IP to the
//'equivalent range. If the latter, each IP will be
//'compared to the single range provided.
//'
//'@return a logical vector, where TRUE indicates the relevant
//'IP is in the range, and FALSE indicates that the IP
//'is not in the range, or is an invalid IP address.
//'
//'@export
//[[Rcpp::export]]
std::vector < bool > ip_in_range(std::vector < std::string > ip_addresses, std::vector < std::string > ranges){
  asio_bindings asio_inst;
  return asio_inst.ip_in_range_(ip_addresses, ranges);
}