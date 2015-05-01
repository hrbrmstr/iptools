context("Test IP generators")

test_that("Random generation works", {
  result <- random_ips(10)
  expect_that(is.vector(result, "character"), equals(TRUE))
  expect_that(length(result), equals(10))
  expect_that(all(ip_to_numeric(result) > 0), equals(TRUE))
})

test_that("Range generation works", {
  result <- generate_range("172.18.0.0/28")
  expect_that(is.vector(result, "character"), equals(TRUE))
  expect_that(length(result), equals(16))
})

test_that("Range generation error handlers work", {
  expect_error(generate_range("TURN DOWN FOR HWAET"), "Invalid range")
})