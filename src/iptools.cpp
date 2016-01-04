// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(AsioHeaders)]]

#include <Rcpp.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#include <asio.hpp>
#pragma clang diagnostic pop

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
//' at all) the returned value for that IP will be 0.
//'
//' For \code{numeric_to_ip}: a vector containing the dotted-decimal representation of \code{ip_addresses},
//' as character strings. If a value cannot be resolved to an IPv4 address, it will appear as "0.0.0.0" or
//' an empty string.
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
//'@seealso \code{\link{is_valid}} et al for logical checks of IP addresses,
//'\code{\link{ip_to_hostname}} for resolving IP addresses to their
//'hostnames, and \code{\link{ip_to_numeric}} for converting (IPv4) IP addresses
//'to their numeric representation.
//'
//'@examples
//'
//'#IPv4
//'ip_classify("173.194.123.100")
//'#[1] "IPv4"
//'
//'#IPv6
//'ip_classify("2607:f8b0:4006:80b::1004")
//'#[1] "IPv6"
//'
//'#Invalid
//'ip_classify("East Coast Twitter is Best Twitter")
//'#[1] "Invalid"
//'
//'@export
//[[Rcpp::export]]
std::vector < std::string > ip_classify(std::vector < std::string > ip_addresses){
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
//' @return a data.frame of two columns, "minimum_ip" (containing the
//' smallest IP in the provided range) and "maximum_ip" (containing the
//' largest). If the range was invalid, both columns will contain
//' "Invalid" as the value.
//'
//' @examples
//' range_boundaries("172.18.0.0/28")
//' #  minimum_ip  maximum_ip
//' #1 172.18.0.0 172.18.0.15
//'
//' @seealso \code{\link{ip_in_range}} to calculate if an IP address
//' falls within a particular range, or \code{\link{ip_to_numeric}} to
//' convert the dotted-decimal notation of returned IP addresses to their
//' numeric representation.
//'
//' @export
// [[Rcpp::export]]
DataFrame range_boundaries(std::vector < std::string > ranges){
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
//'@seealso \code{\link{range_boundaries}} for identifying the minimum
//'and maximum IPs within a range, and \code{\link{validate_range}} for
//'validating that a range exists.
//'
//'@examples
//'#Is this in the range? Yes!
//'ip_in_range("172.18.0.1","172.18.0.0/28")
//'#[1] TRUE
//'
//'@export
//[[Rcpp::export]]
std::vector < bool > ip_in_range(std::vector < std::string > ip_addresses, std::vector < std::string > ranges){
  asio_bindings asio_inst;
  return asio_inst.ip_in_range_(ip_addresses, ranges);
}

//'@title check whether IPv4 ranges are valid
//'@description \code{validate_range} checks whether
//'a vector of IPv4 CIDR ranges ("127.0.0.1/32") are valid or not.
//'
//'@param ranges a vector of IPv4 ranges
//'
//'@return a logical vector, where TRUE indicates that the
//'provided entry is valid, and FALSE that it is not (or
//'isn't an IP range at all)
//'
//'@seealso \code{\link{ip_classify}} for classifying
//'(and, incidentally, validating) IPv4 and IPv6 addresses, or
//'\code{\link{range_boundaries}} for identifying the minimum
//'and maximum IPs within a range.
//'
//'@examples
//'validate_range("127.0.0.1/32")
//'#[1] TRUE
//'validate_range("127.0.0.1/33")
//'#[1] FALSE
//'
//' @export
//[[Rcpp::export]]
std::vector < bool > validate_range(std::vector < std::string > ranges){
  asio_bindings asio_inst;
  return asio_inst.validate_range_(ranges);
}

//'@title Take vectors of IPs and X-Forwarded-For headers and produce single, normalised
//'IP addresses.
//'@description \code{xff_extract} takes IP addresses and x_forwarded_for
//'values and, in the event that x_forwarded_for is non-null, attempts to
//'extract the "real" IP closest to the client.
//'
//'@param ip_addresses a vector of IP addresses
//'
//'@param x_forwarded_for an equally-sized vector of X-Forwarded-For header
//'contents.
//'
//'@return a vector of IP addresses, incorporating the XFF header value
//'where appropriate.
//'
//'@examples
//'xff_extract("192.168.0.1", "193.168.0.1, 230.98.107.1")
//'
//'@export
// [[Rcpp::export]]
std::vector < std::string > xff_extract(std::vector < std::string > ip_addresses,
                                          std::vector < std::string > x_forwarded_for){
  asio_bindings asio_inst;
  return asio_inst.xff_normalise(ip_addresses, x_forwarded_for);
}

//'@title Logical checks for IP addresses
//'@description Check whether an IP address is valid with \code{is_valid},
//'IPv4 with \code{is_ipv4}, IPv6 with \code{is_ipv6}, or multicast (intended to point to
//'multiple machines) with \code{is_multicast}
//'
//'@param ip_addresses a vector of IP addresses
//'
//'@seealso \code{\link{ip_classify}} for character rather than logical classification.
//'
//'@examples
//'# This is multicast
//'is_multicast("224.0.0.2")
//'
//'# It's also IPv4
//'is_ipv4("224.0.0.2")
//'
//'
//'# It's not IPv6
//'is_ipv6("224.0.0.2")
//'
//'@aliases is_ipv4 is_multicast is_ipv6 is_valid
//'@rdname is_checks
//'@export
// [[Rcpp::export]]
LogicalVector is_multicast(std::vector < std::string > ip_addresses){
  asio_bindings asio_inst;
  return asio_inst.is_multicast_(ip_addresses);
}