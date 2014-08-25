context("Host/IPv4 resolution")

test_that("we can resolve IPv4 addresses from hostnames", {

  # success
  expect_that(gethostbyname("dds.ec"), equals("162.243.111.4"))

  # not found
  expect_that(gethostbyname("f0011"), equals(character(0)))

})

context("IPv4 string/integer conversion")

test_that("we can perform IPv4 address string/integer conversions", {

  # success
  expect_that(long2ip(402654475), equals("24.0.5.11"))

  # multiple
  expect_that(long2ip(c(402654475, 3540208992)), equals(c("24.0.5.11", "211.3.77.96")))

  # success
  expect_that(ip2long("24.0.5.11"), equals(402654475))

  # multiple
  expect_that(ip2long(c("24.0.5.11", "211.3.77.96")), equals(c(402654475, 3540208992)))

})


context("IPv4/CIDR validation")

test_that("we can perform IPv4/CIDR validation", {

  # success
  expect_that(validateIP("127.0.0.1"), equals(structure(TRUE, .Names = "127.0.0.1")))

  # failure
  expect_that(validateIP("127.0"), equals(structure(FALSE, .Names = "127.0")))

  # multiple
  expect_that(as.logical(validateIP(c("127.0.0.1", "255.255.255.255", "500.1.1.1", "30.2.2"))),
              is_equivalent_to(c(TRUE, TRUE, FALSE, FALSE)))


  # success
  expect_that(validateCIDR("127.0.0.1/32"), equals(structure(TRUE, .Names = "127.0.0.1/32")))

  # failure
  expect_that(validateCIDR("127.0/8"), equals(structure(FALSE, .Names = "127.0/8")))

  # multiple
  expect_that(as.logical(validateCIDR(c("127.0.0.256/32", "127.0.0.0", "127.0.0.1/33", "127.0.0.1/32"))),
              is_equivalent_to(c(FALSE, FALSE, FALSE, TRUE)))

  # cidr
  expect_that(sum(ip_in_cidr(cidr_ips("192.168.0.0/23"), "192.168.1.0/24")), equals(256))

})

context("Geolocation")

test_that("we can perform IPv4 geolocation", {

  set.seed(1000000000)
  expect_that(geoip(randomIPs(1)), equals(structure(list(ip = structure(1L, .Label = "66.170.120.163", class = "factor"),
                                                         country.code = structure(1L, .Label = "US", class = "factor"),
                                                         country.code3 = structure(1L, .Label = "USA", class = "factor"),
                                                         country.name = structure(1L, .Label = "United States", class = "factor"),
                                                         region = structure(1L, .Label = "CA", class = "factor"),
                                                         region.name = structure(1L, .Label = "California", class = "factor"),
                                                         city = structure(1L, .Label = "Campbell", class = "factor"),
                                                         postal.code = structure(1L, .Label = "95008", class = "factor"),
                                                         latitude = 37.2803001403809, longitude = -121.956703186035,
                                                         time.zone = structure(1L, .Label = "America/Los_Angeles", class = "factor"),
                                                         metro.code = 807L, area.code = 408L), .Names = c("ip", "country.code",
                                                                                                          "country.code3", "country.name", "region", "region.name", "city",
                                                                                                          "postal.code", "latitude", "longitude", "time.zone", "metro.code",
                                                                                                          "area.code"), row.names = c(NA, -1L), class = "data.frame")))

  set.seed(1000000000)
  expect_that(asnip(randomIPs(10)), equals(structure(list(ip = structure(c(8L, 2L, 10L, 5L, 4L, 9L, 6L,
                                                                           3L, 1L, 7L), .Label = c("105.249.128.103", "115.123.191.104",
                                                                           "120.92.170.107", "31.200.58.110", "34.207.116.122", "35.212.185.89",
                                                                          "36.109.13.27", "66.170.120.163", "68.54.134.127", "97.185.12.167"
                                                                           ), class = "factor"), asn = structure(c(3L, NA, 4L, NA, 1L, 5L,
                                                                           NA, NA, 2L, NA), .Label = c("AS12978", "AS29975", "AS30121",
                                                                           "AS6167", "AS7922"), class = "factor"), org = structure(c(1L,
                                                                           NA, 2L, NA, 4L, 3L, NA, NA, 5L, NA), .Label = c("24/7 Customer, Inc.",
                                                                          "Cellco Partnership DBA Verizon Wireless", "Comcast Cable Communications, Inc.",
                                                                          "DOGAN TV DIGITAL PLATFORM ISLETMECILIGI A.S.", "VODACOM-ZA"), class = "factor")), .Names = c("ip",
                                                                           "asn", "org"), row.names = c(NA, -10L), class = "data.frame")))
})
