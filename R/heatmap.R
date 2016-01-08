#' Create an IPv4 Hilbert curve heatmap
#'
#' Return a minimally annotated \code{ggplot2} object of a Hilbert-space heatmap for
#' a set of IPv4 addresses.
#'
#' @param ips character vector of IPv4 addresses
#' @param colors character vector (5 elements) of colors to be used in the plot.
#'        Each color maps to the number of IP addresses in the netblock (~log scale).
#'        By default, it will use ColorBrewer "PuOr" range
#' @param cb_pal named RColorBrewer palette to use (using \code{colors} overrides
#'        any value used here)
#' @param alpha scale pixel alpha along with color
#' @param legend if you want the legend, then set this to TRUE.
#' @return list containing a ggplot2 object (\code{gg}) and the x,y point data
#'         (\code{dt}) - which is a \code{data.table}).
#' @export
#' @examples
#' ips <- c("37.113.19.46", "3.188.210.86", "81.97.190.213", "36.242.75.255",
#' "37.85.209.22", "79.246.149.132", "124.43.169.59", "15.114.34.91",
#' "99.202.51.14", "97.211.252.39", "78.43.125.169", "28.245.39.90",
#' "12.80.98.188", "1.139.230.141", "113.133.95.189", "98.194.96.20",
#' "109.57.232.176", "47.49.35.6", "14.187.171.0", "62.186.25.75")
#' hm <- ipv4heatmap(ips)
#'
#' ## plot it
#' # hm$gg
#'
#' ## inspect the points data.table:
#' str(hm$dt)
ipv4heatmap <- function(ips, colors=NA, cb_pal="PuOr", alpha=FALSE, legend=FALSE) {

  # only takes valid IPv4 addresses

  if (typeof(ips) != "character") {
    mx <- ipv4matrix_l(ips)
  } else {
    mx <- ipv4matrix(grep("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$", ips, value=TRUE))
  }

  dt <- data.table(which(mx!=0, arr.ind=TRUE), val=mx[mx!=0])
  setkeyv(dt, c("row", "col"))
  dt$color <- cut(dt$val,
                  breaks=c(0, 5, 15, 40, 100, 255),
                  include.lowest=TRUE,
                  labels=c("1-5", "6-15", "16-40", "41-100", "101-255"))

  if (anyNA(colors)) {
    hilbcols <- brewer.pal(5, cb_pal)
  } else {
    hilbcols <- colors
  }

  gg <- ggplot()

  suppressMessages({

    if (alpha) {
      gg <- gg + geom_point(data=dt,
                            aes(x=row, y=col, color=color, alpha=color),
                            size=0.1)
    } else {
      gg <- gg + geom_point(data=dt,
                            aes(x=row, y=col, color=color),
                            size=0.1)
    }

    gg <- gg + labs(x=NULL, y=NULL, title=NULL)
    gg <- gg + scale_color_manual(values=hilbcols)
    gg <- gg + scale_x_continuous(expand=c(0,0))
    gg <- gg + scale_y_reverse(expand=c(0,0))
    gg <- gg + coord_equal(xlim=c(0, 4095), ylim=c(0, 4095))
    gg <- gg + theme_bw()

    if (legend == FALSE) {
      gg <- gg + theme(legend.position="none")
    }

    gg <- gg + theme(panel.grid=element_blank())
    gg <- gg + theme(panel.border=element_blank())
    gg <- gg + theme(axis.ticks=element_blank())
    gg <- gg + theme(axis.text=element_text(size=0, margin=unit(0, "null")))
    gg <- gg + theme(axis.text=element_blank())
    gg <- gg + theme(axis.title=element_blank())
    gg <- gg + theme(axis.line=element_blank())
    gg <- gg + theme(panel.background=element_rect(fill="black"))
    gg <- gg + theme(plot.margin=unit(c(0,0,0,0), "null"))
    gg <- gg + theme(axis.ticks.length=unit(0, "null"))

  })

  return(list(gg=gg, dt=dt))

}

color <- NULL
