context("Ensure that ip_in_range and range_boundaries work")

test_that("ip_in_range works with single values", {
  result <- ip_in_range("172.18.0.0","172.18.0.0/12")
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(result, equals(TRUE))
})

test_that("ip_in_range works with multiple IP values", {
  result <- ip_in_range(c("172.18.0.0","172.18.0.0"), "172.18.0.0/12")
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(2))
  expect_that(result, equals(c(TRUE,TRUE)))
})

test_that("ip_in_range works with multiple IP values and multiple range values", {
  result <- ip_in_range(c("172.18.0.0","172.18.0.0"), c("172.18.0.0/12","172.18.0.0/12"))
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(2))
  expect_that(result, equals(c(TRUE,TRUE)))
})

test_that("ip_in_range error handlers function",{
  result <- ip_in_range("asdasdas12","aaaah")
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(result, equals(FALSE))

  result <- ip_in_range("dfndsfkdsfsd","afddas/12")
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(result, equals(FALSE))
})

test_that("range_boundaries works with single values", {
  result <- range_boundaries("172.18.0.0/12")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals(c("172.18.0.0","172.31.255.255")))
})

test_that("range_boundaries works with multiple values", {
  result <- range_boundaries(c("172.18.0.0/12","172.18.0.0/12"))
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(2))
  expect_that(unlist(result), equals(c("172.18.0.0","172.31.255.255","172.18.0.0","172.31.255.255")))
})

test_that("range_boundaries error handlers function",{
  result <- range_boundaries("fdsdfdsfsdfsfd")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals(c("Invalid","Invalid")))

  result <- range_boundaries("fdsdfdsfsdfsfd/12")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals(c("Invalid","Invalid")))
})