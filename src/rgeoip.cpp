#include <Rcpp.h>
#include <boost/asio.hpp>

#include "GeoIP.h"
#include "GeoIPCity.h"

using namespace Rcpp;

extern Function message;

GeoIP *gi = NULL;

// return either a charactervector of the input C string or NA
// NA only if the value was null
CharacterVector mkNA( const char * p ){
 return p ? CharacterVector(p) : CharacterVector(NA_STRING);
}

// return a NA row for a geo record
DataFrame geoNARow() {
  return(DataFrame::create(
    Rcpp::Named("country.code")=CharacterVector::create(NA_STRING),
    Rcpp::Named("country.code3")=CharacterVector::create(NA_STRING),
    Rcpp::Named("country.name")=CharacterVector::create(NA_STRING),
    Rcpp::Named("region")=CharacterVector::create(NA_STRING),
    Rcpp::Named("region.name")=CharacterVector::create(NA_STRING),
    Rcpp::Named("city")=CharacterVector::create(NA_STRING),
    Rcpp::Named("postal.code")=CharacterVector::create(NA_STRING),
    Rcpp::Named("latitude")=NumericVector::create(NA_REAL),
    Rcpp::Named("longitude")=NumericVector::create(NA_REAL),
    Rcpp::Named("time.zone")=CharacterVector::create(NA_STRING),
    Rcpp::Named("metro.code")=IntegerVector::create(NA_INTEGER),
    Rcpp::Named("area.code")=IntegerVector::create(NA_INTEGER))
  );
}

//' Initializes the maxmind library and opens the \code{GeoLiteCity.dat} file
//'
//' This function must be called before performing a lookup with \code{geoip()}.
//' The default full path spec defaults to \code{/usr/local/share/GeoIP/GeoLiteCity.dat}
//' and can be overriden by changing the value of the \code{datafile} parameter.
//'
//' NOTE: You must manually retrieve the \code{GeoLiteCity.dat.gz} file from maxmind
//' via \url{http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz} and
//' unzip it to the location you will be specifying in \code{datafile}.
//'
//' Any errors reading the file will result in a console message.
//'
//' @param datafile full path (including filename) to the \code{GeoLiteCity.dat} file
//' @examples
//' \dontrun{
//' geofile()
//' }
//[[Rcpp::export]]
void geofile(std::string datafile="/usr/local/share/GeoIP/GeoLiteCity.dat") {

  if (gi == NULL) {
     gi = GeoIP_open(datafile.c_str(), GEOIP_INDEX_CACHE);
     if (gi == NULL) {
      message("Error opening database");
     }
  }

}

//' Return a data frame of geolcation values for a given IP address
//'
//' Uses the maxmind \code{GeoIPCity.dat} binary file to perform a geolocation for
//' a given IPv4 address and returns a data frame of geolocation records.
//' You must call \code{geofile()} before calling \code{geoip()}
//'
//' Values returned in the data frame:
//' \itemize{
//'   \item \code{country.code}. ISO2 country code (chr)
//'   \item \code{country.code3}. ISO3 country code (chr)
//'   \item \code{region}. Abbreviated region name (chr)
//'   \item \code{region.name}. Full region name (chr)
//'   \item \code{city}. City name (chr)
//'   \item \code{postal.code}. Postal code (chr)
//'   \item \code{latitude}. Latitude (numeric)
//'   \item \code{lontigude}. Longitude (numeric)
//'   \item \code{time.zone}. Time zone (chr)
//'   \item \code{metro.code}. Metro code (int)
//'   \item \code{area.code}. Area code (int)
//' }
//'
//' @param ip character string of IPv4 address to lookup
//' @return data frame row of geolocation information
//' @note not vectorized
//' @examples
//' \dontrun{
//' geofile()
//' geoip("20.30.40.50")
//' ##  country.code country.code3  country.name region region.name         city
//' ## 1           US           USA United States     VA    Virginia Falls Church
//' ##   postal.code latitude longitude        time.zone metro.code area.code
//' ## 1       22042   38.864  -77.1922 America/New_York        511       703
//' }
//[[Rcpp::export]]
DataFrame geoip(std::string ip) {

  if (gi == NULL) {
    message("Error opening database. Did you forget to call geofile()?");
    return(geoNARow());
  }

  uint32_t ipnum = 0;

  try {
    ipnum = boost::asio::ip::address_v4::from_string(ip.c_str()).to_ulong();
  } catch(boost::system::system_error& error) {
    message( "Host/IP error" );
    return(geoNARow());
  }

  if (ipnum == 0) {

    message("Host/IP error");
    return(geoNARow());

  } else {

    GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);

    if (gir == NULL) {
      message("Error retrieving record");
      return(geoNARow());
    }

    const char *time_zone = GeoIP_time_zone_by_country_and_region(gir->country_code, gir->region);

    Rcpp::DataFrame geodf = Rcpp::DataFrame::create(
      Rcpp::Named("country.code")=CharacterVector(mkNA(gir->country_code)),
      Rcpp::Named("country.code3")=CharacterVector(mkNA(gir->country_code3)),
      Rcpp::Named("country.name")=CharacterVector(mkNA(gir->country_name)),
      Rcpp::Named("region")=CharacterVector(mkNA(gir->region)),
      Rcpp::Named("region.name")=CharacterVector(mkNA(GeoIP_region_name_by_code(gir->country_code, gir->region))),
      Rcpp::Named("city")=CharacterVector(mkNA(gir->city)),
      Rcpp::Named("postal.code")=CharacterVector(mkNA(gir->postal_code)),
      Rcpp::Named("latitude")=NumericVector(Rcpp::wrap(gir->latitude)),
      Rcpp::Named("longitude")=NumericVector(Rcpp::wrap(gir->longitude)),
      Rcpp::Named("time.zone")=CharacterVector(mkNA(time_zone)),
      Rcpp::Named("metro.code")=IntegerVector(Rcpp::wrap(gir->metro_code)),
      Rcpp::Named("area.code")=IntegerVector(Rcpp::wrap(gir->area_code)));

    GeoIPRecord_delete(gir);

    return(geodf);
  }

}

//' Called when finished performing geolocation operations
//'
//' This frees the memory associated with the geolocation file.
//'
//' @examples
//' \dontrun{
//' geoCleanup()
//' }
//[[Rcpp::export]]
void geoCleanup() {
  if (gi != NULL) {
    GeoIP_delete(gi);
    gi = NULL ;
  }
}
