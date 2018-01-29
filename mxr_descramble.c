#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

//mxr_descramble - converts an MXR185 ROM file into a playable raw PCM file
// sdh7 - 1/29/2018
//arguments:
// -4 ROM is 4096 char instead of 2048
// -i infile input file (required)
// -o outfile output file (required)

int mxr2raw (int );

int main (int argc, char *argv[]){
  char inrom[4096];
  char outrom[4096];
  int c,readsize,i;
  int fourk=0;
  char *infile=NULL;
  char *outfile=NULL;
  FILE *ifp,*ofp;

//get options:
  while ((c = getopt (argc, argv, "4i:o:")) != -1){
    switch (c){
      case '4':
       fourk=1;
       break;
      case 'i':
       infile=optarg;
       break;
      case 'o':
       outfile=optarg;
       break;
      case '?':
        if ((optopt=='i') || (optopt=='o'))
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	 else if (isprint (optopt))
	   fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	 else 
	 fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	 return 1;
      default:
        abort();
    }
  }

  printf("input file: %s, output file: %s\n",infile,outfile);
  if (fourk==0){
    printf("2716 ROM image\n");
    readsize=2048;
  } else {
    printf("2732 ROM image\n");
    readsize=4096;
  }

//open the file, read into array
  ifp=fopen(infile,"r");
  if (ifp){
    fread(inrom, sizeof(char), readsize,ifp);
    fclose(ifp);
  }

//for sizeof array, process each byte of the ROM and get the appropriate byte. write byte to output array.
  for (i=0; i<readsize; i++){
    outrom[i]=inrom[mxr2raw(i)];
  }

//write output array to outfile
   ofp=fopen(outfile,"w");
   if (ofp){
     fwrite(outrom,sizeof(char), readsize, ofp);
     fclose(ofp); 
   }

return 0;
}

int mxr2raw(int pos){
  int rtn=0;
  //Q1
  if (pos & 1){
    rtn=rtn+256;
  }
  //Q2
  if (pos & 2){
    rtn=rtn+512;
  }
  //Q3
  if (pos & 4){
    rtn=rtn+1024;
  }
  //Q4
  if (pos & 8){
    rtn=rtn+2;
  }
  //Q5
  if (pos & 16){
    rtn=rtn+32;
  }
  //Q6
  if (pos & 32){
    rtn=rtn+128;
  }
  //Q7
  if (pos & 64){
    rtn=rtn+8;
  }
  //Q8
  if (pos & 128){
    rtn=rtn+4;
  }
  //Q9
  if (pos & 256){
    rtn=rtn+1;
  }
  //Q10
  if (pos & 512){
    rtn=rtn+16;
  }
  //Q11
  if (pos & 1024){
    rtn=rtn+64;

  }
  //Q12
  if (pos & 2048){
    rtn=rtn+2048;
  }
  return rtn;
}

