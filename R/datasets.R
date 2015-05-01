#' @title IANA IPv4 Address Space Registry
#' @description This dataset contains the registry of address space assignments
#' for IPv4 IP addresses, as set by IANA. It consists of a data.frame containing the columns:
#'
#' \itemize{
#'   \item \code{prefix}: A block of IPv4 (CIDR notation) addresses that has been registered
#'   for a particular purpose (e.g. "100.64.0.0/10")
#'   \item \code{designation}: The entity the block is assigned to.
#'   \item \code{date}: the assignment date of the block, stored as YYYY-MM.
#'   \item \code{whois}: whois registry [\code{whois.afrinic.net}|\code{whois.apnic.net}|\code{whois.arin.net}|\code{whois.lacnic.net}|\code{whois.ripe.net}]
#'   \item \code{status}: status of the assignment [\code{ALLOCATED}|\code{LEGACY}|\code{RESERVED}]
#' }
#'
#' @docType data
#' @keywords datasets
#' @name iana_assignments
#'
#' @seealso \code{\link{iana_assignment_refresh}} for updating the dataset, and
#' \code{\link{iana_special_assignments}} for particular, special IPv4 assignments.
#'
#' @references
#'
#' \itemize{
#'   \item The \href{http://www.iana.org/assignments/ipv4-address-space/ipv4-address-space.xhtml}{
#'   IANA page} on the IPv4 assignments.
#'   \item \href{http://tools.ietf.org/html/rfc1466}{RFC1466}.
#'
#' }
#' @usage data(iana_assignments)
#' @note Last updated 2015-05-01.
#' @format A data frame with 256 rows and 5 variables
NULL

#' @title IANA IPv4 Special-Purpose Address Registry
#' @description This dataset contains the registry of special address space assignments
#' for IPv4 IP addresses, as set by IANA. It consists of a data.frame containing the columns:
#' \itemize{
#'   \item \code{address_block}: the full IPv4 range (chr) (e.g. "11.0.0.0/8")
#'   \item \code{name}: the descriptive name for the special-purpose address block
#'   \item \code{rfc}: the Request for Comment (RfC) through which the special-purpose address block was requested
#'   \item \code{allocation_date}: the allocation date of the block, stored as YYYY-MM.
#'   \item \code{source}: whether an address from the allocated special-purpose address block is valid
#'   when used as the source address of an IP datagram that transits two devices (\code{TRUE}) or not (\code{FALSE}).
#'   \item \code{destination}: whether an address from the allocated special-purpose address block is valid when
#'   used as the destination address of an IP datagram that transits two devices (\code{TRUE}) or not (\code{FALSE}).
#'   \item \code{forwardable}: whether a router may forward an IP datagram whose destination address is drawn from the
#'   allocated special-purpose address block between external interfaces (\code{TRUE}) or not (\code{FALSE}).
#'   \item \code{global}: whether an IP datagram whose destination address is drawn from the allocated special-purpose
#'   address block is forwardable beyond a specified administrative domain (\code{TRUE}) or not (\code{FALSE}).
#'   \item \code{reserved_by_protocol}. whether the special-purpose address block is reserved by IP, itself.
#'   This value is \code{TRUE} if the RFC that created the special-purpose address block requires all compliant IP
#'   implementations to behave in a special way when processing packets either to or from addresses contained by
#'   the address block, and \code{FALSE} otherwise.
#' }
#'
#' @docType data
#' @keywords datasets
#' @name iana_special_assignments
#'
#' @seealso \code{\link{iana_special_assignments_refresh}} to refresh this dataset,
#' and \code{\link{iana_assignments}} for a dataset covering general (non-special) IPv4 assignments.
#'
#' @references \itemize{
#'   \item The \href{http://www.iana.org/assignments/ipv4-address-space/ipv4-address-space.xhtml}{
#'   IANA page} on specially assigned blocks.
#'   \item \href{http://tools.ietf.org/html/rfc5376}{RFC5376}
#'   \item \href{http://tools.ietf.org/html/rfc6890}{RFC6890}
#' }
#' @usage data(ianaipv4spar)
#' @note Last updated 2014-08-07
#' @format A data frame with 256 rows and 5 variables
NULL

#' @title IANA Service Name and Transport Protocol Port Number Registry
#' @description IANA Service Name and Transport Protocol Port Number Registry
#' \itemize{
#'   \item \code{Service.Name}. service name for the port assignment (chr)
#'   \item \code{Port.Number}. port(s) assigned (chr) - can be a range (e.g. 6629-6630)
#'   \item \code{Transport.Protocol}. transport protocol(s) of the port assignment (chr) - [\code{dccp}|\code{sctp}|\code{tcp}|\code{udp}]
#'   \item \code{Description}. explanation of the port assignment (chr)
#'   \item \code{Assignee}. Name and email address of the party to whom the assignment is made (chr)
#'   \item \code{Contact}. Name and email address of the Contact person for the assignment (chr)
#'   \item \code{Registration.Date}. date the port was registered (chr)
#'   \item \code{Modification.Date}. date of any modification to record (chr)
#'   \item \code{Reference}. A description of (or a reference to a document describing) the protocol or application using this port (chr)
#'   \item \code{Known.Unauthorized.Uses}. A list of uses by applications or organizations who are not the Assignee (chr)
#'   \item \code{Assignment.Notes}. Indications of owner/name change, or any other assignment process issue. (chr)
#' }
#'
#' @docType data
#' @keywords datasets
#' @name ianaports
#' @seealso \itemize{
#'   \item IANA - \url{http://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml}
#'   \item RFC6335 - \url{http://tools.ietf.org/html/rfc6335},
#' }
#' @usage data(ianaports)
#' @note Last updated 2014-08-07
#' @format A data frame with 13,527 rows and 12 variables
NULL

#' @title IANA Protocol Numbers
#' @description IANA Protocol Numbers
#' \itemize{
#'   \item \code{Decimal}. protocol number (chr)
#'   \item \code{Keyword}. short name/acronym of the protocol name (chr)
#'   \item \code{Protocol}. expanded name of the protocol (chr)
#'   \item \code{IPv6.Extension.Header}. has an IPv4 extension header (chr)
#' }
#'
#' @docType data
#' @keywords datasets
#' @name ianaprotocolnumbers
#' @seealso \itemize{
#'   \item IANA - \url{http://www.iana.org/domains/root/db}
#'   \item RFC5237 - \url{http://www.iana.org/go/rfc5237}
#'   \item RFC7045 - \url{http://www.iana.org/go/rfc7045}
#' }
#' @usage data(ianaprotocolnumbers)
#' @note Last updated 2014-08-10
#' @format A data frame with 148 rows and 4 variables
NULL

#' @title IANA Root Zone Database
#' @description IANA Root Zone Database
#' \itemize{
#'   \item \code{Domain}. top level domain name in dot-tld format (chr) (e.g. "\code{.active}")
#'   \item \code{Type}. type of TLD (chr) - one of [\code{country-code}|\code{generic}|\code{generic-restricted}|\code{infrastructure}|\code{sponsored}|\code{test}]
#'   \item \code{Sponsoring.Organisation}. name of the entity sponsoring the TLD (chr)
#' }
#'
#' @docType data
#' @keywords datasets
#' @name ianarootzonetlds
#' @seealso \itemize{
#'   \item IANA - \url{http://www.iana.org/domains/root/db}
#' }
#' @usage data(ianarootzonetlds)
#' @note Last updated 2014-08-09
#' @format A data frame with 679 rows and 3 variables
NULL
