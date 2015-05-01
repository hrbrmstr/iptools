#'@title Refresh iptools Internal Datasets
#'
#'@description \code{iptools} contains a variety of internal datasets.
#'While these should be updated on a regular basis by the package authors
#'and maintainers, they can also be refreshed by you, the user, using this collection
#'of functions. Each one takes the form [dataset_name]_refresh() to make it clear
#'which dataset is updated by which function.
#'
#'@export
#'
#'@examples
#'\dontrun{
#'#update iana_assignments
#'iana_assignments_refresh()
#'#[1] TRUE
#'}
iana_assignments_refresh <- function(){

  #Read in and basic-clean
  connection <- url("http://www.iana.org/assignments/ipv4-address-space/ipv4-address-space.csv")
  data <- read.csv(connection, as.is = TRUE)
  names(data) <- c("prefix","designation","date","whois","rdap","status", "note")
  data <- data[,c("prefix","designation","date","whois","status")]

  #Sort out the prefixes
  prefix <- as.data.frame(matrix(unlist(strsplit(data$prefix, "/")),
                                 byrow = TRUE, nrow = length(data$prefix)),
                          stringsAsFactors = FALSE)
  data$prefix <- paste0(as.numeric(prefix$V1),".0.0.0/", prefix$V2)

  #Write out and report
  iana_assignments <- data
  save(iana_assignments, file = system.file("data/iana_assignments.rda", package = "iptools"))
  return(TRUE)
}

iana_special_assignments_refresh <- function(){
  connection <- url("http://www.iana.org/assignments/ipv4-address-space/ipv4-address-space.csv")
  data <- read.csv(connection, as.is = TRUE)
}

