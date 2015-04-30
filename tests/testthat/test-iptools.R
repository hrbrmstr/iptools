context("Host/IPv4 resolution")

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