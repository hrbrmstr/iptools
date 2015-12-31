This is an overhaul of the package to use the new AsioHeaders by Dirk.

It has been tested on two Windows systems, 3 Linux systems and 4 OS X systems
as well as WinBuilder (a mixture of devel and stable on most of those systems).

There's one NOTE about:

* checking data for non-ASCII characters ... NOTE
  Note: found 12 marked UTF-8 strings
  
That's fine given that there are UTF-8 domain names in some data files.

It's also been tested against g++ & clang compilers.

It's been checked with devtools::check(), RStudio "Check" and std R pkg check
from the command-line on all three platforms.

If it fails on Solaris, it fails with apologies but also with complete knowledge
that it's highly unlikely Solaris folks are going to be doing work requiring
manipulation of IP addresses.