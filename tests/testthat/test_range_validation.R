context("Test range validation")

test_that("Range validation works with single ranges", {
  result <- validate_range("127.0.0.1/32")
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(result, equals(TRUE))
})

test_that("Range validation works with multiple ranges", {
  result <- validate_range(c("127.0.0.1/32","127.0.0.1/32"))
  expect_that(is.vector(result, "logical"), equals(TRUE))
  expect_that(length(result), equals(2))
  expect_that(result, equals(c(TRUE,TRUE)))
})

test_that("Range validation error handlers work", {
  expect_false(validate_range("127.0.0.1")) #Valid IP, no slash
  expect_false(validate_range("asdsadas/32")) #slash, but not a valid IP
  expect_false(validate_range("127.0.0.1/33")) #slash, valid IP, invalid range
})