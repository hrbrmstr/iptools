`iptools` is a set of tools for a working with IPv4 addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

The package also uses the v1 [GeoLite](http://dev.maxmind.com/geoip/legacy/geolite/) MaxMind library to perform basic geolocation of a given IPv4 address. You must manually install both the maxmind library (`brew install geoip` on OS X, `sudo apt-get install libgeoip-dev` on Ubuntu) and the `GeoLiteCity.dat` file [] for the geolocation functions to work.

The following functions are implemented:

-   `gethostbyaddr` - Returns all 'PTR' records associated with an IPv4 address
-   `gethostbyname` - Returns all 'A' records associated with a hostname
-   `ip2long` - Character (dotted-decimal) IPv4 Address Conversion to long integer
-   `iptools` - A package to help perform various tasks with/on IPv4 addresses
-   `long2ip` - Intger IPv4 Address Conversion to Character
-   `validateIP` - Validate IPv4 addresses in dotted-decimal notation
-   `validateCIDR` - Validate IPv4 CIDRs in dotted-decimal slash notation
-   `geoip` - Perform (local) maxmind geolocation on IPv4 addresses (see `?geoip` for details)
-   `randomIPs` - generate a vector of valid, random IPv4 addresses

The following data sets are included:

-   `ianaports` - IANA Service Name and Transport Protocol Port Number Registry
-   `ianaipv4spar` - IANA IPv4 Special-Purpose Address Registry
-   `ianaipv4assignments` - IANA IPv4 Address Space Registry
-   `ianarootzonetlds` - IANA Root Zone Database

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

    ## [1] '0.1.4'

``` {.r}
# lookup google
gethostbyname("google.com")
```

    ##  [1] "2607:f8b0:4006:807::1008" "74.125.226.8"            
    ##  [3] "74.125.226.4"             "74.125.226.6"            
    ##  [5] "74.125.226.9"             "74.125.226.0"            
    ##  [7] "74.125.226.7"             "74.125.226.3"            
    ##  [9] "74.125.226.2"             "74.125.226.5"            
    ## [11] "74.125.226.1"             "74.125.226.14"

``` {.r}
# lookup apple (in reverse)
gethostbyaddr("17.178.96.59")
```

    ## [1] "darwinsourcecode.com"

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

    ## 2607:f8b0:4006:807::1008             74.125.226.8             74.125.226.4 
    ##                    FALSE                     TRUE                     TRUE 
    ##             74.125.226.6             74.125.226.9             74.125.226.0 
    ##                     TRUE                     TRUE                     TRUE 
    ##             74.125.226.7             74.125.226.3             74.125.226.2 
    ##                     TRUE                     TRUE                     TRUE 
    ##             74.125.226.5             74.125.226.1            74.125.226.14 
    ##                     TRUE                     TRUE                     TRUE

``` {.r}
validateCIDR("8.0.0.0/8")
```

    ## 8.0.0.0/8 
    ##      TRUE

``` {.r}
# geo
geofile()
set.seed(1492)
geoip(randomIPs(25))
```

    ## Error retrieving record
    ## Error retrieving record
    ## Error retrieving record

    ##                 ip country.code country.code3       country.name region
    ## 1   71.238.158.182           US           USA      United States     MI
    ## 2     55.148.38.65           US           USA      United States     AZ
    ## 3    46.142.80.133           DE           DEU            Germany     NA
    ## 4    27.87.221.128           JP           JPN              Japan     NA
    ## 5   13.244.173.130           US           USA      United States     CT
    ## 6     2.37.185.173           IT           ITA              Italy     NA
    ## 7   213.247.175.72           RU           RUS Russian Federation     NA
    ## 8                                                                      
    ## 9    96.54.235.149           CA           CAN             Canada     BC
    ## 10 189.243.201.143           MX           MEX             Mexico     NA
    ## 11  157.173.28.232           GB           GBR     United Kingdom     NA
    ## 12 156.103.109.206           US           USA      United States     TX
    ## 13    47.27.90.166           CA           CAN             Canada     ON
    ## 14   126.6.129.152           JP           JPN              Japan     NA
    ## 15                                                                     
    ## 16 190.180.116.155           BO           BOL            Bolivia     NA
    ## 17   29.189.160.68           US           USA      United States     OH
    ## 18   82.39.232.118           GB           GBR     United Kingdom     B8
    ## 19                                                                     
    ## 20   217.104.27.16           NL           NLD        Netherlands     NA
    ## 21   150.56.113.79           JP           JPN              Japan     NA
    ## 22  89.200.243.215           RO           ROU            Romania     NA
    ## 23  31.246.182.148           DE           DEU            Germany     NA
    ## 24   139.25.48.200           DE           DEU            Germany     02
    ## 25   20.31.144.154           US           USA      United States     VA
    ##         region.name          city postal.code latitude longitude
    ## 1          Michigan   Grand Blanc       48439    42.92  -83.6496
    ## 2           Arizona Fort Huachuca       85613    31.53 -110.3607
    ## 3                NA            NA          NA    51.00    9.0000
    ## 4                NA            NA          NA    35.69  139.6900
    ## 5       Connecticut       Norwalk       06850    41.13  -73.4416
    ## 6                NA            NA          NA    42.83   12.8333
    ## 7                NA            NA          NA    60.00  100.0000
    ## 8                                                 0.00    0.0000
    ## 9  British Columbia       Nanaimo         V9R    49.14 -123.9483
    ## 10               NA            NA          NA    19.43  -99.1300
    ## 11               NA            NA          NA    51.50   -0.1300
    ## 12            Texas       Houston       77002    29.75  -95.3670
    ## 13          Ontario        Ottawa         K2H    45.32  -75.8370
    ## 14               NA            NA          NA    35.69  139.6900
    ## 15                                                0.00    0.0000
    ## 16               NA            NA          NA   -17.00  -65.0000
    ## 17             Ohio      Columbus       43218    39.96  -82.9988
    ## 18          Bromley         Hayes        UB4B    51.37    0.0167
    ## 19                                                0.00    0.0000
    ## 20               NA            NA          NA    52.50    5.7500
    ## 21               NA            NA          NA    35.69  139.6900
    ## 22               NA            NA          NA    46.00   25.0000
    ## 23               NA            NA          NA    51.00    9.0000
    ## 24           Bayern        Munich          NA    48.15   11.5833
    ## 25         Virginia  Falls Church       22042    38.86  -77.1922
    ##            time.zone metro.code area.code
    ## 1   America/New_York        513       810
    ## 2    America/Phoenix        789       520
    ## 3      Europe/Berlin          0         0
    ## 4         Asia/Tokyo          0         0
    ## 5   America/New_York        501       203
    ## 6        Europe/Rome          0         0
    ## 7                 NA          0         0
    ## 8                             0         0
    ## 9  America/Vancouver          0         0
    ## 10                NA          0         0
    ## 11     Europe/London          0         0
    ## 12   America/Chicago        618       713
    ## 13   America/Toronto          0         0
    ## 14        Asia/Tokyo          0         0
    ## 15                            0         0
    ## 16    America/La_Paz          0         0
    ## 17  America/New_York        535       614
    ## 18     Europe/London          0         0
    ## 19                            0         0
    ## 20  Europe/Amsterdam          0         0
    ## 21        Asia/Tokyo          0         0
    ## 22  Europe/Bucharest          0         0
    ## 23     Europe/Berlin          0         0
    ## 24     Europe/Berlin          0         0
    ## 25  America/New_York        511       703

### Test Results

``` {.r}
library(iptools)
library(testthat)
test_dir("tests/")
```

    ## Host/IPv4 resolution : ..
    ## IPv4 string/integer conversion : ....
    ## IPv4/CIDR validation : ......
