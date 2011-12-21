#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ){
  int i, n;
  FILE *fp;
  char temp[100];

  if( argc!=2 ){
    printf( "Usage: sample <input_filename>\n" );
    exit( 1 );
  }

  if(( fp=fopen( argv[1], "r" ))==NULL ){
    printf( "file open error!\n" );
    exit( 1 );
  }

  do {
    fscanf( fp, "%s", temp );
    if( strcmp( "DIMENSION", temp )==0 ){
      fscanf( fp, "%s", temp );
      break;
    }
    if( strcmp( "DIMENSION:", temp )==0 ) break;
  } while( 1 );

  fscanf( fp, "%d", &n );
  printf( "%d\n", n );

  fclose( fp );
}
