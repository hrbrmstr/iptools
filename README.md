<!-- README.md is generated from README.Rmd. Please edit that file -->
[![Travis-CI Build Status](https://travis-ci.org/hrbrmstr/iptools.svg?branch=master)](https://travis-ci.org/hrbrmstr/iptools) [![Project Status: Active - The project has reached a stable, usable state and is being actively developed.](http://www.repostatus.org/badges/0.1.0/active.svg)](http://www.repostatus.org/#active) [![CRAN\_Status\_Badge](http://www.r-pkg.org/badges/version/iptools)](http://cran.r-project.org/web/packages/iptools) ![downloads](http://cranlogs.r-pkg.org/badges/grand-total/iptools)

Easy IP address handling with iptools
-------------------------------------

`iptools` is a set of tools for a working with IP addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

Available [on CRAN](http://cran.r-project.org/web/packages/iptools/)

### NEW FOR 0.3.0.x!

Thanks to @eddelbuettel we've thinned down this version of `iptools` and it should also work on Windows now! Until `AsioHeaders` is in CRAN, please do the following to test this version:

    devtools::install_github("eddelbuettel/asioheaders")
    devtools::install_github("hrbrmstr/iptools")

### Functionality

The package primarily supports IPv4 addresses due to deficiencies in R's support for large numbers, but there is IPv6 support for some functionality, and we plan to build more in as R improves and as we do. Functionality includes:

-   Converting IP addresses to their numeric form, and then back to strings, with `ip_to_numeric` and `numeric_to_ip`;
-   Validating and classifying IP addresses with `ip_classify`;
-   Range generation and checking with `range_boundaries`, `range_generate` and `validate_range`, and;
-   Several inbuilt IP-related datasets.

For more information, see the vignettes on [the functionality](https://github.com/hrbrmstr/iptools/blob/master/vignettes/introduction_to_iptools.Rmd) and [the datasets](https://github.com/hrbrmstr/iptools/blob/master/vignettes/iptools_datasets.Rmd) within `iptools`.

### Installation

To install the development version:

``` r
devtools::install_github("hrbrmstr/iptools")
# or
install.packages("iptools")
```

`iptools` depends on the [Boost](http://boost.org) library, which can be obtained on most linux distributions with:

    sudo apt-get install libboost-all-dev

or

    sudo yum install boost-devel

and on `homebrew` (Mac OSX) with:

    brew install boost

`iptools` does not currently work easily on Windows; It *may* be possible to get it working with a solid, separate build of Boost for Windows under cygwin and setting environment variables properly. The first person(s) to get this working under Windows gets a free copy of [Bob's book](http://dds.ec/amzn).

### Test Results

``` r
library(iptools)
library(testthat)

date()
#> [1] "Wed Dec 30 14:24:02 2015"

test_dir("tests/")
#> testthat results ===========================================================
#> OK: 64 SKIPPED: 0 FAILED: 0
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.
