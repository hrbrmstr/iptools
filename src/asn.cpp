#include <Rcpp.h>
#include "GeoIP.h"

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <asio.hpp>
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif

#include "asio_bindings.h"

using namespace Rcpp;

GeoIP *gi = NULL;

//[[Rcpp::export]]
void asn_init(std::string path) {
  gi = GeoIP_open(path.c_str(), GEOIP_INDEX_CACHE);
}

//' Test if the autonomous system database loaded
//'
//' @export
//[[Rcpp::export]]
bool asn_db_is_loaded() {
  return(gi != NULL);
}

//[[Rcpp::export]]
DataFrame asn_lookup_(std::vector < std::string > ips) {

  asio_bindings asio_inst;

  int input_size = ips.size();

  std::vector < std::string > asn_holding(input_size);
  std::vector < unsigned int > ip_long = asio_inst.ip_to_numeric_(ips);

  if (gi != NULL) {
    char *asn = NULL;
    for (unsigned int i = 0; i < input_size; i++) {
      if ((i % 10000) == 0) {
        Rcpp::checkUserInterrupt();
      }
      if (ip_long[i] == 0) {
        asn_holding[i] = std::string("");
      } else {
        asn = GeoIP_org_by_name(gi, ips[i].c_str());
        asn_holding[i] = std::string(asn);
      }
    }
  }

  return DataFrame::create(_["ip"] = ips,
                           _["ip_numeric"] = ip_long,
                           _["asn_org"] = asn_holding,
                           _["stringsAsFactors"] = false);
}

//[[Rcpp::export]]
void asn_fin() {
  if (gi != NULL) GeoIP_delete(gi);
}
