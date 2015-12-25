#include "myqueue.h"
#include <stdlib.h>

int init_fd=0;
char charsem[32]="abcdefghijklmnopqrstuvwxyzABCDEF"; 
sem_t sread,swrite,sema;

mqd_t mq_open(const char* nom, int mq_flags, int mode){
  
  int fd,i,cours;
  struct mq_attr *sp;
  
  if(init_fd==0){
    if((fd=shm_open(nom,mq_flags,mode))==-1){
      perror("shm open");
      exit(-1);
    }
    if(ftruncate(fd,sizeof(struct mq_attr))==-1){
      perror("ftruncate");
      exit(-1);
    }
    printf("Init\n");

  }

  if ((sp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    exit(-1);

  } 
   
   
  printf("FD:%d\n",fd);

 
  sp->mq_flags=(mq_flags&O_NONBLOCK)?1:0;
  sp->mq_maxmsg=32;
  sp->mq_msgsize=32;
  sp->mq_curmsgs=0;

  for(i=0;i<32;i++){
    sp->blocked_writers[i]=charsem[i];
  }

  for(i=0;i<32;i++){
    sp->blocked_readers[i]=charsem[i];
  }
  
  
  if(sem_init(&sread,1,1)==-1){
      perror("sem open READ");
      return -1;
    }
 if(sem_init(&swrite,1,1)==-1){
      perror("sem open WRITE");
      return -1;
    }

 if(sem_init(&sema,1,1)==-1){
      perror("sem open SEMA");
      return -1;
    }

 
  
  sp->nb_blocked_writers=0;
  sp->nb_blocked_readers=0;
  sp->subscriber=0;
  sp->notification_sig=-1;
  printf("OPEN FINISHED\n");
  return fd;
}
   
int mq_close(mqd_t fd){
 struct mq_attr *tmp;
 if ((tmp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  munmap(tmp,sizeof(struct mq_attr));
  return 0;
}

int mq_unlink(const char* nom){
  struct mq_attr *tmp;
  int fd,i;

  if((fd=shm_open(nom,O_RDWR,0660))==-1){
    perror("shm open");
    return -1;
  }
  
  if ((tmp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  
  sem_destroy(&swrite);
  sem_destroy(&sread);
  sem_destroy(&sema);
  shm_unlink(nom);
  return 0;
}

int mq_getattr(mqd_t fd, struct mq_attr *attr){
  struct mq_attr *tmp;
  
  
  if ((tmp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    return -1;
  } 

  attr=tmp;

  return 0;
}

int mq_setattr(mqd_t fd, struct mq_attr *new, struct mq_attr *old){
  int i;

  if(old==NULL)
    mq_getattr(fd,old);

  old->mq_flags=(new->mq_flags & O_NONBLOCK) ? 1 : 0;
  
  //old->mq_flags=new->mq_flags;
  old->mq_maxmsg=new->mq_maxmsg;
  old->mq_msgsize=new->mq_msgsize;
  old->mq_curmsgs=new->mq_curmsgs;
  old->nb_blocked_writers=new->nb_blocked_writers;
  old->nb_blocked_readers=new->nb_blocked_readers;
  old->subscriber=new->subscriber;
  old->notification_sig=new->notification_sig;
  
  for(i=0;i>32;i++){
    old->queue[i]=new->queue[i];
  }
  strcpy(old->blocked_writers,new->blocked_writers);
  strcpy(old->blocked_readers,new->blocked_readers);
  return 0;
}

int mq_send(mqd_t fd, const char *msg_ptr, size_t msg_len, unsigned msg_prio){
  
  struct mq_attr *tmp;
  int i;

  if ((tmp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    return -1;
  } 

  /* while((tmp->mq_curmsgs+1 > tmp->mq_maxmsg) || (tmp->mq_curmsgs == tmp->mq_maxmsg)); */
  /*   sem_wait(&sema); */
  /* } */
  
  

  if(tmp->mq_msgsize < msg_len){
    perror("Message trop long");
    return -1;
  }

  i=tmp->mq_curmsgs;
  if(i>0){
    while(i>=0 && msg_prio > (tmp->queue[i].prio)){
      tmp->queue[i+1]=tmp->queue[i];
      i--;
    }
  }
  sem_wait(&sema);
  sem_wait(&swrite);
  strcpy(tmp->queue[i].data,msg_ptr);
  tmp->queue[i].prio=msg_prio;
  tmp->queue[i].length=msg_len;
  tmp->mq_curmsgs++;
  sem_post(&swrite);
  sem_post(&sema);

  return 0;
}

ssize_t mq_receive(mqd_t fd, char *msg_ptr, size_t msg_len, unsigned *msg_prio){
  struct mq_attr *tmp;
  int i,len,j;
  
  if ((tmp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    return -1;
  } 
  
  if(tmp->mq_curmsgs==0){
    perror("La queue est vide");
    return -1;
  }
  sem_wait(&sema);
  sem_wait(&sread);
  strcpy(msg_ptr,tmp->queue[0].data);
  len=tmp->queue[0].length; 
  msg_len=tmp->queue[0].length;

  if(msg_prio==NULL){
    msg_prio=(unsigned *)malloc(sizeof(unsigned int));
    *msg_prio=tmp->queue[0].prio;
  }
  else
    *msg_prio=tmp->queue[0].prio;

    
  for(j=0;j<(tmp->mq_curmsgs-1);j++){
    tmp->queue[j]=tmp->queue[j+1];
  }
  tmp->queue[tmp->mq_curmsgs-1].prio=-1;
  tmp->mq_curmsgs--;
  sem_post(&sread);
  sem_post(&sema);

  return len;
}

mqd_t mq_notify(mqd_t fd, const struct sigevent *event){
  
  struct mq_attr *tmp;

  
  if ((tmp = mmap(NULL, sizeof(struct mq_attr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
    perror("mmap");
    return -1;
  } 

  if((event==NULL) && (tmp->subscriber!=0))
    {
      tmp->subscriber=0;
      tmp->notification_sig=-1;
      printf("sucessfully unsubscribed\n");
      return 0;
    }

  if((SIGEV_SIGNAL!=event->sigev_notify) && (SIGEV_NONE!=event->sigev_notify))
    {
      return -1;
    }
  
  if(SIGEV_NONE==event->sigev_notify)
    {
      tmp->subscriber=getpid();
      return 0;
    }
  
  tmp->notification_sig=event->sigev_signo;
  tmp->subscriber=getpid();
  printf(" %d sucessfully subscribed for %d\n",tmp->subscriber,tmp->notification_sig);
  
  return 0;
}



