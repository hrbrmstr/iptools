# See iptools.cpp for is_multicast

#'@rdname is_checks
#'@export
is_ipv4 <- function(ip_addresses){
  return((ip_classify(ip_addresses) == "IPv4"))
}

#'@rdname is_checks
#'@export
is_ipv6 <- function(ip_addresses){
  return((ip_classify(ip_addresses) == "IPv6"))
}

#'@rdname is_checks
#'@export
is_valid <- function(ip_addresses){
  return((ip_classify(ip_addresses) != "Invalid"))
}