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

### Current Version

0.1.2pac

### Installation

``` {.r}
devtools::install_git("https://gitlab.dds.ec/bob.rudis/iptools.git")
```

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

    ##  [1] "2607:f8b0:4006:807::100e" "74.125.226.40"           
    ##  [3] "74.125.226.37"            "74.125.226.39"           
    ##  [5] "74.125.226.35"            "74.125.226.46"           
    ##  [7] "74.125.226.33"            "74.125.226.32"           
    ##  [9] "74.125.226.41"            "74.125.226.34"           
    ## [11] "74.125.226.38"            "74.125.226.36"

``` {.r}
# lookup apple (in reverse)
gethostbyaddr("17.178.96.59")
```

    ## [1] "desktopmovie.org"

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

    ## 2607:f8b0:4006:807::100e            74.125.226.40            74.125.226.37 
    ##                    FALSE                     TRUE                     TRUE 
    ##            74.125.226.39            74.125.226.35            74.125.226.46 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.33            74.125.226.32            74.125.226.41 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.34            74.125.226.38            74.125.226.36 
    ##                     TRUE                     TRUE                     TRUE

``` {.r}
validateCIDR("8.0.0.0/8")
```

    ## 8.0.0.0/8 
    ##      TRUE
