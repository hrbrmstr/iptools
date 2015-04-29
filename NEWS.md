iptools 0.2.0 [WIP]
=============
NEW FEATURES
* gethostbyname and gethostsbynames are now the same function, named hostname\_to\_ip
* gethostbyaddr and gethostsbyaddr are now the same function, named ip_to_hostname

BUG FIXES

DEVELOPMENT
*geolocation functionality deprecated; rely on install_github("ironholds/rgeoip") for geolocation.
*hostname-to-DNS code, including the vectorisation, moved to C++

iptools 0.1.4
=============

NEW FEATURES
* added CIDR functions

iptools 0.1.3
=============

NEW FEATURES
* added IANA TLD dataset

iptools 0.1.2
=============

NEW FEATURES
* added IANA datasets

iptools 0.1.1
============
NEW FEATURES
* validation routines

iptools 0.1.0
=============

* initial release of Rcpp-backed IPv4 functions