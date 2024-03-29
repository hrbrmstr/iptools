iptools 0.7.2
=============
* Fixes CRAN checks and removes dependency on {readr} (which appears to be the cause).
* New `ip6_to_bytes()` and `ip6_to_nibble()`
* New `ip_to_subnet()` (implements #38)

iptools 0.6.1
=============
* Fixed serious bug in `ips_in_cidrs()` introduced after removing tidyverse deps
* Fixed serious bug in `asn_table_to_trie()` introduced after removing
  tidyverse deps (Fixes #36)
* Added tests & examples for all CIDR ops
* Added test data file for ASN trie ops

iptools 0.6.0
=============
* `range_boundaries_to_cidr()` will take a start/stop numeric range
  and turn it into representative/encompassing CIDR blocks

iptools 0.5.0
=============
* NA support added for IP validity checkers and classifiers
* Hilbert coordiates generator function added
* `ip_in_any()` added for a more generic range membership test
* `ip_to_asn()` added (with supporting functions)

iptools 0.4.0
=============
* bug fix for `ip_in_range` always returniong TRUE for /32 ranges (@tdsmith)
* enhanced `range_boundaries()` to include more columns in output
* added `country_ranges()` which enables obtaining all the IPv4 ranges
  "assigned" within a country.

iptools 0.3.0
=============
* Removal of the need for linking to Boost
* Windows support!
* `is_ipv4`, `is_ipv6`, `is_valid` and `is_multicast` added.

iptools 0.2.1
=============
* XFF origin extractor included as xff_extract
* Documentation improvements

iptools 0.2.0
=============
NEW FEATURES
* Total internal rewrite. Check out the vignettes!
* IP DNS resolution now handles IPv6 IPs

BUG FIXES

DEVELOPMENT
*geolocation functionality deprecated; rely on devtools::install_github("ironholds/rgeolocate") for geolocation.
*Large changeover to relying on C++ rather than R

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