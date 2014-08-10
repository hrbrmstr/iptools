#include <Rcpp.h>

#include "GeoIP.h"
#include "GeoIPCity.h"

using namespace Rcpp;

Function msg("message"); // lets us use R's message() function

static const char * _mk_NA( const char * p ){
 return p ? p : "ZZ";
}

// [[Rcpp::export]]
DataFrame geoip(std::string host, std::string datafile="/usr/local/share/GeoIP/GeoLiteCity.dat") {

  GeoIP *gi = GeoIP_open(datafile.c_str(), GEOIP_INDEX_CACHE);

  Rcout<<"one"<<std::endl;

  if (gi == NULL) {
    msg("Error opening database");
    return Rcpp::DataFrame::create(Rcpp::Named("country.code")=CharacterVector(NA_STRING),
                                   Rcpp::Named("region")=CharacterVector(NA_STRING),
                                   Rcpp::Named("region.name")=CharacterVector(NA_STRING),
                                   Rcpp::Named("city")=CharacterVector(NA_STRING),
                                   Rcpp::Named("postal.code")=CharacterVector(NA_STRING),
                                   Rcpp::Named("metro.code")=NumericVector(NA_REAL),
                                   Rcpp::Named("area.code")=NumericVector(NA_REAL),
                                   Rcpp::Named("time.zone")=CharacterVector(NA_STRING),
                                   Rcpp::Named("latitude")=NumericVector(NA_REAL),
                                   Rcpp::Named("longitude")=NumericVector(NA_REAL));
  }

  GeoIPRecord *gir;

  gir = GeoIP_record_by_name(gi, host.c_str());

  Rcout<<"two"<<std::endl;

  if (gir == NULL) {
    msg("Error retrieving record");
    return Rcpp::DataFrame::create(Rcpp::Named("country.code")=CharacterVector(NA_STRING),
                                   Rcpp::Named("region")=CharacterVector(NA_STRING),
                                   Rcpp::Named("region.name")=CharacterVector(NA_STRING),
                                   Rcpp::Named("city")=CharacterVector(NA_STRING),
                                   Rcpp::Named("postal.code")=CharacterVector(NA_STRING),
                                   Rcpp::Named("metro.code")=NumericVector(NA_REAL),
                                   Rcpp::Named("area.code")=NumericVector(NA_REAL),
                                   Rcpp::Named("time.zone")=CharacterVector(NA_STRING),
                                   Rcpp::Named("latitude")=NumericVector(NA_REAL),
                                   Rcpp::Named("longitude")=NumericVector(NA_REAL));
  }

  const char *time_zone = GeoIP_time_zone_by_country_and_region(gir->country_code, gir->region);

  char **ret = GeoIP_range_by_ip(gi, host.c_str());

  Rcout<<"three"<<std::endl;

  Rcpp::DataFrame geodf = Rcpp::DataFrame::create(Rcpp::Named("country.code")=CharacterVector(_mk_NA(gir->country_code)),
                                                  Rcpp::Named("region")=CharacterVector(_mk_NA(gir->region)),
                                                  Rcpp::Named("region.name")=CharacterVector(_mk_NA(GeoIP_region_name_by_code(gir->country_code, gir->region))),
                                                  Rcpp::Named("city")=CharacterVector(_mk_NA(gir->city)),
                                                  Rcpp::Named("postal.code")=CharacterVector(gir->postal_code),
                                                  Rcpp::Named("metro.code")=NumericVector(Rcpp::wrap(gir->metro_code)),
                                                  Rcpp::Named("area.code")=NumericVector(Rcpp::wrap(gir->area_code)),
                                                  Rcpp::Named("time.zone")=CharacterVector(_mk_NA(time_zone)),
                                                  Rcpp::Named("latitude")=NumericVector(Rcpp::wrap(gir->latitude)),
                                                  Rcpp::Named("longitude")=NumericVector(Rcpp::wrap(gir->longitude)));


  GeoIP_range_by_ip_delete(ret);
  GeoIPRecord_delete(gir);
  GeoIP_delete(gi);

  return(geodf);

}
