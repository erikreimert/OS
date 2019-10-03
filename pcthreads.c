//Erik Reimert ereimertburro

/* pcthreads.C */
// #include <iostream>
// using namespace std;
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* gcc -g -o pcthreads pcthreads.C -lpthread */

#define MAXTHREAD 10

sem_t prod, cons; /* semaphores */
int n, index;
pthread_t idprod[MAXTHREAD]; /* ids of threads,  */

struct msg {
    int iFrom;
    int value;
    int cnt; //count of operations
    int tot; //total time
};

//
struct mailbox{
  int spot;
  struct msg msg;
  sem_t *cond;
  sem_t *prode;
};
struct mailbox mbarr[MAXTHREAD];

void initMailBox(int sem){
  sem_t *cons = malloc(sizeof(sem_t));
  sem_t *prod = malloc(sizeof(sem_t));
  sem_init(prod, 0, 1);
  sem_init(cons, 0, 0);
    mbarr[sem].spot = sem;
    mbarr[sem].cond = cons;
    mbarr[sem].prode = prod;
};


void SendMsg(int iTo, struct msg *pMsg){
  // use mbox array and semaphor to do this shit
  struct msg unfuck;
  unfuck.value = -6666;

      // printf("Here2 \n");
      int shit,crap;
      // crap = sem_getvalue(mbarr[iTo].prode, &shit);
      // printf("sem val %d \n", crap);
      // fflush(stdout);

  sem_wait(mbarr[iTo].prode);

  int b = pMsg->value;

      // printf("Here \n");
      fflush(stdout);

  mbarr[iTo].msg = *pMsg;
  if(b == -1){mbarr[iTo].msg = unfuck;};

        // printf("pmsg value in send %d\n", mbarr[iTo].msg.value);
        fflush(stdout);

  sem_post(mbarr[iTo].cond);

      // printf("sem post works\n\n");
      fflush(stdout);

};

void RecvMsg(int iRecv, struct msg *pMsg){
  // struct msg *unfuck = {0,6666,0,0};

  sem_wait(mbarr[iRecv].cond);
  *pMsg = mbarr[iRecv].msg;

  // if((b + 1) == 0){pMsg = &unfuck;};
      int b = pMsg->value;
      int a = mbarr[iRecv].msg.value;
      // printf("message value received %d\nmessage value in pmsg @receive %d\nspot in array for mailbox %d \n\n", a, b, mbarr[iRecv].spot);
      fflush(stdout);

  sem_post(mbarr[iRecv].prode);

      // printf("recv posted");
      // fflush("stdout");
};

// recv message and send
void *adder(void *whaaaat){
  //timer
  time_t start = time(NULL);
  int this = (long)whaaaat;
  int ops = 0;
  struct msg *fake = malloc(sizeof(struct msg));
  int japiro = 0;

while(1){
  RecvMsg(this, fake);

        // printf("index at adder %ld \n",this);
        fflush(stdout);

        // printf("message arrived at adder %d\n",fake->value);
        // fflush(stdout);

  if(fake->value >= 0){

        // printf("adder recv works:\nmessage in adder not EOF section  %d\n", fake->value);
        // fflush(stdout);

    ops++;
    japiro += fake->value;
    sleep(1);
  }else {
      // timer end
      // printf("inside -1\n" );
      // fflush(stdout);
      time_t end = time(NULL);
      //timer end minus start
      int time = (end - start);
      struct msg* sorete = malloc(sizeof(struct msg));
      sorete->tot = time;
      sorete->iFrom = this;
      sorete->value = japiro;
      sorete->cnt = ops;
      SendMsg(0, sorete);
      return 0;
    }
  }
};

main(int argc, char *argv[])
{
  int tval = atoi(argv[1]);

  struct msg *poronga = malloc(sizeof(struct msg));
  if (tval > MAXTHREAD ){
  printf("Cant have more than 10 threads\n");
  return 1;
}
initMailBox(0);
for(int i = 0; i < tval ; i++){
  index = i;
  initMailBox(i+1);
  pthread_create(&idprod[i], NULL, adder, (void *)index+1);
};
  printf("Enter messages and threads to be used \n");
    while(1){
    char buf[100] = "";
    int poop, pee;
    fgets(buf, 100, stdin);
    if(buf[0] != '\n' && buf[0] != EOF){
      sscanf(buf,"%d %d", &poop, &pee); //poop first value, pee second value
        if(pee > tval || (poop + pee) < poop){
          perror("Wrong input");
        }
        poronga->value = poop;
      // printf("%c %c %d %d\n",buf[0], buf[2], poop, pee);
      SendMsg(pee, poronga);
    }
    else if(buf[0] == '\n' || buf[0] == EOF){
        poronga->value = -1;
      for(int i = 1; i < tval+1; i++){
        SendMsg(i, poronga);
      }
      break;
    }
  }

    for(int i = 0; i< tval;i++){
      RecvMsg(0, poronga);
      printf("The result from thread %d is %d from %d operations during %d secs \n",poronga->iFrom, poronga->value, poronga->cnt, poronga->tot);

    }

    for(int i = 0; i < tval -1; i++){
    (void)pthread_join(idprod[i], NULL);
    // (void)sem_destroy(&cons);
    // (void)sem_destroy(&prod);
  }
  exit(1);
  return;
}
