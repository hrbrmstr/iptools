#'@title generate random IPv4 IP addresses
#'@description \code{ip_random} generates random IP addresses.
#'These currently only follow IPv4 standards, since IPv6 addresses
#'are too large to be stored in R in their numeric form. All
#'IPs generated this way are valid.
#'
#'@param n the number of IP addresses to randomly generate.
#'
#'@return a character vector of randomly-generated IPv4 addresses,
#'in their dotted-decimal form.
#'
#'@seealso \code{\link{ip_to_numeric}} for converting \code{random_ips}'
#'output to its numeric form, and \code{\link{range_generate}} for
#'generating all IP addresses within a specific range.
#'
#'@examples
#'ip_random(1)
#'#[1] "49.20.57.31"
#'@export
ip_random <- function(n){
  numeric_to_ip(runif(n, 16777216, 2130706431))
}

#'@title generate all IP addresses within a range
#'@description generates a vector containing all IP addresses
#'within a provided range. Currently IPv4 only due to R's support
#'(or lack thereof) for really big numbers.
#'
#'@param range an IPv4 IP range
#'
#'@return a character vector containing each IPv4 IP address
#'within the provided range.
#'
#'@seealso \code{\link{ip_random}} for randomly-generated IPs, or
#'\code{\link{ip_to_numeric}} for converting \code{generate_range}'s
#'output to its numeric form.
#'
#'@examples
#'range_generate("172.18.0.0/28")
#'#[1]  "172.18.0.0"  "172.18.0.1"  "172.18.0.2"  "172.18.0.3"  "172.18.0.4"
#'#[6]  "172.18.0.5"  "172.18.0.6"  "172.18.0.7"  "172.18.0.8"  "172.18.0.9"
#'#[11] "172.18.0.10" "172.18.0.11" "172.18.0.12" "172.18.0.13" "172.18.0.14" "172.18.0.15"
#'
#'@export
range_generate <- function(range){
  boundaries <- unlist(range_boundaries(range))
  if(!boundaries[1] == "Invalid"){
    ips <- numeric_to_ip(seq(from = ip_to_numeric(boundaries[1]),
                             to = ip_to_numeric(boundaries[2]),
                             by = 1))
    return(ips)
  }

  stop("Invalid range")
}