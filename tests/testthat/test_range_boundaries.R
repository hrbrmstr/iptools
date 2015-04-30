context("Test range_boundaries works appropriately")

test_that("range_boundaries handles single values appropriately",{
  result <- range_boundaries("172.18.0.0/28")
  expect_that(length(result), equals(1))
  expect_true(is.list(result))
  result <- unlist(result)
  expect_that(result, equals(c("172.18.0.0","172.18.0.15")))
})

test_that("range_boundaries handles multiple values appropriately",{
  result <- range_boundaries(c("172.18.0.0/28","192.168.1.0/24"))
  expect_that(length(result), equals(2))
  expect_true(is.list(result))
  result <- unlist(result)
  expect_that(result, equals(c("172.18.0.0","172.18.0.15","192.168.1.0","192.168.1.255")))
})