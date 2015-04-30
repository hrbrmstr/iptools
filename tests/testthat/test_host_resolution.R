context("Test IP to hostname and hostname to IP resolution")

test_that("A single hostname can be turned into its equivalent IP(s)",{
  result <- hostname_to_ip("dds.ec")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals("162.243.111.4"))
})

test_that("Multiple hostnames can be turned into their equivalent IP(s)",{
  result <- hostname_to_ip(c("dds.ec","dds.ec"))
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(2))
  expect_that(unlist(result), equals(c("162.243.111.4","162.243.111.4")))
})

test_that("hostname-to-ip error handlers work",{
  result <- hostname_to_ip("tellmetellmetellmesomethingidontknow.org")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals("Not resolved"))
})

test_that("A single IP can be turned into its equivalent hostname(s)",{
  result <- ip_to_hostname("162.243.111.4")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals("dds.ec"))
})

test_that("Multiple IPs can be turned into their equivalent hostname(s)",{
  result <- ip_to_hostname(c("162.243.111.4","162.243.111.4"))
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(2))
  expect_that(unlist(result), equals(c("dds.ec","dds.ec")))
})

test_that("ip-to-hostname error handlers work",{
  result <- ip_to_hostname("1!!!")
  expect_that(is.list(result), equals(TRUE))
  expect_that(length(result), equals(1))
  expect_that(unlist(result), equals("Invalid IP address"))
})