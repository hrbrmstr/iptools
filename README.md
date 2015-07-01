<!-- README.md is generated from README.Rmd. Please edit that file -->
[![Project Status: Active - The project has reached a stable, usable state and is being actively developed.](http://www.repostatus.org/badges/0.1.0/active.svg)](http://www.repostatus.org/#active) [![CRAN\_Status\_Badge](http://www.r-pkg.org/badges/version/iptools)](http://cran.r-project.org/web/packages/iptools) ![downloads](http://cranlogs.r-pkg.org/badges/grand-total/iptools)

Easy IP address handling with iptools
-------------------------------------

`iptools` is a set of tools for a working with IP addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

Available [on CRAN](http://cran.r-project.org/web/packages/iptools/)

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

`iptools` depends on the Boost library, which can be obtained on most linux distributions with:

    sudo apt-get install libboost-all-dev

or

    sudo yum install boost-devel

and on `homebrew` (Mac OSX) with:

    brew install boost

`iptools` does not currently work on Windows; the first person(s) to get this working there gets a free copy of [Bob's book](http://dds.ec/amzn).

### Test Results

``` r
library(iptools)
library(testthat)

date()
#> [1] "Wed Jul  1 19:19:07 2015"

test_dir("tests/")
#> Test IP generators : ......
#> Test IP to hostname and hostname to IP resolution : ..................
#> Test IP conversion : ..................
#> Ensure that ip_in_range and range_boundaries work : ...........................
#> Test range and IP validation : .............
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.
