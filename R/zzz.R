.onUnload <- function (libpath) {
  library.dynam.unload("iptools", libpath)
}

.pkgenv <- new.env(parent=emptyenv())

.onAttach <- function(...) {

  cached_country_cidrs <- list()
  assign("cached_country_cidrs", cached_country_cidrs, envir=.pkgenv)

}