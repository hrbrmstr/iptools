#' Convert a \emph{pyasn} generated CIDR dat to a trie
#'
#' @param asn_table_file filename of dat file (can be gzip'd)
#' @export
asn_table_to_trie <- function(asn_table_file) {

  rip <- readr::read_tsv(asn_table_file, comment=";", col_names=c("cidr", "asn"))
  rip %>%
    tidyr::separate(cidr, c("ip", "mask"), "/") %>%
    dplyr::mutate(prefix=stringi::stri_sub(ip_to_binary_string(ip), 1, mask)) -> rip_df

  triebeard::trie(rip_df$prefix, rip_df$asn)

}

#' Match IP addresses to autonomous systems
#'
#' @param cidr_trie trie created with \code{asn_table_to_trie()}
#' @param ip character vector or numeric vector of IPv4 addresses
#' @export
ip_to_asn <- function(cidr_trie, ip) {

  if (inherits(ip, "numeric")) {
    ip <- ip_numeric_to_binary_string(ip)
  } else {
    ip <- ip_to_binary_string(ip)
  }

  triebeard::longest_match(cidr_trie, ip)

}

#' Determine if a vector if IPv4 addresses are in a vector of CIDRs
#'
#' @param ips character vector or numeric vector of IPv4 addresses
#' @param cidrs character vector or numeric vector of IPv4 CIDRs
#' @note auto-appends \code{/32} if a bare IPv4 is detected
#' @return \code{data_frame} with \code{ips} column and a logical \code{in_cdir} column
#' @export
ips_in_cidrs <- function(ips, cidrs) {

  cidrs[!grepl("/", cidrs)] <- sprintf("%s/32", cidrs[!grepl("/", cidrs)])

  dplyr::data_frame(cidr=cidrs) %>%
    tidyr::separate(cidr, c("ip", "mask"), "/") %>%
    dplyr::mutate(prefix=stri_sub(ip_to_binary_string(ip), 1, mask),
                  value=TRUE) -> tr

  cidr_trie <- trie(tr$prefix, tr$value)

  dplyr::data_frame(ips=ips,
                    in_cidr=longest_match(cidr_trie,
                                          ip_to_binary_string(ips))) %>%
    mutate(in_cidr=!is.na(in_cidr))

}
