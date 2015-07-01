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
#'
#'#update iana_assignments
#'
#'iana_assignments_refresh()
#'#[1] TRUE
#'
#'#update iana_special_assignments
#'
#'iana_special_assignments_refresh()
#'#[1] TRUE
#'}
#'
#'@rdname iptools_refresh
#'@export
iana_assignments_refresh <- function(){

  #Read in and clean
  connection <- url("http://www.iana.org/assignments/ipv4-address-space/ipv4-address-space.csv")
  data <- read.csv(connection, as.is = TRUE, encoding="UTF-8")
  names(data) <- c("prefix","designation","date","whois","rdap","status", "note")
  data <- data[,c("prefix","designation","date","whois","status")]

  #Sort out the prefixes
  prefix <- as.data.frame(matrix(unlist(strsplit(data$prefix, "/")),
                                 byrow = TRUE, nrow = length(data$prefix)),
                          stringsAsFactors = FALSE)
  data$prefix <- paste0(as.numeric(prefix$V1),".0.0.0/", prefix$V2)

  #Write out and report
  iana_assignments <- data
  save(iana_assignments, file = system.file("data/iana_assignments.rda", package = "iptools"),
       compress = 'xz')
  return(TRUE)
}

#'@rdname iptools_refresh
#'@export
iana_special_assignments_refresh <- function(){
  special_gsub <- function(x){
    gsub(x = x, pattern = "\\[.*\\]", replacement = "")
  }

  #Read in
  connection <- url("http://www.iana.org/assignments/iana-ipv4-special-registry/iana-ipv4-special-registry-1.csv")
  data <- read.csv(connection, as.is = TRUE, encoding="UTF-8")
  names(data) <- c("address_block","name","rfc","allocation_date","termination_date","source",
                   "destination","forwardable","global","reserved_by_protocol")
  data <- data[,!names(data) == c("termination_date")]

  #Reformat
  data$address_block <- special_gsub(data$address_block)
  data$source <- as.logical(special_gsub(data$source))
  data$destination <- as.logical(special_gsub(data$destination))
  data$forwardable <- as.logical(special_gsub(data$forwardable))
  data$global <- as.logical(special_gsub(data$global))
  data$reserved_by_protocol <- as.logical(special_gsub(data$reserved_by_protocol))
  iana_special_assignments <- data
  save(iana_special_assignments, file = system.file("data/iana_special_assignments.rda", package = "iptools"),
       compress = 'xz')
  return(TRUE)
}

#'@rdname iptools_refresh
#'@export
iana_ports_refresh <- function(){
  connection <- url("http://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.csv")
  data <- read.csv(connection, as.is = TRUE, encoding="UTF-8")
  names(data) <- c("service_name","port_number","transport_protocol", "description", "assignee", "contact",
                   "registration_date","modification_date","reference","service_code","known_unauthorised_uses",
                   "assignment_notes")
  data$assignee <- gsub(x = data$assignee, pattern = "(\\[|\\]|)", replacement = "")
  data$assignee <- gsub(x = data$assignee, pattern = "_", replacement = " ")
  data$contact <- gsub(x = data$contact, pattern = "(\\[|\\]|)", replacement = "")
  data$contact <- gsub(x = data$contact, pattern = "_", replacement = " ")
  iana_ports <- data
  save(iana_ports, file = system.file("data/iana_ports.rda", package = "iptools"),
       compress = 'xz')
  return(TRUE)
}