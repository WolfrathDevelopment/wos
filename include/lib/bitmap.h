#ifndef WOS_BITMAP_H
#define WOS_BITMAP_H

#define SET_BIT(x, n)		((x) |=  (1 << n))
#define CLR_BIT(x, n)		((x) &= ~(1 << n))
#define IS_BIT_SET(x, n)	((x) &   (1 << n))

#endif
