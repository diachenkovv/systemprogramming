/****************************************************************************
* COPY1.C - Программа копирования файлов, использующая функции работы с file
*           handles
* Date:  05.02.96
* (C)
****************************************************************************/
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <alloc.h>
#include <errno.h>

#define  BUFSIZE  10000


/***************************************************************************/
void main( int argc, char **argv ) {
  int   source, target;
  int   i;
  char  *buffer;
  int   count;

  if( argc != 3 ) {
    printf( "\n"
            "Usage: COPY1 [d:][\\path]source_file [d:][\\path]target_file\n" );
    exit( 1 );
  } /* если переменная argc не равна 3 то выполняется составной оператор */

  if( ( source = open( argv[ 1 ], O_BINARY | O_RDONLY ) ) == -1 ) {
    printf( "\nOpen source file error: %d", errno );
    exit( 2 );
  }
  target = open( argv[ 2 ], O_BINARY | O_WRONLY | O_CREAT | O_EXCL,
                 S_IREAD | S_IWRITE );
  if( errno == EEXIST ) {
    printf( "\nFile already exists. Overwrite? (Y/N)\n" );
    i = getch();
    if( ( i == 'y' ) || ( i == 'Y' ) )
      target = open( argv[ 2 ], O_BINARY | O_WRONLY | O_CREAT | O_TRUNC,
                     S_IREAD | S_IWRITE );
  }
  if( target == -1 ) {
    printf( "\nOpen target file error: %d", errno );
    exit( 2 );
  }
  count = BUFSIZE;
  if( ( buffer = ( char* )malloc( count ) ) == NULL ) {
    printf( "\nNot enough memory" );
    exit( 3 );
  }

  while( !eof( source ) ) {
    if( ( count = read( source, buffer, count ) ) == -1 ) {
      printf( "\nRead file error: %d", errno );
      exit( 4 );
    }
    if( ( count = write( target, buffer, count ) ) == -1 ) {
      printf( "\nWrite file error: %d", errno );
      exit( 5 );
    }
  }
  close( source );
  close( target );
  free( buffer );
}
/* end of file copy1.c */