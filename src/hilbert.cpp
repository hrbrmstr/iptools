#include <Rcpp.h>

using namespace Rcpp;

int addr_space_bits_per_image = 32;
int addr_space_bits_per_pixel = 8;	/* /24 */
unsigned int addr_space_first_addr = 0;
unsigned int addr_space_last_addr = ~0;

//' Encode an IPv4 address to Hilbert space
//'
//' @param x IPv4 address
//' @param bpp Hilbert depth (max 12)
//' @export
// [[Rcpp::export]]
NumericMatrix hilbert_encode(std::vector<unsigned> x, int bpp=8) {

  NumericMatrix res(x.size(), 2);

  int order = (addr_space_bits_per_image - bpp) / 2;

  for (unsigned n=0; n<x.size(); n++) {

    unsigned s = ((unsigned)x[n] - addr_space_first_addr) >> bpp;

    unsigned state=0, x=0, y=0, row=0;
    int i;

    for (i=2*order-2; i>=0; i-=2) {
      row = 4 * state | ((s >> i) & 3);
      x = (x << 1) | ((0x936C >> row) & 1);
      y = (y << 1) | ((0x39C6 >> row) & 1);
      state = (0x3E6B94C1 >> 2 * row) & 3;
    }

    res(n, 0) = x;
    res(n, 1) = y;

  }

  return(res);

}
