// Erik Reimert ereimertburro
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <ctype.h>
#define BUFSIZE 1024
#define MAXTHREAD 15



/* gcc -g -o main main.C -lpthread */

int tval,iTo,iFrom;
int j =0;
int c=0;
int badcnt = 0; //files that dont work count
int goodcnt = 0; //files that work count
int dircnt = 0; //directories count
int txtcnt = 0; //text files count
int snowflake = 0; //special files count
off_t rbit = 0;// bits in regular files
off_t tbit = 0;//bits in text files
pthread_t idprod[MAXTHREAD]; /* ids of threads,  */
sem_t *cunt;
sem_t *reed; /* semaphores */

//checks if file is text file
//takes in string file name and size in bytes of file
int isTxt(char* f, int val){

  char buf[BUFSIZE];
  int fdIn = 0;
  int cnt, check, i;
  int flag = 0;

  if ((fdIn = open(f, O_RDONLY)) < 0) {
    fprintf(stderr, "file open\n");
    return 0;
  }

  cnt = read(fdIn, buf, BUFSIZE);

  while(cnt= read(fdIn, buf, BUFSIZE)> 0){ // && flag != 0

    for(i =0; i < val;i++) { //iterating through bytes
      if (isprint(buf[i]) == 0 && isspace(buf[i]) == 0){
        // flag = 1;
        close(fdIn);
        return 0;
      }
    }
  }
  // if (fdIn > 0)
  close(fdIn);
  // }

  return 1;
}

//runs stat, isreg, isdir, gets file size, checks for special files
//takes in string file name and struct stat
void fcheck(char* file){
  char* filename = malloc(sizeof(char*));
  filename = file;
  // printf("this is the filename %s\n",filename);
  struct stat fil;

  int statboi = stat(filename, &fil);
  if (statboi == 0){
    if(S_ISDIR(fil.st_mode)>0){
      dircnt++;
    }else if(S_ISREG(fil.st_mode)>0){
      goodcnt++;
      rbit += fil.st_size;
      if(isTxt(filename,fil.st_size)==1){
        txtcnt++;
        tbit += fil.st_size;
      }
    }else{
      snowflake++;
    }
  }else{
    badcnt++;
  }
}

void *adder(void *whaaaat){
  char* bananero = malloc(sizeof(char*[BUFSIZE]));
  sem_wait(reed);
  bananero = (char*)whaaaat;
  fcheck(bananero); //reads file, adds values
  sem_post(cunt);

}

void main (int argc, char *argv[]){
  /////////////////////////////time stuff/////////////////////////
  struct rusage usage;
  long startTime;
  struct timeval clockStart, systemEnd;
  gettimeofday(&clockStart, NULL);
  startTime = ((clockStart.tv_sec * 1000) + (clockStart.tv_usec / 1000));
  clock_t ubeg = clock();
  time_t start = time(NULL);
  struct timeval clockEnd, userEnd;
  ///////////////////////////////////////////////////////////////

  int tval = atoi(argv[2]);
  if (tval > MAXTHREAD ){
    printf("Cant have more than 15 threads\n");
    exit(1);
  }

  int mierda = argc;
  char buf[1024] = "";
  printf("Enter file name:\n");

  cunt = malloc(sizeof(sem_t));
  reed = malloc(sizeof(sem_t));
  sem_init(cunt, 0, 1);
  sem_init(reed, 0, 1);

  if(mierda > 2){
    if(strcmp(argv[1],"thread")==0){
      char* unfuck = malloc(sizeof(char*[BUFSIZE]));

      while(fgets(buf, 1024, stdin) != NULL){

        sem_wait(cunt);
        if(buf[0] != '\n' && buf[0] != EOF){

          sscanf(buf,"%s", unfuck);

          if(j < tval){

            pthread_create(&idprod[j % tval], NULL, adder, (void *)unfuck);
            c++;
          }
          else{
            pthread_join(idprod[j % tval], NULL);
            pthread_create(&idprod[j % tval], NULL, adder, (void *)unfuck);
          }
          // printf("filename in while %s\n",unfuck);

          j++;
          // sleep(1);

        }else{
          break;
        }
        sem_post(reed);
      }
    }
  }
  else{
    //////////////////////////////////////////////////////////////////////
    /////////////////////serial mode//////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    while(fgets(buf, 1024, stdin) != NULL){
      // printf("Enter file to be used \n");

      char* poop = malloc(sizeof(char*[BUFSIZE])); //buffer to store file names
      if(buf[0] != '\n' && buf[0] != EOF){

        sscanf(buf,"%s", poop);
        //serial mode

        fcheck(poop);


      }else{break;}
    }
  }


  if (mierda > 2){
    for(int i = 0; i < c; i++){
    // printf("post mierda\n");
    (void)pthread_join(idprod[i], NULL);
    }
    gettimeofday(&clockEnd, NULL);
    long endTime = (clockEnd.tv_sec * 1000) + (clockEnd.tv_usec / 1000);
    getrusage(RUSAGE_SELF, &usage);
    userEnd = usage.ru_utime;
    systemEnd = usage.ru_stime;
    double user = ((userEnd.tv_sec * 1000) + (userEnd.tv_usec / 1000)); //user time
    double wend = endTime - startTime; //wall clock time
    double st = ((systemEnd.tv_sec * 1000) + (systemEnd.tv_usec / 1000)); //system time
    // clock_t uend = clock();
    // int usert = (uend - ubeg)/CLOCKS_PER_SEC;
    // time_t end = time(NULL);
    // int time = (end - start);
    printf("\nBad Files: %d\nDirectories: %d\nRegular Files: %d\nSpecial Files: %d\n",badcnt,dircnt,goodcnt,snowflake);
    printf("Regular File Bytes: %ld\nText Files: %d\nText File Bytes: %ld\nWall clock time: %f\nUser Time: %f\nSystem Time: %f \n",rbit,txtcnt,tbit,wend,user,st);
  }else{
    gettimeofday(&clockEnd, NULL);
    long endTime = (clockEnd.tv_sec * 1000) + (clockEnd.tv_usec / 1000);
    getrusage(RUSAGE_SELF, &usage);
    userEnd = usage.ru_utime;
    systemEnd = usage.ru_stime;
    double user = ((userEnd.tv_sec * 1000) + (userEnd.tv_usec / 1000)); //user time
    double wend = endTime - startTime; //wall clock time
    double st = ((systemEnd.tv_sec * 1000) + (systemEnd.tv_usec / 1000)); //system time
    // clock_t uend = clock();
    // int usert = (uend - ubeg)/CLOCKS_PER_SEC;
    // time_t end = time(NULL);
    // int time = (end - start);
    printf("\nBad Files: %d\nDirectories: %d\nRegular Files: %d\nSpecial Files: %d\n",badcnt,dircnt,goodcnt,snowflake);
    printf("Regular File Bytes: %ld\nText Files: %d\nText File Bytes: %ld\nWall Clock time: %f\nUser Time: %f\nSystem Time: %f\n",rbit,txtcnt,tbit,wend,user,st);
  }
}
