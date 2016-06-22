// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(AsioHeaders)]]

#include <Rcpp.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <asio.hpp>
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif
#include <iostream>
#include <sstream>

using namespace Rcpp;

#ifndef __ASIO_BINDINGS__
#define __ASIO_BINDINGS__

/**
 * A class containing the asio bindings in use in iptools.
 */
class asio_bindings {

private:

  /**
   * A consistent (well, instantiated once per run) io_service
   * instance; used by hostname_to_dns/ip_to_dns functions, and
   * (since it's reusable) we'll hopefully get a speed improvement
   * out of only using one.
   */
  asio::io_service io_service;

  /**
   * A function for taking a hostname ("http://en.wikipedia.org")
   * and converting it to the actual IP addresses it resolves to.
   *
   * @param hostname a hostname.
   *
   * @param resolver_ptr a reference to an asio resolver.
   *
   * @see single_ip_to_dns for the reverse operation, or
   * multi_hostname_to_dns for the vectorised version.
   *
   * @return a vector containing the IP address(es) the hostname
   * resolves to.
   */
  std::vector < std::string > single_hostname_to_dns(std::string hostname, asio::ip::tcp::resolver& resolver_ptr);

  /**
   * A function for taking an IP address ("162.243.111.4")
   * and identifying the hostname(s) that IP uses for DNS
   *
   * @param ip_address an IP address.
   *
   * @param resolver_ptr a reference to an asio resolver.
   *
   * @see single_hostname_to_dns for the reverse operation, or
   * multi_ip_to_dns for the vectorised version.
   *
   * @return a vector containing the hostname(s) the IP
   * resolves to.
   */
  std::vector < std::string > single_ip_to_dns(std::string ip_address, asio::ip::tcp::resolver& resolver_ptr);

  /**
   * A function for identifying whether or not an IP
   * address falls within a CIDR range.
   *
   * @param ip_address an IP address.
   *
   * @param range the range.
   *
   * @see ip_in_range_ for the vectorised version
   *
   * @return a boolean true (in range) or false (not
   * in range)
   */
  bool single_ip_in_range(std::string ip_address, std::string range);

  /**
   * A function for identifying the minimum and maximum values
   * of a given IP range
   *
   * @param range an IP range.
   *
   * @see calculate_range_ for the vectorised version
   *
   * @return a string vector of length 2 containing the
   * maxima and minima for that IP range, or c("Invalid",
   * "Invalid") if the range is, well, invalid.
   */
  std::vector < std::string > calculate_ip_range(std::string range);

  /**
   * A function for identifying whether a given string is
   * a valid IPv4 CIDR range
   *
   * @param range an IP range.
   *
   * @see validate_range_ for the vectorised version
   *
   * @return a boolean true or false, where true is
   * "this is a CIDR range" and false is "this isn't,
   * or isn't a valid IP at all"
   */
  bool validate_single_range(std::string range);

  /**
   * A function for tokenising XFF fields
   *
   * @param x_forwarded_for an x_forwarded_for field
   *
   * @see xff_normalise which uses this
   *
   * @return a vector containing the chain of IP addresses
   * split out from the XFF field.
   */
  std::vector < std::string > tokenise_xff(std::string x_forwarded_for);

public:

  /**
   * A function for taking a vector of hostnames ("http://en.wikipedia.org")
   * and converting it to the actual IP addresses it resolves to.
   *
   * @param a vector of strings representing the hostnames.
   *
   * @see single_hostname_to_dns for the non-vectorised version, or
   * multi_ip_to_dns for the same operation in reverse.
   *
   * @return a list of vectors, each vector containing the IP address(es)
   * the equivalently-indexed input hostname resolved to.
   */
  std::list < std::vector < std::string > > multi_hostname_to_dns(
      std::vector < std::string > hostnames
  );

  /**
   * A function for taking a vector of IP addresses ("162.243.111.4")
   * and converting them to the hostname that represents them.
   *
   * @param a vector of strings representing the IP addresses,
   * in dotted-decimal form.
   *
   * @see single_ip_to_dns for the non-vectorised version, or
   * multi_hostname_to_dns for the same operation in reverse.
   *
   * @return a list of vectors, each vector containing the hostnames
   * the equivalently-indexed input IP resolved to. Mostly
   * this will only be one hostname, but not always.
   */
  std::list < std::vector < std::string > > multi_ip_to_dns(
      std::vector < std::string > ip_addresses
  );

  std::vector < unsigned long > v6_scope_(std::vector < std::string > ip_addresses);

  std::vector < std::string > expand_ipv6_(std::vector < std::string > ip_addresses);

  /**
   * A function for taking a vector of IPv4 addresses in dotted-decimal
   * notation ("162.243.111.4") and converting them to their numeric
   * representation.
   *
   * @param a vector of strings representing the IP addresses,
   * in dotted-decimal form.
   *
   * @see ip_to_numeric_ for the opposite functionality.
   *
   * @return a vector of unsigned integers containing the numeric
   * representation of each input IP. Non-IPv4 IPs are represented
   * with -1
   */
  std::vector < unsigned int > ip_to_numeric_(std::vector < std::string > ip_addresses);

  /**
   * A function for taking a vector of IPv4 addresses in numeric form
   * and converting them to their dotted-decimal notation.
   *
   * @param a vector of unsigned integers representing the IP addresses.
   *
   * @see numeric_to_ip_ for the opposite functionality.
   *
   * @return a vector of strings containing the dotted-decimal
   * representation of each input IP. Non-IPv4 IPs are represented
   * with an empty string
   */
  std::vector < std::string > numeric_to_ip_ (std::vector < unsigned int > ip_addresses);

  /**
   * Classify IP addresses as either IPv4, IPv6 or invalid.
   *
   * @param a vector of strings containing IP addresses, in
   * dotted-decimal form.
   *
   * @return a vector of strings containing "IPv4", "IPv6"
   * or "Invalid" for each element of the input vector.
   */
  CharacterVector classify_ip_ (CharacterVector ip_addresses);

  /**
   * Identify if IP addresses are multicast or not
   *
   * @param a vector of strings containing IP addresses, in
   * dotted-decimal form.
   *
   * @return a boolean vector containing trues for those that
   * are multicast, and false for those that aren't.
   */
  LogicalVector is_multicast_ (CharacterVector ip_addresses);

  /**
   * A function for identifying whether or vector of
   * IP addresses fall within a CIDR range
   *
   * @param ip_addresses a vector of IP addresses
   *
   * @param range a vector of ranges.
   *
   * @see single_ip_in_range for the non-vectorised
   * version.
   *
   * @return a vector of boolean true (in range) or false (not
   * in range) for each IP.
   */
  std::vector < bool > ip_in_range_(std::vector < std::string > ip_addresses, std::vector < std::string > ranges);
  std::vector < bool > ip_in_any_(std::vector < std::string > ip_addresses, std::vector < std::string > ranges);

  /**
   * A vectorised version of calculate_ip_range
   *
   * @param ranges a vector of CIDR ranges
   *
   * @see calculate_ip_range for the non-vectorised
   * version.
   *
   * @return a list of vectors, each vector containing
   * the minimum and maximum IPs in a given range, in
   * dotted-decimal notation.
   */
  DataFrame calculate_range_(std::vector < std::string > ranges);

  /**
   * A vectorised version of validate_single_range
   *
   * @param ranges a vector of IP ranges.
   *
   * @see validate_range_ for the vectorised version
   *
   * @return a vector of boolean true or falses, where true is
   * "this is a CIDR range" and false is "this isn't,
   * or isn't a valid IP at all"
   */
  std::vector < bool > validate_range_(std::vector < std::string > ranges);

  /**
   * A normaliser for the x_forwarded_for HTTP field. Takes a vector of IPs and the
   * corresponding XFF headers and grabs the earliest valid XFF.
   *
   * @param ip_addresses a vector of IP addresses.
   *
   * @param x_forwarded_for a vector of x_forwarded_for fields
   *
   * @return a vector of normalised XFF fields - specifically, the earliest valid
   * IP address in the chain.
   *
   */
  std::vector < std::string > xff_normalise(std::vector < std::string > ip_addresses,
                                            std::vector < std::string > x_forwarded_for);
};

#endif