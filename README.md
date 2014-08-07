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

    ##  [1] "2607:f8b0:4006:807::1009" "173.194.43.34"           
    ##  [3] "173.194.43.40"            "173.194.43.35"           
    ##  [5] "173.194.43.38"            "173.194.43.39"           
    ##  [7] "173.194.43.41"            "173.194.43.37"           
    ##  [9] "173.194.43.33"            "173.194.43.32"           
    ## [11] "173.194.43.36"            "173.194.43.46"

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

    ## 2607:f8b0:4006:807::1009            173.194.43.34            173.194.43.40 
    ##                    FALSE                     TRUE                     TRUE 
    ##            173.194.43.35            173.194.43.38            173.194.43.39 
    ##                     TRUE                     TRUE                     TRUE 
    ##            173.194.43.41            173.194.43.37            173.194.43.33 
    ##                     TRUE                     TRUE                     TRUE 
    ##            173.194.43.32            173.194.43.36            173.194.43.46 
    ##                     TRUE                     TRUE                     TRUE

``` {.r}
validateCIDR("8.0.0.0/8")
```

    ## 8.0.0.0/8 
    ##      TRUE
