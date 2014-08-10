#include <Rcpp.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "GeoIP.h"
#include "GeoIPCity.h"

using namespace Rcpp;

extern Function message;

GeoIP *gi = NULL;
GeoIP *giASN = NULL;

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


//' Initializes the MaxMind library and opens the \code{GeoLiteCity.dat} & \code{GeoLiteASNum.dat} file
//'
//' This function must be called before performing a lookup with \code{geoip()} or \code{asnip()}.
//' Each full path spec prefix defaults to \code{/usr/local/share/GeoIP/}
//' and can be overriden by changing the value of the \code{citydata}  & \code{asndata} parameters.
//'
//' NOTE: You must manually retrieve the \code{GeoLiteCity.dat.gz} \code{GeoLiteASNum.dat.gz} files from MaxMind
//' via \url{http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz} &
//' \url{http://geolite.maxmind.com/download/geoip/database/GeoLiteASNum.dat.gz}  and
//' unzip each to the locations you will be specifying in the parameters.
//'
//' Any errors reading the files will result in a console message.
//'
//' @param citydata full path (including filename) to the \code{GeoLiteCity.dat} file
//' @param asndata full path (including filename) to the \code{GeoLiteASNum.dat} file
//' @examples
//' \dontrun{
//' maxmindinit()
//' }
//[[Rcpp::export]]
void maxmindinit(std::string citydata="/usr/local/share/GeoIP/GeoLiteCity.dat",
             std::string asndata="/usr/local/share/GeoIP/GeoIPASNum.dat") {

  if (gi == NULL) {
     gi = GeoIP_open(citydata.c_str(), GEOIP_INDEX_CACHE);
     if (gi == NULL) {
      message("Error opening city database");
     }
  }

  if (giASN == NULL) {
     giASN = GeoIP_open(asndata.c_str(), GEOIP_STANDARD);
     if (giASN == NULL) {
      message("Error opening ASN database");
     }
  }

}

//' Return a data frame of geolcation values for IPv4 address
//'
//' Uses the maxmind \code{GeoIPCity.dat} binary file to perform a geolocation for
//' a given IPv4 address and returns a data frame of geolocation records.
//' You should call \code{maxmindinit()} before calling \code{geoip()}, but \code{geoip}
//' will make a last-ditch effort to load the city database from the default location
//' in the event you forget.
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
//' @param showMessages show/hide console messages (bool) default: do not show messages
//' @return data frame of geolocation information for the IP addresses
//' @note  vectorized
//' @examples
//' \dontrun{
//' maxmindinit()
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
DataFrame geoip(CharacterVector ip, bool showMessages=false) {

  if (gi == NULL) {
    maxmindinit();
    if (gi == NULL) {
      if(showMessages) message("Error opening database. Did you forget to call madmindinit()?");
      return(DataFrame::create());
    }
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

    iso2[i]=NA_STRING;
    iso3[i]=NA_STRING;
    cname[i]=NA_STRING;
    reg[i]=NA_STRING;
    regname[i]=NA_STRING;
    city[i]=NA_STRING;
    postalcode[i]=NA_STRING;
    lat[i]=NA_REAL;
    lon[i]=NA_REAL;
    tz[i]=NA_STRING;
    metro[i]=NA_INTEGER;
    area[i]=NA_INTEGER;

    ips[i]=IP;

    try {
      ipnum = boost::asio::ip::address_v4::from_string(IP.c_str()).to_ulong();
    } catch(boost::system::system_error& error) {
      if(showMessages) message( "Host/IP error" );
      continue;
    }

    if (ipnum == 0) {

      if (showMessages) message("Host/IP error");
      continue ;

    } else {

      GeoIPRecord *gir = GeoIP_record_by_ipnum(gi, ipnum);

      if (gir == NULL) {
        if(showMessages) message("Error retrieving record");
        continue ;
      }

      const char *time_zone = GeoIP_time_zone_by_country_and_region(gir->country_code, gir->region);

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

//' Return a data frame of IPv4 to ASN & org mappings
//'
//' Uses the maxmind \code{GeoIPASNum.dat} binary file to perform an AS number & org
//' identification for a given IPv4 address and returns a data frame of results.
//' You should call \code{maxmindinit()} before calling \code{asnip()}, but \code{asnip}
//' will make a last-ditch effort to load the ASNum database from the default location
//' in the event you forget.
//'
//' Values returned in the data frame:
//' \itemize{
//'   \item \code{ip}. original IP address (chr)
//'   \item \code{asn}. ASN (chr)
//'   \item \code{org}. Assigned org (chr)
//' }
//'
//' @param ip character vector of IPv4 addresses to lookup
//' @param includeAS (bool) whether to include or not include the "AS" prefix in the \code{asnum} column (default TRUE)
//' @param showMessages show/hide console messages (bool) default: do not show messages
//' @return data frame of AS # & org information for the IP addresses
//' @note vectorized
//' @examples
//' \dontrun{
//' maxmindinit()
//' set.seed(1000000); asnip(randomIPs(10))
//' ##                 ip     asn                         org
//' ## 1       70.5.34.39  AS3651                      Sprint
//' ## 2    79.32.183.102  AS3269       Telecom Italia S.p.a.
//' ## 3     70.166.53.78 AS36801   New Wave Industries, Inc.
//' ## 4  131.199.143.169  AS1341 Allen-Bradley Company, Inc.
//' ## 5    160.150.52.98  AS1515        Headquarters, USAISC
//' ## 6   242.119.216.60    <NA>                        <NA>
//' ## 7     53.55.24.145 AS31399   Daimler Autonomous System
//' ## 8   151.141.222.29 AS19956          BellSouth.net Inc.
//' ## 9  243.118.250.204    <NA>                        <NA>
//' ## 10   83.173.28.251 AS31441  Gagnaveita Reykjavikur ehf
//' }
//[[Rcpp::export]]
DataFrame asnip(CharacterVector ip, bool includeAS=true, bool showMessages=false) {

  if (giASN == NULL) {
    maxmindinit();
    if (giASN == NULL) {
      if(showMessages) message("Error opening database. Did you forget to call maxmindinit()?");
      return(DataFrame::create());
    }
  }

  int sz = ip.size();

  CharacterVector ips(sz);
  CharacterVector asns(sz);
  CharacterVector orgs(sz);

  for (int i=0; i<ip.size(); i++) {

    uint32_t ipnum = 0;

    std::string IP = Rcpp::as<std::string>(ip[i]) ;

    asns[i]=NA_STRING;
    orgs[i]=NA_STRING;

    ips[i]=IP;

    try {
      ipnum = boost::asio::ip::address_v4::from_string(IP.c_str()).to_ulong();
    } catch(boost::system::system_error& error) {
      if(showMessages) message( "Host/IP error" );
      continue;
    }

    if (ipnum == 0) {

      if (showMessages) message("Host/IP error");
      continue ;

    } else {

      char *org = GeoIP_org_by_ipnum(giASN, ipnum);

      if (org == NULL) {
        if(showMessages) message("Not found");
        continue ;
      }

      std::string Org = std::string(GeoIP_org_by_ipnum(giASN, ipnum)) ;
      std::string AS = Org.substr(0, Org.find(" ")) ;
      Org = Org.substr(Org.find(" ")+1);

      if (!includeAS) {
        boost::replace_all(AS, "AS", "");
      }

      asns[i]=mkNAs(AS.c_str());
      orgs[i]=mkNAs(Org.c_str());

    }

  }

  return(DataFrame::create(
    Named("ip")=ips,
    Named("asn")=asns,
    Named("org")=orgs));

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

  if (giASN != NULL) {
    GeoIP_delete(giASN);
    giASN = NULL ;
  }

}
