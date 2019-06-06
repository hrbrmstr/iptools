#' Create an IPv4 network from an address and prefix length.
#'
#' Given a character vector of IP addresses in CIDR notation (e.g. `1.2.3.4/24`)
#' or a character vector of IP addresses and an integer vector of prefix lengths,
#' return a character vector of the network (in CIDR notation).
#'
#' Suggested by Slava Nikitin (<https://github.com/hrbrmstr/iptools/issues/38>).
#'
#' @md
#' @param ip_addresses either a character vector of IP addresses in CIDR notation
#'        (e.g. `1.2.3.4/24`) --- in which case `prefix_lengths` should be `NULL` ---
#'        or a character vector of IP addresses --- in which case `prefix_lengths` should
#'        be a character vector of the same length as `ip_addresses`.
#' @param prefix_lengths should be `NULL` (the default) if `ip_addresses` is a
#'        a character vector of IP addresses in CIDR notation otherwise should be
#'        a character vector of the same length as `ip_addresses`.
#' @export
#' @examples
#' host_ip <- c("1.2.3.4", "4.3.2.1")
#' subnet_len <- c(24L, 25L)
#' ip_to_subnet(host_ip, subnet_len)
#' ip_to_subnet(c("1.2.3.4/24", "4.3.2.1/25"))
ip_to_subnet <- function(ip_addresses, prefix_lengths = NULL) {

  stopifnot(is.character(ip_addresses))

  if (!is.null(prefix_lengths)) {
    stopifnot(length(prefix_lengths) == length(ip_addresses))
    if (!is.integer(prefix_lengths)) prefix_lengths <- as.integer(prefix_lengths)
    stopifnot(all((prefix_lengths <= 32) & (prefix_lengths >= 0)))
  } else {
    bits <- stri_split_fixed(ip_addresses, pattern = "/", 2, simplify = TRUE)
    ip_addresses <- bits[,1]
    prefix_lengths <- suppressWarnings(as.integer(bits[,2]))
  }

  prefix_lengths[is.na(prefix_lengths)] <- 32L

  return(int_ip_to_subnet(ip_addresses, prefix_lengths))

}