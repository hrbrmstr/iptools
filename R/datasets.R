#' @title IANA IPv4 Address Space Registry
#' @description This dataset contains the registry of address space assignments
#' for IPv4 IP addresses, as set by IANA. It consists of a data.frame containing the columns:
#'
#' \itemize{
#'   \item \code{prefix}: A block of IPv4 (CIDR notation) addresses that has been registered
#'   for a particular purpose (e.g. "100.64.0.0/10")
#'   \item \code{designation}: The entity the block is assigned to.
#'   \item \code{date}: the assignment date of the block, stored as YYYY-MM.
#'   \item \code{whois}: whois registry [\code{whois.afrinic.net}|\code{whois.apnic.net}|
#'         \code{whois.arin.net}|\code{whois.lacnic.net}|\code{whois.ripe.net}]
#'   \item \code{status}: status of the assignment [\code{ALLOCATED}|\code{LEGACY}|
#'         \code{RESERVED}]
#' }
#'
#' @docType data
#' @keywords datasets
#' @name iana_assignments
#'
#' @seealso \code{\link{iana_assignments_refresh}} for updating the dataset, and
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
#' @usage data(iana_special_assignments)
#' @note Last updated 2014-08-07
#' @format A data frame with 256 rows and 5 variables
NULL

#' @title IANA Service Name and Transport Protocol Port Number Registry
#' @description This is the dataset of IANA service names and their assigned
#' ports and transport protocols - along with related metadata.
#'
#' \itemize{
#'   \item \code{service_name}: The service name for the port assignment
#'   \item \code{port_number}: The ports assigned to that service. This can be individual ports, or a range.
#'   \item \code{transport_protocol}: The transport protocol(s) of the port assignment -
#'   [\code{dccp}|\code{sctp}|\code{tcp}|\code{udp}]
#'   \item \code{description}: An explanation of the port assignment
#'   \item \code{assignee}: the name of the individual or organisation to whom the assignment is made
#'   \item \code{contact}: the name of the individual or organisation who serves as the contact person for
#'   the assignment.
#'   \item \code{registration_date}. The date the assignment was registered on. This may be empty, in the
#'   case of early assignments; otherwise, it is stored in the form "YYYY-MM".
#'   \item \code{modification_date}: The date of any modification to the assignment. Same format as
#'   \code{registration_date}
#'   \item \code{reference}: A description of (or a reference to a document describing)
#'   the protocol or application using this port
#'   \item \code{known_unauthorised_uses}: A list of uses by applications or organizations who are not the assignee
#'   \item \code{assignment_notes}. Indications of owner/name change, or any other assignment process issue
#' }
#'
#' @docType data
#' @keywords datasets
#' @name iana_ports
#'
#' @references
#' \itemize{
#'   The \href{http://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml}{IANA list}.
#'   \href{http://tools.ietf.org/html/rfc6335}{RFC6335}
#' }
#' @usage data(iana_ports)
#' @note Last updated 2015-05-02
#' @format A data frame with 13,659 rows and 12 variables
NULL