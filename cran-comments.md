## Test environments
- It has been tested on:

  * a standalone Windows systems (release)
  * 2 Linux systems (devel)
  * 2 OS X systems (devel & release)
  * WinBuilder (devel & release)

## R CMD check results
- There were no ERRORs or WARNINGs. 
- The NOTE - "Note: found 12 marked UTF-8 strings" are necessary (and have been
  NOTEd in every release to-date)

## Other

iptools 0.4.0
=============
* bug fix for `ip_in_range` always returning TRUE for /32 ranges (@tdsmith)
* enhanced `range_boundaries()` to include more columns in output
* added `country_ranges()` which enables obtaining all the IPv4 ranges
  "assigned" within a country.
