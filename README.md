`iptools` is a set of tools for a working with IPv4 addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

The following functions are implemented:

-   `gethostbyaddr` - Returns all 'PTR' records associated with an IPv4 address
-   `gethostbyname` - Returns all 'A' records associated with a hostname
-   `ip2long` - Character (dotted-decimal) IPv4 Address Conversion to long integer
-   `iptools` - A package to help perform various tasks with/on IPv4 addresses
-   `long2ip` - Intger IPv4 Address Conversion to Character
-   `validateIP` - Validate IPv4 addresses in dotted-decimal notation
-   `validateCIDR` - Validate IPv4 CIDRs in dotted-decimal slash notation

The following data sets are included:

-   `ianaports` - IANA Service Name and Transport Protocol Port Number Registry
-   `ianaipv4spar` - IANA IPv4 Special-Purpose Address Registry
-   `ianaipv4assignments` - IANA IPv4 Address Space Registry

### Installation

``` {.r}
devtools::install_git("https://gitlab.dds.ec/bob.rudis/iptools.git")
```

> NOTE: Under Ubuntu (it probably applies to other variants), this only works with the current version (1.55) of the boost library, which I installed via the [launchpad boost-latest](https://launchpad.net/~boost-latest/+archive/ubuntu/ppa/+packages) package:

    sudo add-apt-repository ppa:boost-latest/ppa
    # sudo apt-get install python-software-properties if "add-apt-repository" is not found
    sudo apt-get update
    sudo apt-get install boost1.55 # might need to use 1.54 on some systems

The first person(s) to get this working under Windows/mingw + boost/Rcpp gets a free copy of [our book](http://dds.ec/amzn)

### Usage

``` {.r}
library(iptools)

# current verison
packageVersion("iptools")
```

    ## [1] '0.1.2'

``` {.r}
# lookup google
gethostbyname("google.com")
```

    ##  [1] "2607:f8b0:4006:806::1002" "74.125.226.35"           
    ##  [3] "74.125.226.32"            "74.125.226.40"           
    ##  [5] "74.125.226.46"            "74.125.226.38"           
    ##  [7] "74.125.226.33"            "74.125.226.34"           
    ##  [9] "74.125.226.39"            "74.125.226.37"           
    ## [11] "74.125.226.41"            "74.125.226.36"

``` {.r}
# lookup apple (in reverse)
gethostbyaddr("17.178.96.59")
```

    ## [1] "universityarts.com"

``` {.r}
# decimal and back
ip2long("17.178.96.59")
```

    ## [1] 296902715

``` {.r}
long2ip(ip2long("17.178.96.59"))
```

    ## [1] "17.178.96.59"

``` {.r}
# checking it twice
validateIP(gethostbyname("google.com"))
```

    ## 2607:f8b0:4006:806::1002            74.125.226.35            74.125.226.32 
    ##                    FALSE                     TRUE                     TRUE 
    ##            74.125.226.40            74.125.226.46            74.125.226.38 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.33            74.125.226.34            74.125.226.39 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.37            74.125.226.41            74.125.226.36 
    ##                     TRUE                     TRUE                     TRUE

``` {.r}
validateCIDR("8.0.0.0/8")
```

    ## 8.0.0.0/8 
    ##      TRUE

### Test Results

``` {.r}
library(iptools)
library(testthat)
test_dir("tests/")
```

    ## Host/IPv4 resolution : ..
    ## IPv4 string/integer conversion : ....
    ## IPv4/CIDR validation : ......
