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
