There is an INSTALL file that notes that this does not compile under Windows (the OS_type DESCRIPTION field has been set accordingly to 'unix'). The package relies on Boost Regex and Boost ASIO which require compilation (i.e. are not just usable with the BH package) and that requires a full cygwin setup (they don't compile with regular Rtools) and then further configuration. It is highly unlikely anyone will be able to get it to work easily on Windows without significant effort.

We believe there is enough merit/utility in the library (extremely fast IP address operations useful for cybersecurity research and general internet/web large scale analyses) that we are attempting to get it on CRAN.

NOTE: There is a character set warning: 

  Note: found 9 marked UTF-8 strings
  Warning: found non-ASCII strings
  'Framework for transferring any file from any app, to
        any app on any device: simply with a sw<c3><bf>p.' in object 'iana_ports'
  '[Ari<e2><88><9a><c2><b4>n Huisken <xadmin&huisken-systems.nl> 15 June 2009]' in object 'iana_ports'

This is from the actual data from IANA that is used by the package (and stored in R data files). This is not an error and should not be worked around.

There is a configuration script for the C/C++ src build and it has been tested on vanilla Ubuntu Trusty/Precise (64-bit) systems, CentOS 7.1 and OS X 10.10.3.