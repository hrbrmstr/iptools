# hook funtions to cleanup allocated geoip file record (if any)

.onUnload <- function(libpath) {
  geoCleanup();
}

.onDetach <- function(libpath) {
  geoCleanup();
}