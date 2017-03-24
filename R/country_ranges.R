# these are the 2-letter codes that http://www.iwik.org/ipcountry/ uses
# rather than do yet-another-web-call we'll update this as the world changes

c("AD", "AE", "AF", "AG", "AI", "AL", "AM", "AO", "AP", "AR",
"AS", "AT", "AU", "AW", "AX", "AZ", "BA", "BB", "BD", "BE", "BF",
"BG", "BH", "BI", "BJ", "BL", "BM", "BN", "BO", "BQ", "BR", "BS",
"BT", "BW", "BY", "BZ", "CA", "CD", "CF", "CG", "CH", "CI", "CK",
"CL", "CM", "CN", "CO", "CR", "CU", "CV", "CW", "CY", "CZ", "DE",
"DJ", "DK", "DM", "DO", "DZ", "EC", "EE", "EG", "ER", "ES", "ET",
"EU", "FI", "FJ", "FM", "FO", "FR", "GA", "GB", "GD", "GE", "GF",
"GG", "GH", "GI", "GL", "GM", "GN", "GP", "GQ", "GR", "GT", "GU",
"GW", "GY", "HK", "HN", "HR", "HT", "HU", "ID", "IE", "IL", "IM",
"IN", "IO", "IQ", "IR", "IS", "IT", "JE", "JM", "JO", "JP", "KE",
"KG", "KH", "KI", "KM", "KN", "KP", "KR", "KW", "KY", "KZ", "LA",
"LB", "LC", "LI", "LK", "LR", "LS", "LT", "LU", "LV", "LY", "MA",
"MC", "MD", "ME", "MF", "MG", "MH", "MK", "ML", "MM", "MN", "MO",
"MP", "MQ", "MR", "MS", "MT", "MU", "MV", "MW", "MX", "MY", "MZ",
"NA", "NC", "NE", "NF", "NG", "NI", "NL", "NO", "NP", "NR", "NU",
"NZ", "OM", "PA", "PE", "PF", "PG", "PH", "PK", "PL", "PM", "PR",
"PS", "PT", "PW", "PY", "QA", "RE", "RO", "RS", "RU", "RW", "SA",
"SB", "SC", "SD", "SE", "SG", "SI", "SK", "SL", "SM", "SN", "SO",
"SR", "SS", "ST", "SV", "SX", "SY", "SZ", "TC", "TD", "TG", "TH",
"TJ", "TK", "TL", "TM", "TN", "TO", "TR", "TT", "TV", "TW", "TZ",
"UA", "UG", "US", "UY", "UZ", "VA", "VC", "VE", "VG", "VI", "VN",
"VU", "WF", "WS", "YE", "YT", "ZA", "ZM", "ZW") -> iso2c

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

  countries <- toupper(countries)
  retrieve <- countries[countries %in% iso2c]
  if (length(retrieve) != length(countries)) {
    warning("Skipping invalid country codes")
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
#' Iterates through all the country codes and returns
#' a named list of CIDR blocks for all those countries.
#'
#' @return named list of character vectors of CIDR blocks
#' @note This is an expensive operation as it pulls 249 files from
#'       \url{http://www.iwik.org/ipcountry/}. Try not to do this too often.
#' @export
get_all_country_ranges <- function() {

  setNames(lapply(iso2c, get_country_cidrs), iso2c)

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