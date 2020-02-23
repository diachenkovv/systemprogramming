#include<stdio.h>
#include<stdlib.h>
#include<dir.h>
#include<dos.h>

void print_info( struct ffblk *find );
char *time_conv( unsigned time, char *char_buf );
char *date_conv( unsigned date, char *char_buf );

void main( int argc, char *argv[] ) {

  struct ffblk find;

  if( !findfirst( argv[1], &find, FA_ARCH ) ) {
    printf( "\n"
	    "\nFile Name      Attr   Date         Time       Size"
	    "\n------------   ----   ----------   --------   --------" );
    print_info( &find );
  }
  else {
    printf( "\nUsage: FIND filename"
	    "\n   Wildcards  ""!""  and  ""*""  is OK.\n" );
    exit( -1 );
  }

  while( !findnext( &find ) )  print_info( &find );
  exit( 0 );
}

void print_info( struct ffblk *pfind ) {

  char timebuf[10], datebuf[12];

  date_conv( pfind->ff_fdate, datebuf );
  time_conv( pfind->ff_ftime, timebuf );

  printf( "\n%-12s   ", pfind->ff_name);
  printf( "0x%02X   %8s   %8s   %8ld ",
	  pfind->ff_attrib, datebuf, timebuf, pfind->ff_fsize );
}

char *time_conv( unsigned t, char *buf ) {
  int  h, m;

  h = ( t >> 11 ) & 0x1f, m = ( t >> 5 ) & 0x3f;
  sprintf( buf, "%2.2d:%02.2d:%02.2d", h, m, ( t & 0x1f ) * 2 );
  return( buf );
}

char *date_conv( unsigned d, char *buf ) {

  sprintf( buf, "%2.2d.%02.2d.%04.2d",
	   d & 0x1f, ( d >> 5 ) & 0x0f, ( d >> 9 ) + 1980 );
  return( buf );
}