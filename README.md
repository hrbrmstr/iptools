`iptools` is a set of tools for a working with IPv4 addresses. The aim is to provide functionality not presently available with any existing R package and to do so with as much speed as possible. To that end, many of the operations are written in `Rcpp` and require installation of the `Boost` libraries. A current, lofty goal is to mimic most of the functionality of the Python `iptools` module and make IP addresses first class R objects.

The package also uses the v1 [GeoLite](http://dev.maxmind.com/geoip/legacy/geolite/) MaxMind library to perform basic geolocation of a given IPv4 address. You must manually install both the maxmind library (`brew install geoip` on OS X, `sudo apt-get install libgeoip-dev` on Ubuntu) and the `GeoLiteCity.dat` <http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz> & `GeoLiteASNum.dat` <http://geolite.maxmind.com/download/geoip/database/GeoLiteASNum.dat.gz> files for the geolocation/ASN functions to work. If there's interest in porting to the newer library/GeoLite2 format, I'll consider updating the package.

The following functions are implemented:

*Revolver-ish*

-   `gethostbyaddr` - Returns all `PTR` records associated with an IPv4 address
-   `gethostsbyaddr` - Vectorized version of `gethostbyaddr`
-   `gethostbyname` - Returns all `A` records associated with a hostname
-   `gethostsbyname` - Vectorized version of `gethostbyname`

*IP int/string conversion*

-   `ip2long` - Character (dotted-decimal) IPv4 Address Conversion to long integer
-   `long2ip` - Intger IPv4 Address Conversion to Character

*CIDR*

-   `cidr_ips` - Converts an IPv4 CIDR (e.g. "192.168.1.0/24") to a vector of individual IPv4 addresses
-   `ip_in_cidr` - Test if IPv4 addresses are in a CIDR block
-   `cidr_range` - IPv4 CIDR to long integer range

*Validation*

-   `validateIP` - Validate IPv4 addresses in dotted-decimal notation
-   `validateCIDR` - Validate IPv4 CIDRs in dotted-decimal slash notation

*Geo/ASN Lookup*

-   `geoip` - Perform (local) maxmind geolocation on IPv4 addresses (see `?geoip` for details)
-   `asnip` - Perform (local) maxmind AS \# & org lookup on IPv4 addresses (see `?asnip` for details)

*Testing*

-   `randomIPs` - generate a vector of valid, random IPv4 addresses (very helpful for testing)

The following data sets are included:

-   `ianaports` - IANA Service Name and Transport Protocol Port Number Registry
-   `ianaipv4spar` - IANA IPv4 Special-Purpose Address Registry
-   `ianaipv4assignments` - IANA IPv4 Address Space Registry
-   `ianarootzonetlds` - IANA Root Zone Database
-   `ianaprotocolnumbers` - IANA Protocol Numbers

### Installation

``` {.r}
devtools::install_git("https://gitlab.dds.ec/bob.rudis/iptools.git")
```

> NOTE: Under Ubuntu (it probably applies to other variants), this only works with the current version (1.55) of the boost library, which I installed via the [launchpad boost-latest](https://launchpad.net/~boost-latest/+archive/ubuntu/ppa/+packages) package:

    sudo add-apt-repository ppa:boost-latest/ppa
    # sudo apt-get install python-software-properties if "add-apt-repository" is not found
    sudo apt-get update
    sudo apt-get install boost1.55 # might need to use 1.54 on some systems

> `homebrew` (OS X) users can do: `brew install boost` and it should `#justwork`.

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

    ##  [1] "2607:f8b0:4006:806::1008" "74.125.226.33"           
    ##  [3] "74.125.226.34"            "74.125.226.41"           
    ##  [5] "74.125.226.36"            "74.125.226.39"           
    ##  [7] "74.125.226.38"            "74.125.226.37"           
    ##  [9] "74.125.226.46"            "74.125.226.32"           
    ## [11] "74.125.226.35"            "74.125.226.40"

``` {.r}
# lookup apple (in reverse)
gethostbyaddr("17.178.96.59")
```

    ## [1] "imovietheater.com"

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

    ## 2607:f8b0:4006:806::1008            74.125.226.33            74.125.226.34 
    ##                    FALSE                     TRUE                     TRUE 
    ##            74.125.226.41            74.125.226.36            74.125.226.39 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.38            74.125.226.37            74.125.226.46 
    ##                     TRUE                     TRUE                     TRUE 
    ##            74.125.226.32            74.125.226.35            74.125.226.40 
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

    ##                ip country.code country.code3       country.name region
    ## 1   35.251.195.57           US           USA      United States     NA
    ## 2     28.57.78.42           US           USA      United States     OH
    ## 3   24.60.146.202           US           USA      United States     CT
    ## 4    14.236.36.53           VN           VNM            Vietnam     44
    ## 5   7.146.253.182           US           USA      United States     OH
    ## 6     2.9.228.172           FR           FRA             France     B5
    ## 7  108.111.124.79           US           USA      United States     NA
    ## 8    65.78.24.214           US           USA      United States     NA
    ## 9   50.48.151.239           US           USA      United States     NA
    ## 10  97.231.13.131           US           USA      United States     NA
    ## 11 81.212.124.183           TR           TUR             Turkey     NA
    ## 12 81.118.172.171           IT           ITA              Italy     NA
    ## 13 25.143.188.239           GB           GBR     United Kingdom     NA
    ## 14 66.200.137.181           US           USA      United States     CA
    ## 15  66.185.80.107           CA           CAN             Canada     NA
    ## 16  100.205.217.1           US           USA      United States     NA
    ## 17  16.140.15.132           US           USA      United States     CA
    ## 18   44.77.96.220           US           USA      United States     CA
    ## 19   84.67.252.61           IN           IND              India     NA
    ## 20 116.123.67.129           KR           KOR Korea, Republic of     11
    ## 21   81.60.52.139           ES           ESP              Spain     NA
    ## 22   49.6.198.156           CN           CHN              China     25
    ## 23  17.181.158.29           US           USA      United States     CA
    ## 24  76.124.28.185           US           USA      United States     PA
    ## 25  12.98.125.231           US           USA      United States     NA
    ##         region.name                 city postal.code latitude longitude
    ## 1                NA                   NA          NA    38.00   -97.000
    ## 2              Ohio             Columbus       43218    39.96   -82.999
    ## 3       Connecticut         East Hampton       06424    41.56   -72.494
    ## 4            Ha Noi                Hanoi          NA    21.03   105.850
    ## 5              Ohio             Columbus       43218    39.96   -82.999
    ## 6  Pays de la Loire Challain-la-potherie          NA    47.64    -1.047
    ## 7                NA                   NA          NA    38.00   -97.000
    ## 8                NA                   NA          NA    38.00   -97.000
    ## 9                NA                   NA          NA    38.00   -97.000
    ## 10               NA                   NA          NA    38.00   -97.000
    ## 11               NA                   NA          NA    39.00    35.000
    ## 12               NA                   NA          NA    42.83    12.833
    ## 13               NA                   NA          NA    51.50    -0.130
    ## 14       California           Pleasanton       94566    37.65  -121.855
    ## 15               NA                   NA          NA    60.00   -95.000
    ## 16               NA                   NA          NA    38.00   -97.000
    ## 17       California            Palo Alto       94304    37.38  -122.183
    ## 18       California            San Diego       92111    32.81  -117.165
    ## 19               NA                   NA          NA    20.00    77.000
    ## 20 Seoul-t'ukpyolsi                Seoul          NA    37.60   126.978
    ## 21               NA                   NA          NA    40.00    -4.000
    ## 22         Shandong                Jinan          NA    36.67   116.997
    ## 23       California            Cupertino       95014    37.30  -122.095
    ## 24     Pennsylvania         Phoenixville       19460    40.12   -75.538
    ## 25               NA                   NA          NA    38.00   -97.000
    ##              time.zone metro.code area.code
    ## 1                   NA          0         0
    ## 2     America/New_York        535       614
    ## 3     America/New_York        533       860
    ## 4      Asia/Phnom_Penh          0         0
    ## 5     America/New_York        535       614
    ## 6         Europe/Paris          0         0
    ## 7                   NA          0         0
    ## 8                   NA          0         0
    ## 9                   NA          0         0
    ## 10                  NA          0         0
    ## 11       Asia/Istanbul          0         0
    ## 12         Europe/Rome          0         0
    ## 13       Europe/London          0         0
    ## 14 America/Los_Angeles        807       925
    ## 15                  NA          0         0
    ## 16                  NA          0         0
    ## 17 America/Los_Angeles        807       650
    ## 18 America/Los_Angeles        825       858
    ## 19        Asia/Kolkata          0         0
    ## 20          Asia/Seoul          0         0
    ## 21                  NA          0         0
    ## 22       Asia/Shanghai          0         0
    ## 23 America/Los_Angeles        807       408
    ## 24    America/New_York        504       610
    ## 25                  NA          0         0

``` {.r}
set.seed(100000)
asnip(randomIPs(15))
```

    ##                 ip     asn                                org
    ## 1     85.220.9.200  AS6677                          Siminn hf
    ## 2    61.38.131.106  AS3786               LG DACOM Corporation
    ## 3    82.217.23.239  AS9143                         Ziggo B.V.
    ## 4  119.116.206.247  AS4837         CNCGROUP China169 Backbone
    ## 5    14.126.106.50  AS4134                           Chinanet
    ## 6    59.254.86.207 AS62294                  CastCable Limited
    ## 7     23.15.102.96 AS20940          Akamai International B.V.
    ## 8     50.219.47.40  AS7922 Comcast Cable Communications, Inc.
    ## 9    121.239.17.18  AS4134                           Chinanet
    ## 10   12.48.178.180  AS7018                AT&T Services, Inc.
    ## 11  92.141.170.178  AS3215                        Orange S.A.
    ## 12  33.226.233.128    <NA>                               <NA>
    ## 13  43.137.201.147    <NA>                               <NA>
    ## 14    2.223.87.145  AS5607   British Sky Broadcasting Limited
    ## 15   93.66.230.159 AS30722              Vodafone Omnitel B.V.

``` {.r}
#cidr
table(ip_in_cidr(cidr_ips("192.168.0.0/23"), "192.168.1.0/24"))
```

    ## 
    ## FALSE  TRUE 
    ##   256   256

### Test Results

``` {.r}
library(iptools)
library(testthat)

date()
```

    ## [1] "Mon Aug 25 11:11:01 2014"

``` {.r}
test_dir("tests/")
```

    ## Host/IPv4 resolution : ..
    ## IPv4 string/integer conversion : ....
    ## IPv4/CIDR validation : .......
    ## Geolocation : ..
