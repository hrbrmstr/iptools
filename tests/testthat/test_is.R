context("Test boolean is_* functions")

test_that("IPv4 identification works",{
  expect_equal(is_ipv4("127.0.0.1"), TRUE)
  expect_equal(is_ipv4("2607:f8b0:4006:80b::1004"), FALSE)
})

test_that("IPv6 identification works",{
  expect_equal(is_ipv6("127.0.0.1"), FALSE)
  expect_equal(is_ipv6("2607:f8b0:4006:80b::1004"), TRUE)
})


test_that("Validity checks work",{
  expect_equal(is_valid("127.0.0.1"), TRUE)
  expect_equal(is_valid("2607:f8b0:4006:80b::1004"), TRUE)
  expect_equal(is_valid("Cooper-Temple Clause"), FALSE)
})

test_that("Multicast checks work",{
  expect_equal(is_multicast("224.0.0.5"), TRUE)
  expect_equal(is_multicast("193.168.0.1"), FALSE)
  expect_equal(is_multicast("Cooper-Temple Clause"), NA)
})