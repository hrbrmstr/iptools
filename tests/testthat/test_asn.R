context("Test autonomous systems functions")

test_that("IPv4 AS lookup works",{
  expect_that(as_lookup("188.143.232.26"), is_a("data.frame"))
  expect_that(as_lookup("2607:f8b0:4006:80b::1004"), is_a("data.frame"))
})

