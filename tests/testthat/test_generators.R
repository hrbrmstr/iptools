context("Test IP generators")

test_that("Random generation works", {
  result <- ip_random(10)
  expect_that(is.vector(result, "character"), equals(TRUE))
  expect_that(length(result), equals(10))
  expect_that(all(ip_to_numeric(result) > 0), equals(TRUE))
})

test_that("Range generation works", {
  result <- range_generate("172.18.0.0/28")
  expect_that(is.vector(result, "character"), equals(TRUE))
  expect_that(length(result), equals(16))
})

test_that("Range generation error handlers work", {
  expect_error(range_generate("TURN DOWN FOR HWAET"), "Invalid range")
})