#' Return CIDR ranges for given ISO 3166-1 alpha-2 country codes
#'
#' Query \url{http://www.iwik.org/ipcountry/} for the CIDR ranges for a given
#' set of ISO 3166-1 alpha-2 country codes and return the results in a named
#' list of character vectors.
#'
#' An internal cache of the CIDR query results are maintained as they only change
#' daily (12:00 CET). The cache can be flushed with \code{flush_country_cidrs} and
#' which codes have been cached can be retrieved with \code{cached_country_cidrs)}.
#'
#' @param countries character vector of ISO 3166-1 alpha-2 country codes (case-insensitive)
#' @return named list of character vectors of CIDR blocks
#' @note This function requires internet connectivity as it pulls daily updated data
#'       from \url{http://www.iwik.org/ipcountry/}.
#' @export
#' @examples
#' rng <- country_ranges(c("PW", "UZ"))
country_ranges <- function(countries) {

  ISO_3166_1 <- get("ISO_3166_1", envir=.pkgenv)

  countries <- toupper(countries)
  retrieve <- countries[countries %in% ISO_3166_1$Alpha_2]
  if (length(retrieve) != length(countries)) {
    warning("Skipping invalid country coides")
  }

  if (length(retrieve) > 0) {
    setNames(lapply(retrieve, get_country_cidrs), retrieve)
  } else {
    return(list())
  }

}

#' Flush the country CIDR cache
#' @export
flush_country_cidrs <- function() {
  .pkgenv$cached_country_cidrs <- list()
  return(invisible())
}

#' Inspect cached CIDR countries
#' @export
cached_country_cidrs <- function() {
  cns <- names(.pkgenv$cached_country_cidrs)
  if (length(cns) == 0) {
    message("No entries in country CIDR cache")
    return(invisible())
  } else {
    return(cns)
  }
}

#' Fetch all country CIDR blocks
#'
#' Iterates through all the country codes in \code{ISO_3166_1$Alpha_2} and returns
#' a named list of CIDR blocks for all those countries.
#'
#' @return named list of character vectors of CIDR blocks
#' @note This is an expensive operation as it pulls 249 files from
#'       \url{http://www.iwik.org/ipcountry/}. Try not to do this too often.
#' @export
get_all_country_ranges <- function() {

  ISO_3166_1 <- get("ISO_3166_1", envir=.pkgenv)

  setNames(lapply(ISO_3166_1$Alpha_2, get_country_cidrs), ISO_3166_1$Alpha_2)

}

# fetch CIDR blocks for a country. keeping this in a separate function
# so it's easier to swap out later if the site goes bad
get_country_cidrs <- function(cn) {

  cn_ret <- .pkgenv$cached_country_cidrs[[cn]]

  if (length(cn_ret) == 0) {

    suppressWarnings(
      cn_ret <- grep("^#",
           tryCatch(
             readLines(sprintf("http://www.iwik.org/ipcountry/%s.cidr", cn), warn=FALSE),
             error=function(err) { NA }
           ),
           invert=TRUE, value=TRUE)
    )

    .pkgenv$cached_country_cidrs[[cn]] <- cn_ret

  }

  return(cn_ret)

}