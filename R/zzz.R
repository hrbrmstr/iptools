.onUnload <- function (libpath) {
  asn_fin()
  library.dynam.unload("iptools", libpath)
}

.pkgenv <- new.env(parent=emptyenv())

.onAttach <- function(...) {

  init_as_db()

  if (interactive()) {
    packageStartupMessage(paste0("This product includes GeoLite data created by ",
                                 "MaxMind, available from <http://www.maxmind.com>.",
                                 sep="", collapse="\n"))
  }

  cached_country_cidrs <- list()
  assign("cached_country_cidrs", cached_country_cidrs, envir=.pkgenv)
  data("ISO_3166_1", package="ISOcodes", envir=.pkgenv)

}