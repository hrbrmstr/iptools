cache_dir <- rappdirs::user_cache_dir("iptools")

asn_url <- "https://download.maxmind.com/download/geoip/database/asnum/GeoIPASNum.dat.gz"
asn_zip <- file.path(cache_dir, "GeoIPASNum.dat.gz")
asn_dat <- file.path(cache_dir, "GeoIPASNum.dat")

#' Update the autonomous system database
#'
#' Use this function to update the autonomous system database.
#' The legacy MaxMind databases update (roughly) twice a month
#'
#' @note This product includes GeoLite data created by MaxMind, available from
#'       \href{http://www.maxmind.com}{MaxMind}.
update_as_db <- function() {

  if (!file.exists(cache_dir)) {
    dir.create(cache_dir, recursive=TRUE, showWarnings=FALSE)
  }

  utils::download.file(asn_url, asn_zip, quiet=TRUE, mode="wb")

  if (!file.exists(asn_zip)) {
    stop("Error downloading MaxMind GeoLite AS database", call.=FALSE)
  }

  R.utils::gunzip(asn_zip, asn_dat, ignore.case=TRUE, overwrite=TRUE)

  if (!file.exists(asn_dat)) {
    stop("Error finding MaxMind GeoLite AS database", call.=FALSE)
  }

  asn_fin()
  asn_init(asn_dat)

}

init_as_db <- function() {

  if (!file.exists(asn_dat)) {
    update_as_db()
  } else {
    asn_init(asn_dat)
  }

}

#' Lookup autonomous system information for a vector of string IPv4 addresses
#'
#' The MaxMind GeoLite autonomous system organization database is automatically
#' downloaded if not installed. You should update the database (roughly) twice
#' a month (which is roughly how often MaxMind updates the legacy product) using
#' the \code{link{update_as_db()}} function.
#'
#' @param ips character vector of IPv4 addresses
#' @return A \code{data.frame} including \code{ips}, the numeric version of
#'         \code{ips}, the \code{as} (without the "\code{AS}" prefix and the
#'         \code{org}.
#' @note This product includes GeoLite data created by MaxMind, available from
#'       \href{http://www.maxmind.com}{MaxMind}.
#' @export
#' @examples
#' ips <- c("188.143.232.26", "193.189.117.147", "188.143.232.41",
#'          "167.114.224.11", "87.69.225.131", "14.106.226.74",
#'          "14.106.229.236", "1.227.120.42", "81.213.203.232")
#' as_info <- as_lookup(ips)
as_lookup <- function(ips) {
  dat <- asn_lookup_(ips)
  orgs <- stringi::stri_split_fixed(dat$asn_org, " ", 2, simplify=TRUE)
  orgs <- setNames(as.data.frame(orgs, stringsAsFactors=FALSE), c("as", "org"))
  orgs <- cbind.data.frame(dat[,1:2], orgs, stringsAsFactors=FALSE)
  orgs$as <- stringi::stri_replace_all_regex(orgs$as, "^AS", "")
  orgs
}
