#include<pthread.h>
#include<semaphore.h>
#include<iostream>
#include<queue>
#include<unistd.h>
using namespace std;

sem_t count;

int f1=0;
int f2=0;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

struct argument 
{
int count;
int id;
};
void *fun1(void *args)
{
struct argument *n=(struct argument *)args;
pthread_mutex_lock(&lock1);
while(!f1)
pthread_cond_wait(&cond1,&lock1);
int c=n->count;
for(int i=0;i<c;i++)
cout<<"FIRST ONE\n";
cout<<n->id;
pthread_mutex_unlock(&lock1);
sem_post(&count);
}
void *fun2(void *args)
{

pthread_mutex_lock(&lock2);
while(!f2)
pthread_cond_wait(&cond2,&lock2);
sem_wait(&count);
cout<<"SECOND ONE\n";
pthread_mutex_unlock(&lock2);

}
void set1()
{
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock3);
f1=1;
pthread_cond_signal(&cond1);
pthread_mutex_unlock(&lock3);
}
void set2()
{
pthread_mutex_t lock4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock4);
f2=1;
pthread_cond_signal(&cond2);
pthread_mutex_unlock(&lock4);
}

int main()
{
pthread_t t1[100];
pthread_t t2;
queue <int>item;
sem_init(&count,0,0);
struct argument str;
str.count=5;
str.id=10;
for(int i=0;i<10;i++)
pthread_create(&t1[i],NULL,&fun1,&str);
pthread_create(&t2,NULL,&fun2,NULL);
set2();
//phread_join(t2,NULL);
usleep(1000);

set1();
//pthread_join(t1,NULL);

for(int j=10;j<=100;j+=10)
item.push(j);
while(!item.empty())
{
cout<<"front="<<item.front()<<"\n";
item.pop();
cout<<"now front="<<item.front()<<"\n";

}
for(int j=2;j<=20;j+=2)
item.push(j);
int l=item.size();
//while(!item.empty())
//{
//cout<<item.front()<<"\n";
//item.pop();
//}
cout<<l;
}

