iptools
===============================================================================

`iptools` is a set of tools for a working with IPv4 addresses. The aim is to
provide functionality not presently available with any existing R package
and to do so with as much speed as possible. To that end, many of the operations
are written in `Rcpp` and require installation of the `Boost` libraries.
A current, lofty goal is to mimic most of the functionality of the Python
`iptools` module and make IP addresses first class R objects.

Currently, the following functions are implemented:

- `gethostbyaddr` - Returns all 'PTR' records associated with an IPv4 address
- `gethostbyname` - Returns all 'A' records associated with a hostname
- `ip2long` - Character (dotted-decimal) IPv4 Address Conversion to long integer
- `iptools` - A package to help perform various tasks with/on IPv4 addresses
- `long2ip` - Intger IPv4 Address Conversion to Character