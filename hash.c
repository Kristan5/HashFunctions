#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE	1024

int collisions;
int duplicates;

struct record
{
  char *last_name;
  char *first_name;
  char *license_no;
  char *license_type;
  char *issue_date;
};

struct array
{
  struct record *arr;
  int nelements;
  struct record **hash;
  int hash_size;
};

int char2int( unsigned char c )
{
  if ( isupper(c) )
  {
    return (int)(c - 'A');
  }
  if ( islower(c) )
  {
    return (int)(c - 'a');
  }
  return 26;
}

int str2int( char *s, int max )
{
  char *c;
  unsigned long number, column, new;

  column = 1;
  number = 0;
  for (c=s;(*c);c++)
  {
    number += char2int(*c) * column;
    column *= 27;
  }

  new = 0;
  while (number)
  {
    new = ( number + (new % max) ) % max;
    number = number / max;  
  }

  return (int)new;
}

struct array *read_records()
{
  char buffer[BUFFER_SIZE];

  struct array *arrptr;

  FILE *fp;

  int line, start, end;

  arrptr = malloc( sizeof( struct array ) );
  arrptr->nelements = 0;

  fp = fopen( "Professional_and_Occupational_Licensing.tsv", "r" );
  fgets( buffer, BUFFER_SIZE, fp );

  while (!feof(fp))
  {
    if ( fgets( buffer, BUFFER_SIZE, fp )==NULL )
    {
      break;
    }

    if ( strlen(buffer)==BUFFER_SIZE-1 )
    {
      fprintf( stderr, "Error:  BUFFER TOO SMALL\n" );
      exit(-1);
    }

    (arrptr->nelements)++;
  }

  arrptr->arr = malloc( sizeof( struct record ) * (arrptr->nelements) );

  fseek( fp, 0, SEEK_SET );

  fgets( buffer, BUFFER_SIZE, fp );
  for (line=0;line<arrptr->nelements;line++)
  {
    fgets( buffer, BUFFER_SIZE, fp );

    start = 0;
    for (end=start;buffer[end]!='\t';end++); /* find next comma last_name*/

    (arrptr->arr)[line].last_name = malloc( end - start + 1 );
    strncpy( (arrptr->arr)[line].last_name, buffer+start, end-start );
    (arrptr->arr)[line].last_name[end-start] = '\0';
    start = end+1;      

  

    /*printf("Last Name: %s\n",(arrptr->arr)[line].last_name);*/
    for (end=start;buffer[end]!='\t';end++); /* find next comma first_name*/

    (arrptr->arr)[line].first_name = malloc( end - start + 1 );
    strncpy( (arrptr->arr)[line].first_name, buffer+start, end-start );
    (arrptr->arr)[line].first_name[end-start] = '\0';
    /*printf("First Name: %s\n",(arrptr->arr)[line].first_name);*/

    

    start = end+1;
    /*
    if (strcmp((arrptr->arr)[line].first_name, "") != 0){
      for (end=start;buffer[end]!='\t';end++); 

      start = end+1;
    }
    */
    for (end=start;buffer[end]!='\t';end++); /* find next comma combined_name*/

    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma licensed_no*/

    (arrptr->arr)[line].license_no = malloc( end - start + 1 );
    strncpy( (arrptr->arr)[line].license_no, buffer+start, end-start );
    (arrptr->arr)[line].license_no[end-start] = '\0';
    /*printf("License No: %s\n",(arrptr->arr)[line].license_no);*/
    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma profession_id*/

    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma license_type*/

    (arrptr->arr)[line].license_type = malloc( end - start + 1 );
    strncpy( (arrptr->arr)[line].license_type, buffer+start, end-start );
    (arrptr->arr)[line].license_type[end-start] = '\0';
    /*printf("License type: %s\n",(arrptr->arr)[line].license_type);*/
    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma city*/

    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma state*/

    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma zipcode*/
    
    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma country*/

    start = end+1;
    for (end=start;buffer[end]!='\t';end++); /* find next comma issue_date*/

    (arrptr->arr)[line].issue_date = malloc( end - start + 1 );
    strncpy( (arrptr->arr)[line].issue_date, buffer+start, end-start );
    (arrptr->arr)[line].issue_date[end-start] = '\0';
    /*printf("Issue Date: %s\n",(arrptr->arr)[line].issue_date);*/
    
    
    /*printf("--------------\n");*/
  }


  return arrptr;
}


void build_hash( struct array *arrptr, int hash_size )
{
  int idx, line;
  int duplicate;

  arrptr->hash_size = hash_size;
  arrptr->hash = malloc( sizeof( struct record * ) * arrptr->hash_size );

  for (idx=0;idx<arrptr->hash_size;idx++)
  {
    (arrptr->hash)[idx] = NULL;
  }

  for (line=0;line<arrptr->nelements;line++)
  {
    printf( "%d Adding %s\n", line, (arrptr->arr)[line].license_no );
    idx = hash3( (arrptr->arr)[line].issue_date, arrptr->hash_size );
    duplicate = 0;
    while ( (arrptr->hash)[idx]!=NULL )
    {
      if ( strcmp( ((arrptr->hash)[idx])->issue_date,(arrptr->arr)[line].issue_date ) == 0 )
      {
      	printf( "  Skipping duplicate\n" );
      	duplicates++;
      	duplicate = 1;
	       break;
      }
      printf( "  collision at %d %s\n", idx, ((arrptr->hash)[idx])->issue_date );
      collisions++;
      idx++;
      if (idx>=arrptr->hash_size)
      {
	idx = 0;
      }
      
    }
    if (!duplicate)
    {
      printf("  inserting at %d\n", idx );
      (arrptr->hash)[idx] = (arrptr->arr)+line;
    }
  }
}

void free_array_ptr( struct array *ptr )
{
  int i;

  for (i=0;i<ptr->nelements;i++)
  {
    free( ptr->arr[i].last_name );
    free( ptr->arr[i].first_name );
    free( ptr->arr[i].license_type );
  }

  free( ptr->arr );
  free( ptr->hash );

  free( ptr );
}

struct record *find( char *key, struct array *arrptr )
{
  int idx;

  idx = hash1( key, arrptr->hash_size );
  printf("Trying to find in: %d\n", idx);
  while ( (arrptr->hash)[idx] != NULL )
  {
    if ( strcmp( key, ((arrptr->hash)[idx])->issue_date )==0 )
    {
      return (arrptr->hash)[idx];
    }
    idx++;

    if (idx>=(arrptr->hash_size))
    {
      idx = 0;
    }
  }
  return NULL;

}


int main()
{
  struct array *arrptr;
  /*int line; */
  struct record *r;

  collisions = 0;
  duplicates = 0;
  arrptr = read_records();

  build_hash( arrptr, 500000 );
  /*
  for (line=0;line<arrptr->nelements;line++)
  {
    printf( " %d %s, %s: %s %d\n", line,
            	                    (arrptr->arr)[line].last_name,
            	                    (arrptr->arr)[line].first_name,
            			                (arrptr->arr)[line].license_type,
                   			          str2int( (arrptr->arr)[line].last_name, 100 ) );
  }
  */
  printf( "Duplicates: %d\n", duplicates );
  printf( "Collisions: %d\n", collisions );

  r = find( "10/19/1999", arrptr );
  if (r==NULL)
  {
    printf( "Not found\n" );
  }
  else
  {
    printf( "%s, %s : %s : %s : %s\n", r->last_name,
                                       r->first_name,
          			                       r->license_type, 
                                       r->license_no,
                                       r->issue_date ); 
  }

  free_array_ptr( arrptr );
  return 0;
}
