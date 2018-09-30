#' @title A package to quickly and easily handle IP addresses.
#'
#' @description A toolkit for manipulating, validating and testing 'IP' addresses and
#'  ranges, along with datasets relating to 'IP' addresses. Tools are also provided
#'  to map IPv4 blocks to country codes. While it primarily has support for the 'IPv4'
#'  address space, more extensive 'IPv6' support is intended.
#'
#' @name iptools
#' @docType package
#' @useDynLib iptools
#' @importFrom Rcpp sourceCpp
#' @import stringi
#' @import triebeard
#' @import utils
#' @import stats
#' @import AsioHeaders
#' @importFrom readr read_tsv
NULL