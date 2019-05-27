#' Convert an vector of IPv6 address strings to nibbles
#'
#' @param x a vector of IPv6 address strings
#' @param ip6_arpa tack on a trailing "`.ip6.arpa.`"
#' @export
#' @examples
#' c("2001:db8::1",
#'   "2001:e42:101:2:202:181:99:52",
#'   "2400:8500:1801:417:118:27:35:213",
#'   "x",
#'   "2a02:2770:8:0:21a:4aff:fe96:7a47",
#'   "2400:2413:32c0:8:21a:92ff:fe22:c7b3",
#'   "2001:44b8:3138:c570:250:56ff:fe9c:c19b",
#'   "240f:a2:2e5:1:214:c2ff:fec8:1673",
#'   "2001:e42:102:1103:153:121:36:109",
#'   "2401:2500:203:2f:153:127:108:158"
#' ) -> tst6
#'
#'
#' ipv6_to_nibble(tst6)
#'
#' ipv6_to_nibble(tst6, ip6_arpa = TRUE)
ipv6_to_nibble <- function(x, ip6_arpa = FALSE) {

  b <- ipv6_to_bytes(x)
  m <- stri_list2matrix(b, byrow = TRUE)

  apply(m, 1, function(.x) {
    if (is.na(.x[1])) return(NA_character_)
    tmp <- paste0(rev(unlist(strsplit(.x, ""))), collapse = ".")
    if (ip6_arpa) sprintf("%s.ip6.arpa.", tmp) else tmp
  })

}




