.onUnload <- function (libpath) {
  library.dynam.unload("iptools", libpath)
}