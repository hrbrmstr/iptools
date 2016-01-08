// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(AsioHeaders)]]

#include <Rcpp.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <asio.hpp>
#ifdef __APPLE__
#pragma clang diagnostic pop
#endif

#include "asio_bindings.h"
#include "heatmap.h"

using namespace Rcpp;

unsigned int allones = ~0;
int debug = 0;

#ifdef WIN64
#define IS_WINDOWS
#elif defined WIN32
#define IS_WINDOWS
#endif

#ifdef IS_WINDOWS
int inet_pton(int af, const char *src, unsigned int *dst);
#endif

char *l2ip(unsigned long val) {
  char conv_ip[] = "               ";
  unsigned short o1, o2, o3, o4;
  o1 = (val & (0xff << 24)) >> 24;
  o2 = (val & (0xff << 16)) >> 16;
  o3 = (val & (0xff << 8)) >> 8;
  o4 = val & 0xff;
  sprintf(conv_ip, "%hu.%hu.%hu.%hu", o1, o2, o3, o4);
  return(strdup(conv_ip));
}

unsigned long ip2l(char *ip) {
  unsigned short o1, o2, o3, o4;
  sscanf(ip, "%hu.%hu.%hu.%hu", &o1, &o2, &o3, &o4 );
  return((unsigned long)( o1 << 24 ) | ( o2 << 16 ) | ( o3 << 8 ) | o4);
}

/*
 * Parse a CIDR string
 */
int cidr_parse(const char *cidr, unsigned int *rfirst, unsigned int *rlast, int *rslash) {
  char cidr_copy[24];
  char *t;
  int slash;
  unsigned int first;
  unsigned int last;
  strncpy(cidr_copy, cidr, 24);
  t = strchr(cidr_copy, '/');
  if (NULL == t) {
    Rprintf("missing / on CIDR '%s'\n", cidr_copy);
    return 0;;
  }
  *t++ = '\0';
  slash = atoi(t);
  if (1 != inet_pton(AF_INET, cidr_copy, &first)) {
    Rprintf("inet_pton failed on '%s'\n", cidr_copy);
    return 0;
  }
  first = ntohl(first);
  if (slash < 32)
    last = first | (allones >> slash);
  else
    last = first;
  *rfirst = first;
  *rlast = last;
  *rslash = slash;
  return 1;
}

/*
 * The default the Hilbert curve order is 12.  This gives a 4096x4096
 * output image covering the entire space where each pixel represents
 * a /24.
 */
int addr_space_bits_per_image = 32;  /* /0 */
int addr_space_bits_per_pixel = 8;  /* /24 */
unsigned int addr_space_first_addr = 0;
unsigned int addr_space_last_addr = ~0;
int hilbert_curve_order = (addr_space_bits_per_image - addr_space_bits_per_pixel) / 2;

void set_morton_mode();
void set_crop(const char *);
void set_bits_per_pixel(int);


#ifndef MIN
#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)
#endif

int morton_flag = 0 ;


/*
 * Translate an IPv4 address (stored as a 32bit int) into
 * output X,Y coordinates.  First check if its within our
 * crop bounds.  Return 0 if out of bounds.
 */
unsigned int xy_from_ip(unsigned ip, unsigned *xp, unsigned *yp) {
  unsigned int s;
  if (ip < addr_space_first_addr)
    return 0;
  if (ip > addr_space_last_addr)
    return 0;
  s = (ip - addr_space_first_addr) >> addr_space_bits_per_pixel;
  xy_from_s(s, hilbert_curve_order, xp, yp);
  return 1;
}


void set_morton_mode() {
  xy_from_s = mor_xy_from_s;
}

int set_order() {
  hilbert_curve_order = (addr_space_bits_per_image - addr_space_bits_per_pixel) / 2;
  return hilbert_curve_order;
}

void set_crop(const char *cidr) {
  cidr_parse(cidr, &addr_space_first_addr, &addr_space_last_addr, &addr_space_bits_per_image);
  addr_space_bits_per_image = 32 - addr_space_bits_per_image;
  if (1 == (addr_space_bits_per_image % 2))
    Rprintf("Space to render must have even number of CIDR bits");
}

void set_bits_per_pixel(int bpp) {
  addr_space_bits_per_pixel = bpp;
  if (1 == (addr_space_bits_per_pixel % 2))
    Rprintf("CIDR bits per pixel must be even");
}


/*
 * Z_order curve calculation
 * Roy Arends
 * Nominet
 */

void mor_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp) {
  int i;
  unsigned x, y;
  x = y = 0;
  for (i = 2 * order - 2; i >= 0; i -= 2) {
    x = (x << 1) | ((s >> i) & 1);
    y = (y << 1) | ((s >> (i + 1)) & 1);
  }
  *xp = x;
  *yp = y;
}


/*
 * Figure 14-5 from Hacker's Delight (by Henry S. Warren, Jr. published by
 * Addison Wesley, 2002)
 *
 * See also http://www.hackersdelight.org/permissions.htm
 */

void hil_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp) {

  int i;
  unsigned state, x, y, row;

  state = 0;      /* Initialize. */
x = y = 0;

for (i = 2 * order - 2; i >= 0; i -= 2) {	/* Do n times. */
row = 4 * state | ((s >> i) & 3);	/* Row in table. */
x = (x << 1) | ((0x936C >> row) & 1);
y = (y << 1) | ((0x39C6 >> row) & 1);
state = (0x3E6B94C1 >> 2 * row) & 3;	/* New state. */
}
*xp = x;			/* Pass back */
*yp = y;			/* results. */
}



/*
 * Find the "bounding box" for the IPv4 netblock starting at 'first' and having
 * 'slash' netmask bits.
 *
 * For square areas this is pretty easy.  We know how to find the point diagonally
 * opposite the first value (add 1010..1010). Its a little harder for
 * rectangular areas, so I cheat a little and divide it into the two smaller
 * squares.
 */
static bbox bounding_box(unsigned int first, int slash) {
  bbox box;
  unsigned int diag = 0xAAAAAAAA;
  if (morton_flag) { diag = 0xFFFFFFFF; }
  unsigned int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

  if (slash > 31) {
    /*
     * treat /32 as a special case
     */
    xy_from_ip(first, &x1, &y1);
    box.xmin = x1;
    box.ymin = y1;
    box.xmax = x1;
    box.ymax = y1;
  } else if (0 == (slash & 1)) {
    /*
     * square
     */
    diag >>= slash;
    xy_from_ip(first, &x1, &y1);
    xy_from_ip(first + diag, &x2, &y2);
    box.xmin = MIN(x1, x2);
    box.ymin = MIN(y1, y2);
    box.xmax = MAX(x1, x2);
    box.ymax = MAX(y1, y2);
  } else {
    /*
     * rectangle: divide, conquer
     */
    bbox b1 = bounding_box(first, slash + 1);
    bbox b2 = bounding_box(first + (1 << (32 - (slash + 1))), slash + 1);
    box.xmin = MIN(b1.xmin, b2.xmin);
    box.ymin = MIN(b1.ymin, b2.ymin);
    box.xmax = MAX(b1.xmax, b2.xmax);
    box.ymax = MAX(b1.ymax, b2.ymax);
  }
  return box;
}

/*
 * Calculate the bounding box of a CIDR prefix string
 */
bbox bbox_from_cidr(const char *cidr) {
  int slash;
  unsigned int first;
  unsigned int last;
  bbox bbox;
  cidr_parse(cidr, &first, &last, &slash);
  if (first < addr_space_first_addr || last > addr_space_last_addr) {
    bbox.xmin = bbox.ymin = bbox.xmax = bbox.ymax = -1;
    return bbox;
  }
  memset(&bbox, '\0', sizeof(bbox));
  bbox = bounding_box(first, slash);

  return bbox;
}

//' IPv4 Hilbert Curve Matrix
//'
//' Returns a 4096x4096 Hilbert curve matrix for an input vector of IP
//' addresses. Generally not called directly (most of the time you'll want to
//' use \code{ipv4heatmap}), but avilable in the event another method of
//' visualization is necessary/desired.
//'
//' @param ip input character (dotted-deciman IPv4s) vector
//' @return 4096x4096 matrix, with non-0 entries being the count of IP addresses
//'         in that netblock
//' @export
//[[Rcpp::export]]
NumericMatrix ipv4matrix(CharacterVector ip) {

  unsigned int x;
  unsigned int y;

  // init hilbert stuff

  set_order() ;

  // make a matrix to hold our hilbert space & init to 0
  NumericMatrix ipmap(4096, 4096);
  std::fill(ipmap.begin(), ipmap.end(), 0);

  int ipCt = ip.size() ;
  for (int i=0; i<ipCt; i++) {

    if (0 != xy_from_ip(ip2l(ip[i]), &x, &y)) {
      ipmap(x, y) = ipmap(x, y) + 1;
    }

  }

  return(ipmap);

}

//' Bounding box from CIDR blocks
//'
//' Returns a \code{list} of bounding boxes for a given CIDR within the
//' Hilbert-curve plane
//'
//' @param cidr character vector of dotted-decimal/digit CIDRs
//' @return list of bounding box extents for each CIDR - \code{cidr},
//'         \code{xmin}, \code{ymin}, \code{xmax}, \code{ymax}
//' @export
//' @examples
//' boundingBoxFromCIDR("30.0.0.0/8")
//[[Rcpp::export]]
List boundingBoxFromCIDR(CharacterVector cidr) {

    bbox bbox;

    int cidrCt = cidr.size() ;

    List boxes(cidrCt) ;

    for (int i=0; i<cidrCt; i++) {

       bbox = bbox_from_cidr(cidr[i]);

       boxes[i] = List::create(
                          Rcpp::Named("cidr") = Rcpp::as<std::string>(cidr[i]),
                          Rcpp::Named("xmin") = bbox.xmin,
                          Rcpp::Named("ymin") = bbox.ymin,
                          Rcpp::Named("xmax") = bbox.xmax,
                          Rcpp::Named("ymax") = bbox.ymax
                          );

    }

    return(boxes);
}