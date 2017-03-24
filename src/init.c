#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME:
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP iptools_expand_ipv6(SEXP);
extern SEXP iptools_hilbert_encode(SEXP, SEXP);
extern SEXP iptools_hostname_to_ip(SEXP);
extern SEXP iptools_ip_classify(SEXP);
extern SEXP iptools_ip_in_any(SEXP, SEXP);
extern SEXP iptools_ip_in_range(SEXP, SEXP);
extern SEXP iptools_ip_numeric_to_binary_string(SEXP);
extern SEXP iptools_ip_to_binary_string(SEXP);
extern SEXP iptools_ip_to_hostname(SEXP);
extern SEXP iptools_ip_to_numeric(SEXP);
extern SEXP iptools_is_multicast(SEXP);
extern SEXP iptools_numeric_to_ip(SEXP);
extern SEXP iptools_range_boundaries(SEXP);
extern SEXP iptools_v6_scope(SEXP);
extern SEXP iptools_validate_range(SEXP);
extern SEXP iptools_xff_extract(SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"iptools_expand_ipv6",                 (DL_FUNC) &iptools_expand_ipv6,                 1},
    {"iptools_hilbert_encode",              (DL_FUNC) &iptools_hilbert_encode,              2},
    {"iptools_hostname_to_ip",              (DL_FUNC) &iptools_hostname_to_ip,              1},
    {"iptools_ip_classify",                 (DL_FUNC) &iptools_ip_classify,                 1},
    {"iptools_ip_in_any",                   (DL_FUNC) &iptools_ip_in_any,                   2},
    {"iptools_ip_in_range",                 (DL_FUNC) &iptools_ip_in_range,                 2},
    {"iptools_ip_numeric_to_binary_string", (DL_FUNC) &iptools_ip_numeric_to_binary_string, 1},
    {"iptools_ip_to_binary_string",         (DL_FUNC) &iptools_ip_to_binary_string,         1},
    {"iptools_ip_to_hostname",              (DL_FUNC) &iptools_ip_to_hostname,              1},
    {"iptools_ip_to_numeric",               (DL_FUNC) &iptools_ip_to_numeric,               1},
    {"iptools_is_multicast",                (DL_FUNC) &iptools_is_multicast,                1},
    {"iptools_numeric_to_ip",               (DL_FUNC) &iptools_numeric_to_ip,               1},
    {"iptools_range_boundaries",            (DL_FUNC) &iptools_range_boundaries,            1},
    {"iptools_v6_scope",                    (DL_FUNC) &iptools_v6_scope,                    1},
    {"iptools_validate_range",              (DL_FUNC) &iptools_validate_range,              1},
    {"iptools_xff_extract",                 (DL_FUNC) &iptools_xff_extract,                 2},
    {NULL, NULL, 0}
};

void R_init_iptools(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}