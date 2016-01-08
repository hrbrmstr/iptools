## Test environments
- It has been tested on:

  * two Windows systems (release)
  * 3 Linux systems (devel)
  * 4 OS X systems (devel & release)
  * WinBuilder (devel & release)

- It's also been tested against g++ & clang compilers.

- It's been checked with devtools::check(), RStudio "Check" and std R pkg check
  from the command-line on all three platforms.

## R CMD check results
- There were no ERRORs, NOTEs or WARNINGs. 
- The previous NOTE - "Note: found 12 marked UTF-8 strings" - has not come up
  on any system we've done this round of package checks on.

## Other
- This is an overhaul of the package to use the new AsioHeaders pkg by Dirk which
  makes it far more lightweight and able to support Windows installs. This has
  also fixed the "CRAN Package Check Results for Package iptools" on OS X:
  https://cran.rstudio.com/web/checks/check_results_iptools.html