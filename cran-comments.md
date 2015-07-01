There is a SystemRequirements field that has external dependency information and an additional README with explicit library installation recommendations.

There is also an INSTALL file that notes that this does not compile under Windows (the OS_type DESCRIPTION field has been set accordingly to 'unix').

As stated in the README, the package relies on Boost Regex and Boost ASIO which require compilation (i.e. are not just usable with the BH package) and that requires a full cygwin setup (they don't compile with regular Rtools) and then further configuration. It is highly unlikely anyone will be able to get it to work easily on Windows without significant effort.

We believe there is enough merit/utility in the library (extremely fast IP address operations useful for cybersecurity research and general internet/web large scale analyses) that we are attempting to get it on CRAN.

There is a configuration script for the C/C++ src build and it has been tested on vanilla Ubuntu Trusty/Precise (64-bit) systems, CentOS 7.1, Debian 8 (r-devel) and OS X 10.10.3.
