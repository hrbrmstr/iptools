Easy IP address handling with iptools
---------------------------------------

`iptools` is a set of tools for a working with IP addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

### Functionality
The package primarily supports IPv4 addresses due to deficiencies in R's support for large numbers, but there is IPv6 support for some functionality, and we plan to build more in as R improves and as we do. Functionality includes:

-   Converting IP addresses to their numeric form, and then back to strings, with `ip_to_numeric` and `numeric_to_ip`;
-   Validating and classifying IP addresses with `ip_classify`;
-   Range generation and checking with `range_boundaries`, `range_generate` and `validate_range`, and;
-   Several inbuilt IP-related datasets.

For more information, see the vignettes on [the functionality](https://github.com/hrbrmstr/iptools/blob/master/vignettes/introduction_to_iptools.Rmd) and [the datasets](https://github.com/hrbrmstr/iptools/blob/master/vignettes/iptools_datasets.Rmd) within `iptools`.

### Installation

To install the development version:

```
devtools::install_github("hrbrmstr/iptools")
```

`iptools` depends on the Boost library, which can be obtained on linux distributions with:

```
sudo apt-get install libboost-all-dev
```

and on `homebrew` (Mac OSX) with:

```
brew install boost
```

`iptools` does not currently work on Windows; the first person(s) to get this working there gets a free copy of [Bob's book](http://dds.ec/amzn).