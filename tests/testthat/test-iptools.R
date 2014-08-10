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


})

context("Geolocation")

test_that("we can perform IPv4 geolocation", {

  set.seed(1000000000)
  expect_that(geoip(randomIPs(1)), equals(structure(list(ip = structure(1L, .Label = "133.232.196.68", class = "factor"),
                                                         country.code = structure(1L, .Label = "JP", class = "factor"),
                                                         country.code3 = structure(1L, .Label = "JPN", class = "factor"),
                                                         country.name = structure(1L, .Label = "Japan", class = "factor"),
                                                         region = structure(1L, .Label = "NA", class = "factor"),
                                                         region.name = structure(1L, .Label = "NA", class = "factor"),
                                                         city = structure(1L, .Label = "NA", class = "factor"), postal.code = structure(1L, .Label = "NA", class = "factor"),
                                                         latitude = 35.689998626709, longitude = 139.690002441406,
                                                         time.zone = structure(1L, .Label = "Asia/Tokyo", class = "factor"),
                                                         metro.code = 0L, area.code = 0L), .Names = c("ip", "country.code",
                                                                                                      "country.code3", "country.name", "region", "region.name", "city",
                                                                                                      "postal.code", "latitude", "longitude", "time.zone", "metro.code",
                                                                                                      "area.code"), row.names = c(NA, -1L), class = "data.frame")))

  set.seed(1000000000)
  expect_that(asnip(randomIPs(10)), equals(structure(list(ip = structure(c(1L, 5L, 3L, 8L, 7L, 2L, 9L, 6L,
                                                                           4L, 10L), .Label = c("133.104.20.250", "136.113.177.174", "196.105.157.133",
                                                                                                "213.243.203.112", "232.205.194.54", "242.150.122.127", "62.147.200.18",
                                                                                                "68.190.32.214", "70.152.59.168", "71.107.246.210"), class = "factor"),
                                                          asn = structure(c(3L, NA, NA, 2L, 1L, NA, 5L, NA, 4L, 6L), .Label = c("AS12322",
                                                                                                                                "AS20115", "AS2503", "AS29050", "AS6389", "AS701"), class = "factor"),
                                                          org = structure(c(6L, NA, NA, 2L, 3L, NA, 1L, NA, 5L, 4L), .Label = c("BellSouth.net Inc.",
                                                                                                                                "Charter Communications", "Free SAS", "MCI Communications Services, Inc. d/b/a Verizon Business",
                                                                                                                                "Terrecablate Reti e Servizi S.R.L.", "Tohoku Open Internet Community"
                                                          ), class = "factor")), .Names = c("ip", "asn", "org"), row.names = c(NA,
                                                                                                                               -10L), class = "data.frame")))
})
