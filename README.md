`iptools` is a set of tools for a working with IPv4 addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

The package also uses the v1 [GeoLite](http://dev.maxmind.com/geoip/legacy/geolite/) MaxMind library to perform basic geolocation of a given IPv4 address. You must manually install both the maxmind library (`brew install geoip` on OS X, `sudo apt-get install libgeoip-dev` on Ubuntu) and the `GeoLiteCity.dat` [<http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz>](http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz) & `GeoLiteASNum.dat` [<http://geolite.maxmind.com/download/geoip/database/GeoLiteASNum.dat.gz>](http://geolite.maxmind.com/download/geoip/database/GeoLiteASNum.dat.gz) files for the geolocation/ASN functions to work. If there's interest in porting to the newer library/GeoLite2 format, I'll consider updating the package.

The following functions are implemented:

-   `gethostbyaddr` - Returns all `PTR` records associated with an IPv4 address
-   `gethostbyname` - Returns all `A` records associated with a hostname
-   `gethostsbyaddr` - Vectorized version of
-   `gethostsbyname` - Vectorized version of
-   `ip2long` - Character (dotted-decimal) IPv4 Address Conversion to long integer
-   `iptools` - A package to help perform various tasks with/on IPv4 addresses
-   `long2ip` - Intger IPv4 Address Conversion to Character
-   `validateIP` - Validate IPv4 addresses in dotted-decimal notation
-   `validateCIDR` - Validate IPv4 CIDRs in dotted-decimal slash notation
-   `geoip` - Perform (local) maxmind geolocation on IPv4 addresses (see `?geoip` for details)
-   `asnip` - Perform (local) maxmind AS \# & org lookup on IPv4 addresses (see `?asnip` for details)
-   `randomIPs` - generate a vector of valid, random IPv4 addresses (very helpful for testing)

The following data sets are included:

-   `ianaports` - IANA Service Name and Transport Protocol Port Number Registry
-   `ianaipv4spar` - IANA IPv4 Special-Purpose Address Registry
-   `ianaipv4assignments` - IANA IPv4 Address Space Registry
-   `ianarootzonetlds` - IANA Root Zone Database
-   `ianarootzonetlds` - IANA Protocol Numbers

### Installation

``` {.r}
devtools::install_git("https://gitlab.dds.ec/bob.rudis/iptools.git")
```

> NOTE: Under Ubuntu (it probably applies to other variants), this only works with the current version (1.55) of the boost library, which I installed via the [launchpad boost-latest](https://launchpad.net/~boost-latest/+archive/ubuntu/ppa/+packages) package:

    sudo add-apt-repository ppa:boost-latest/ppa
    # sudo apt-get install python-software-properties if "add-apt-repository" is not found
    sudo apt-get update
    sudo apt-get install boost1.55 # might need to use 1.54 on some systems

> `homebrew` (OS X) users can do: `brew install boost` and it should work.
>
> The first person(s) to get this working under Windows/mingw + boost/Rcpp gets a free copy of [our book](http://dds.ec/amzn)

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

    ##  [1] "2607:f8b0:4006:807::1005" "74.125.226.72"           
    ##  [3] "74.125.226.70"            "74.125.226.66"           
    ##  [5] "74.125.226.71"            "74.125.226.69"           
    ##  [7] "74.125.226.67"            "74.125.226.78"           
    ##  [9] "74.125.226.73"            "74.125.226.65"           
    ## [11] "74.125.226.68"            "74.125.226.64"

``` {.r}
# lookup apple (in reverse)
gethostbyaddr("17.178.96.59")
```

    ## [1] "ibookpartner.com"

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

    ## 2607:f8b0:4006:807::1005            74.125.226.72            74.125.226.70 
    ##                    FALSE                     TRUE                     TRUE 
    ##            74.125.226.66            74.125.226.71            74.125.226.69 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.67            74.125.226.78            74.125.226.73 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.65            74.125.226.68            74.125.226.64 
    ##                     TRUE                     TRUE                     TRUE

``` {.r}
validateCIDR("8.0.0.0/8")
```

    ## 8.0.0.0/8 
    ##      TRUE

``` {.r}
# geo
maxmindinit()
set.seed(1492)
geoip(randomIPs(25))
```

    ##                 ip country.code country.code3       country.name region
    ## 1   71.238.158.182           US           USA      United States     MI
    ## 2     55.149.38.66           US           USA      United States     AZ
    ## 3    47.143.81.134           CA           CAN             Canada     ON
    ## 4    28.88.224.129           US           USA      United States     OH
    ## 5   13.248.176.132           US           USA      United States     CT
    ## 6     2.37.188.176           IT           ITA              Italy     NA
    ## 7   218.253.179.74           HK           HKG          Hong Kong     NA
    ## 8   130.216.63.230           NZ           NZL        New Zealand     NA
    ## 9    99.55.243.154           US           USA      United States     MI
    ## 10 196.251.208.148         <NA>          <NA>               <NA>   <NA>
    ## 11  164.180.29.241           US           USA      United States     OH
    ## 12 163.107.114.215           FR           FRA             France     NA
    ## 13    49.28.94.174           KR           KOR Korea, Republic of     11
    ## 14   133.6.136.160           JP           JPN              Japan     NA
    ## 15   133.79.52.183           JP           JPN              Japan     NA
    ## 16 202.191.124.165           BD           BGD         Bangladesh     81
    ## 17   31.202.171.73           UA           UKR            Ukraine     07
    ## 18   87.42.248.127           IE           IRL            Ireland     NA
    ## 19  169.100.123.87           US           USA      United States     NY
    ## 20   234.113.30.18         <NA>          <NA>               <NA>   <NA>
    ## 21   163.61.123.85         <NA>          <NA>               <NA>   <NA>
    ## 22   97.218.98.234           US           USA      United States     NA
    ## 23   33.30.199.162           US           USA      United States     OH
    ## 24   153.28.53.220           US           USA      United States     OH
    ## 25   23.34.158.170           US           USA      United States     MA
    ##            region.name          city postal.code latitude longitude
    ## 1             Michigan   Grand Blanc       48439    42.92    -83.65
    ## 2              Arizona Fort Huachuca       85613    31.53   -110.36
    ## 3              Ontario        Ottawa         K2H    45.32    -75.84
    ## 4                 Ohio      Columbus       43218    39.96    -83.00
    ## 5          Connecticut       Norwalk       06850    41.13    -73.44
    ## 6                   NA            NA          NA    42.83     12.83
    ## 7                   NA            NA          NA    22.25    114.17
    ## 8                   NA            NA          NA   -41.00    174.00
    ## 9             Michigan       Portage       49024    42.20    -85.62
    ## 10                <NA>          <NA>        <NA>       NA        NA
    ## 11                Ohio      Columbus       43218    39.96    -83.00
    ## 12                  NA            NA          NA    48.86      2.35
    ## 13    Seoul-t'ukpyolsi         Seoul          NA    37.60    126.98
    ## 14                  NA            NA          NA    35.69    139.69
    ## 15                  NA            NA          NA    35.69    139.69
    ## 16               Dhaka         Dhaka          NA    23.72     90.41
    ## 17 Kharkivs'ka Oblast'       Kharkiv          NA    49.98     36.25
    ## 18                  NA            NA          NA    53.00     -8.00
    ## 19            New York      New York       10271    40.71    -74.01
    ## 20                <NA>          <NA>        <NA>       NA        NA
    ## 21                <NA>          <NA>        <NA>       NA        NA
    ## 22                  NA            NA          NA    38.00    -97.00
    ## 23                Ohio      Columbus       43218    39.96    -83.00
    ## 24                Ohio      Columbus       43218    39.96    -83.00
    ## 25       Massachusetts     Cambridge       02142    42.36    -71.08
    ##            time.zone metro.code area.code
    ## 1   America/New_York        513       810
    ## 2    America/Phoenix        789       520
    ## 3    America/Toronto          0         0
    ## 4   America/New_York        535       614
    ## 5   America/New_York        501       203
    ## 6        Europe/Rome          0         0
    ## 7     Asia/Hong_Kong          0         0
    ## 8                 NA          0         0
    ## 9   America/New_York        563       269
    ## 10              <NA>         NA        NA
    ## 11  America/New_York        535       614
    ## 12      Europe/Paris          0         0
    ## 13        Asia/Seoul          0         0
    ## 14        Asia/Tokyo          0         0
    ## 15        Asia/Tokyo          0         0
    ## 16        Asia/Dhaka          0         0
    ## 17 Europe/Zaporozhye          0         0
    ## 18     Europe/Dublin          0         0
    ## 19  America/New_York        501       212
    ## 20              <NA>         NA        NA
    ## 21              <NA>         NA        NA
    ## 22                NA          0         0
    ## 23  America/New_York        535       614
    ## 24  America/New_York        535       614
    ## 25  America/New_York        506       617

``` {.r}
set.seed(100000)
asnip(randomIPs(15))
```

    ##                 ip      asn                                     org
    ## 1  172.218.158.139    AS852               TELUS Communications Inc.
    ## 2    122.30.148.37   AS4713          NTT Communications Corporation
    ## 3    166.78.178.16  AS33070                       Rackspace Hosting
    ## 4    240.37.137.24     <NA>                                    <NA>
    ## 5      27.30.25.28   AS4134                                Chinanet
    ## 6  119.190.124.103   AS4837              CNCGROUP China169 Backbone
    ## 7     44.167.43.28   AS7377   University of California at San Diego
    ## 8  101.165.102.119   AS1221                         Telstra Pty Ltd
    ## 9   245.189.210.92     <NA>                                    <NA>
    ## 10   22.173.243.85     <NA>                                    <NA>
    ## 11  186.219.208.57 AS262996  TELECOMUNICACOES E INFORMATICA LTDA ME
    ## 12    66.162.92.26   AS4323               tw telecom holdings, inc.
    ## 13   86.66.214.138  AS15557 Societe Francaise du Radiotelephone S.A
    ## 14   3.220.143.125     <NA>                                    <NA>
    ## 15    187.97.8.132  AS26615                        Tim Celular S.A.

### Test Results

``` {.r}
library(iptools)
library(testthat)

date()
```

    ## [1] "Mon Aug 11 13:37:04 2014"

``` {.r}
test_dir("tests/")
```

    ## Host/IPv4 resolution : ..
    ## IPv4 string/integer conversion : ....
    ## IPv4/CIDR validation : ......
    ## Geolocation : ..
