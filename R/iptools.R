#' @title Vectorized version of \code{gethostbyaddr}
#' @description Returns a list (slots are named by the input IPv4 addresses) with lookup results per slot
#' @param ipv4 vector of IPv4 address (character - dotted-decimal)
#' @return list
#' @export
#' @examples
#' set.seed(1492)
#' gethostsbyaddr(randomIPs(5))
#'
gethostsbyaddr <- Vectorize(gethostbyaddr, SIMPLIFY=FALSE)

#' @title Vectorized version of \code{gethostbyname}
#' @description Returns a list (slots are named by the input host names) with lookup results per slot
#' @param hostname vector of host names (character - FQDN)
#' @return list
#' @export
#' @examples
#' gethostsbyname(c("google.com", "stackoverflow.com", "twitter.com", "r-bloggers.com"))
#'
gethostsbyname <- Vectorize(gethostbyname, SIMPLIFY=FALSE)


#
# non-vectorized version of validateIP
#
.validateIP <- function(ip) {

  res <- regexpr('^(((2(5[0-5]|[0-4][0-9])|[01]?[0-9][0-9]?)\\.){3}(2(5[0-5]|[0-4][0-9])|[01]?[0-9][0-9]?))$', ip)
  return(min(res) > 0)

}

#' @title Validate IPv4 addresses in dotted-decimal notation
#' @description Input addresses are considered valid if they consist of 1-4 octets separated by periods
#' @param ip vector of IPv4 address (character - dotted-decimal)
#' @return named logical vector
#' @export
#' @examples
#' validateIP("127.0.0.1")
#' validateIP("127.0")
#' validateIP(c("127.0.0.1", "255.255.255.255", "500.1.1.1", "30.2.2"))
#'
validateIP <- Vectorize(.validateIP)

#
# non-vectorized version of validateCIDR
#
.validateCIDR <- function(cidr) {

  res <- regexpr('^(((2(5[0-5]|[0-4][0-9])|[01]?[0-9][0-9]?)\\.){3}(2(5[0-5]|[0-4][0-9])|[01]?[0-9][0-9]?)(/(3[012]|[12]?[0-9])))$', cidr, perl=TRUE)
  return(min(res) > 0)

}

#' @title Validate IPv4 CIDRs in dotted-decimal slash notation
#' @description Input CIDRs are considered valid if they consist of a valid IPv4 address in dotted-quad format followed by a forward slash (/) and a bit mask length (1-32)
#' @param cidr vector of IPv4 CIDRs (character - dotted-decimal slash notation)
#' @return named logical vector
#' @export
#' @examples
#' validateIP("127.0.0.1/32")
#' validateIP("127.0/8")
#' validateIP(c("127.0.0.256/32", "127.0.0.0", "127.0.0.1/33", "127.0.0.1/32"))
#'
validateCIDR <- Vectorize(.validateCIDR)

#
# non-vectorized version of validateIP
#
.validateIP <- function(ip) {

  res <- regexpr('^(((2(5[0-5]|[0-4][0-9])|[01]?[0-9][0-9]?)\\.){3}(2(5[0-5]|[0-4][0-9])|[01]?[0-9][0-9]?))$', ip)
  return(min(res) > 0)

}

#' @title Converts an IPv4 CIDR (e.g. "192.168.1.0/24") to a vector of individual IPv4 addresses
#' @description Returns a character vector of IPv4 addresses within the specified CIDR
#' @param cidr (chr) IPv4 CIDR
#' @return character vector or NULL if invalid CIDR
#' @export
cidr_ips <- function(cidr) {
  cr <- cidr_range(cidr)
  if (!any(is.na(cr))) {
    sapply(cr[1]:cr[2], long2ip)
  }
}

#' @title Generate random sequence of IPv4 addresses
#' @description Returns a character vector of randomly generated (but valid) IPv4 addresses
#' @param size number of IP addreses to generate
#' @return character vector
#' @export
#' @examples
#' set.seed(1492)
#' randomIPs(10)
#'
randomIPs <- function(size) {

  v1 <- c(16777216, 2130706431)
  long2ip(c(runif(size, v1[1], v1[2])))

}



