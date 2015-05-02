## ----eval=FALSE----------------------------------------------------------
#  ips <- c("192.168.0.1","2607:f8b0:4006:80b::aaa","the next IP is also invalid","256.256.190.900")
#  ip_classify(ips)
#  [1] "IPv4"    "IPv6"    "Invalid" "Invalid"

## ----eval=FALSE----------------------------------------------------------
#  #Dotted-decimal to numeric
#  ips <- c("192.168.0.1","172.18.0.0","172.18.0.15")
#  numeric_ips <- ip_to_numeric(ips)
#  numeric_ips
#  [1] 3232235521 2886860800 2886860815
#  
#  #And back again
#  numeric_to_ip(numeric_ips)
#  [1] "192.168.0.1" "172.18.0.0"  "172.18.0.15"

