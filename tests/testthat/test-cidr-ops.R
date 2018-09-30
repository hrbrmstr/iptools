context("Test CIDR Operations")

test_that("asn table ops work",{

  asn_table_to_trie(
    system.file("test", "rib.tst.gz", package="iptools")
  ) -> asntbl

  expect_equal(dim(asntbl), 9994)

  expect_equal(ip_to_asn(asntbl, "5.192.0.1"), 5384)

  ips_in_cidrs(
    c("4.3.2.1", "1.2.3.4", "1.20.113.10", "5.190.145.5"),
    c("5.190.144.0/21", "1.20.113.0/24")
  ) -> res

  expect_equal(res$in_cidr, c(FALSE, FALSE, TRUE, TRUE))

  expect_equal(host_count("1.52.0.0/14"), 262144)

})
