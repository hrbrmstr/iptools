#' @title IANA IPv4 Address Space Registry
#' @description IANA IPv4 Address Space Registry
#' \itemize{
#'   \item \code{Prefix}. A block of IPv4 (CIDR notation) addresses that has been registered for a special purpose. (chr) (e.g. "100.64.0.0/10")
#'   \item \code{Designation}. entity the block is assigned to (chr)
#'   \item \code{Date}. assignment date (chr)
#'   \item \code{Whois}. whois registry (chr) [whois.afrinic.net|whois.apnic.net|whois.arin.net|whois.lacnic.net|whois.ripe.net]
#'   \item \code{Status}. status of the assignment (chr) [ALLOCATED|LEGACY|RESERVED]
#' }
#'
#' @docType data
#' @keywords datasets
#' @name ianaipv4assignments
#' @seealso \itemize{
#'   \item IANA - \url{http://www.iana.org/assignments/ipv4-address-space/ipv4-address-space.xhtml}
#'   \item RFC1466 - \url{http://tools.ietf.org/html/rfc1466},
#' }
#' @usage data(ianaipv4assignments)
#' @note Last updated 2014-08-07
#' @format A data frame with 256 rows and 5 variables
NULL
