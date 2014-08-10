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

std::string mkNAs( const char * p ){
 return p ? std::string(p) : Rcpp::as<std::string>(NA_STRING);
}


// return a NA row for a geo record
DataFrame geoNARow(std::string ip) {

  DataFrame nadf = DataFrame::create(
    Named("ip")=CharacterVector::create(ip),
    Named("country.code")=CharacterVector::create(NA_STRING),
    Named("country.code3")=CharacterVector::create(NA_STRING),
    Named("country.name")=CharacterVector::create(NA_STRING),
    Named("region")=CharacterVector::create(NA_STRING),
    Named("region.name")=CharacterVector::create(NA_STRING),
    Named("city")=CharacterVector::create(NA_STRING),
    Named("postal.code")=CharacterVector::create(NA_STRING),
    Named("latitude")=NumericVector::create(NA_REAL),
    Named("longitude")=NumericVector::create(NA_REAL),
    Named("time.zone")=CharacterVector::create(NA_STRING),
    Named("metro.code")=IntegerVector::create(NA_INTEGER),
    Named("area.code")=IntegerVector::create(NA_INTEGER));

  return(nadf);

}

// return a NA row for a geo record
List geoNARowL(std::string ip) {

  List nalist = List::create(
    Named("ip")=CharacterVector::create(ip),
    Named("country.code")=CharacterVector::create(NA_STRING),
    Named("country.code3")=CharacterVector::create(NA_STRING),
    Named("country.name")=CharacterVector::create(NA_STRING),
    Named("region")=CharacterVector::create(NA_STRING),
    Named("region.name")=CharacterVector::create(NA_STRING),
    Named("city")=CharacterVector::create(NA_STRING),
    Named("postal.code")=CharacterVector::create(NA_STRING),
    Named("latitude")=NumericVector::create(NA_REAL),
    Named("longitude")=NumericVector::create(NA_REAL),
    Named("time.zone")=CharacterVector::create(NA_STRING),
    Named("metro.code")=IntegerVector::create(NA_INTEGER),
    Named("area.code")=IntegerVector::create(NA_INTEGER));

  return(nalist);

}


//DataFrame geoip(std::string ip) {
//
//  if (gi == NULL) {
//    message("Error opening database. Did you forget to call geofile()?");
//    return(geoNARow(ip));
//  }
//
//  uint32_t ipnum = 0;
//
//  try {
//    ipnum = boost::asio::ip::address_v4::from_string(ip.c_str()).to_ulong();
//  } catch(boost::system::system_error& error) {
//    message( "Host/IP error" );
//    return(geoNARow(ip));
//  }
//
//  if (ipnum == 0) {
//
//    message("Host/IP error");
//    return(geoNARow(ip));
//
//  } else {
//
//    GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);
//
//    if (gir == NULL) {
//      message("Error retrieving record");
//      return(geoNARow(ip));
//    }
//
//    const char *time_zone = GeoIP_time_zone_by_country_and_region(gir->country_code, gir->region);
//
//    DataFrame geodf = DataFrame::create(
//      Named("country.code")=CharacterVector(mkNA(gir->country_code)),
//      Named("country.code3")=CharacterVector(mkNA(gir->country_code3)),
//      Named("country.name")=CharacterVector(mkNA(gir->country_name)),
//      Named("region")=CharacterVector(mkNA(gir->region)),
//      Named("region.name")=CharacterVector(mkNA(GeoIP_region_name_by_code(gir->country_code, gir->region))),
//      Named("city")=CharacterVector(mkNA(gir->city)),
//      Named("postal.code")=CharacterVector(mkNA(gir->postal_code)),
//      Named("latitude")=NumericVector(Rcpp::wrap(gir->latitude)),
//      Named("longitude")=NumericVector(Rcpp::wrap(gir->longitude)),
//      Named("time.zone")=CharacterVector(mkNA(time_zone)),
//      Named("metro.code")=IntegerVector(Rcpp::wrap(gir->metro_code)),
//      Named("area.code")=IntegerVector(Rcpp::wrap(gir->area_code)));
//
//    GeoIPRecord_delete(gir);
//
//    return(geodf);
//  }
//
//}


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

//' Return a data frame of geolcation values for IPv4 address
//'
//' Uses the maxmind \code{GeoIPCity.dat} binary file to perform a geolocation for
//' a given IPv4 address and returns a data frame of geolocation records.
//' You must call \code{geofile()} before calling \code{geoip()}
//'
//' Values returned in the data frame:
//' \itemize{
//'   \item \code{ip}. original IP address (chr)
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
//' @param ip character vector of IPv4 addresses to lookup
//' @return data frame of geolocation information for the IP addresses
//' @note  vectorized
//' @examples
//' \dontrun{
//' geofile()
//' geoip(c("24.24.24.24", "42.42.42.42", "8.8.8.8"))
//' ##            ip country.code country.code3       country.name region region.name      city
//' ## 1 24.24.24.24           US           USA      United States     NY    New York Deer Park
//' ## 2 42.42.42.42           KR           KOR Korea, Republic of     NA          NA        NA
//' ## 3     8.8.8.8           US           USA      United States     NA          NA        NA
//' ##   postal.code latitude longitude        time.zone metro.code area.code
//' ## 1       11729  40.7627  -73.3227 America/New_York        501       631
//' ## 2          NA  37.5700  126.9800       Asia/Seoul          0         0
//' ## 3          NA  38.0000  -97.0000               NA          0         0
//' }
//[[Rcpp::export]]
DataFrame geoip(CharacterVector ip) {

  if (gi == NULL) {
    message("Error opening database. Did you forget to call geofile()?");
    return(DataFrame::create());
  }

  int sz = ip.size();

  CharacterVector ips(sz);
  CharacterVector iso2(sz);
  CharacterVector iso3(sz);
  CharacterVector cname(sz);
  CharacterVector reg(sz);
  CharacterVector regname(sz);
  CharacterVector city(sz);
  CharacterVector postalcode(sz);
  NumericVector lat(sz);
  NumericVector lon(sz);
  CharacterVector tz(sz);
  IntegerVector metro(sz);
  IntegerVector area(sz);

  for (int i=0; i<ip.size(); i++) {

    uint32_t ipnum = 0;

    std::string IP = Rcpp::as<std::string>(ip[i]) ;

    try {
      ipnum = boost::asio::ip::address_v4::from_string(IP.c_str()).to_ulong();
    } catch(boost::system::system_error& error) {
      message( "Host/IP error" );
      continue;
    }

    if (ipnum == 0) {

      message("Host/IP error");
      continue ;

    } else {

      GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);

      if (gir == NULL) {
        message("Error retrieving record");
        continue ;
      }

      const char *time_zone = GeoIP_time_zone_by_country_and_region(gir->country_code, gir->region);

      ips[i]=IP;
      iso2[i]=mkNAs(gir->country_code);
      iso3[i]=mkNAs(gir->country_code3);
      cname[i]=mkNAs(gir->country_name);
      reg[i]=mkNAs(gir->region);
      regname[i]=mkNAs(GeoIP_region_name_by_code(gir->country_code, gir->region));
      city[i]=mkNAs(gir->city);
      postalcode[i]=mkNAs(gir->postal_code);
      lat[i]=gir->latitude;
      lon[i]=gir->longitude;
      tz[i]=mkNAs(time_zone);
      metro[i]=gir->metro_code;
      area[i]=gir->area_code;

      GeoIPRecord_delete(gir);

    }

  }

  return(DataFrame::create(
        Named("ip")=ips,
        Named("country.code")=iso2,
        Named("country.code3")=iso3,
        Named("country.name")=cname,
        Named("region")=reg,
        Named("region.name")=regname,
        Named("city")=city,
        Named("postal.code")=postalcode,
        Named("latitude")=lat,
        Named("longitude")=lon,
        Named("time.zone")=tz,
        Named("metro.code")=metro,
        Named("area.code")=area));

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
