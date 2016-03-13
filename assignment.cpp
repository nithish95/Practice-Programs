#include<pthread.h>
#include<queue>
#include<unistd.h>
#include<semaphore.h>
#include<cstring>
#include<iostream>
#include<cstdlib>
using namespace std;

#define DEFAULT_CHAIRS 3
#define DEFAULT_BARBERS 1

int tnc,tnb,nb,nc,nDropsOff=0;
int nBarbers,nChairs,nCustomers,serviceTime;
sem_t busy_barber;
sem_t var;
sem_t isleep;
sem_t bye;
int cid=1,f=0;
int bid=0;
int is_Bfree=1;

pthread_mutex_t lock1=PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t nocustomer = PTHREAD_COND_INITIALIZER;
pthread_cond_t wait_for_haircut = PTHREAD_COND_INITIALIZER;
pthread_cond_t wait_chair = PTHREAD_COND_INITIALIZER;


        


class Shop {
	public:
	
	queue <int>waitq;
	int stime;
	
	Shop(int nBarbers,int nChairs);
	Shop();
	
	int visitShop(int id);
	void leaveShop(int customerId,int barberId);
	void helloCustomer(int id);
	void byeCustomer(int id);



};



Shop::Shop(int nBarbers ,int nChairs)
{
	tnb=nBarbers;
	tnc=nChairs;
}

Shop::Shop()
{
	tnb=DEFAULT_BARBERS;
	tnc=DEFAULT_CHAIRS;
}


int Shop::visitShop(int id)
{
	
        
	
        if(f)
  	sem_post(&isleep);
	if(waitq.size()==tnc)
	{
	
		cout<<id<<" Leaves The Shop Because Of No Available Of Waiting Chairs\n";
		nDropsOff++;
		cout<<"Total No of DROPOFFs "<<nDropsOff<<"\n";
		pthread_mutex_unlock(&lock2);
		return -1;			
	}
	
	if(!is_Bfree)
	{
	        waitq.push(id);
		cout<<id<<" Is Waiting in The Chair. # Waiting Seats Available = "<<(tnc-waitq.size())<<"\n";
		
		pthread_mutex_lock(&lock1);
                while(!is_Bfree)
                pthread_cond_wait(&wait_chair,&lock1);
	        pthread_mutex_unlock(&lock1);
	        
                cid=waitq.front();
                
                waitq.pop();
        }
        is_Bfree=0;
        
        cout<<id<<" moves to chair["<<bid<<"]"<<"\n";
        sem_post(&var);
        sem_wait(&busy_barber);
	
        return bid;	
}

void Shop::leaveShop(int customerID,int barberID)
{
      
	
	cout<< cid << " waits for barber["<<barberID <<"] to be done with hair-cut."<<"\n";
	sem_wait(&busy_barber);
	while(!is_Bfree)
	pthread_cond_wait(&wait_for_haircut,&lock1);
	cout<<customerID<<" says Good-bye to the barber"<<"\n"; 
	sem_post(&bye);
	
	
}

void Shop::helloCustomer(int id)
{
        
        
      
       sem_wait(&var); 
       cout<<id<<" starts a haircut service for the customer["<<cid<<"]"<<"\n"; 
       sem_post(&busy_barber);
        
       
       
}

void Shop::byeCustomer(int id)
{
       
        
        cout<<id<<" says he's done with the hair-cut service for the customer["<<cid<<"]"<<"\n";
        sem_post(&busy_barber); 
        pthread_cond_signal(&wait_for_haircut); 
             
        is_Bfree=1;
        sem_wait(&bye);
          if(is_Bfree==1 && waitq.empty())
        {    
             f=1;
	     cout<<id<<" sleeps because of no customers"<<"\n";
	     sem_wait(&isleep);
             
        } 
        pthread_cond_signal(&wait_chair);
        cout<<id<<" calls in another customer"<<"\n";
         
        
        
      
}
struct ThreadParam
{
	Shop shop;
	int serviceTime;
        int Bid;
	int Cid;
};

 void *barber( void *arg ) 
{
	 ThreadParam &param = *(ThreadParam *)arg;
 	 Shop &shop = param.shop;
	 int id = param.Bid;
	 int serviceTime = param.serviceTime;
	

	 while( true )
        {
	       shop.helloCustomer( id ); 
	       usleep( serviceTime );

               shop.byeCustomer( id );
        }
 }
 
 void *customer( void *arg ) 
{

	 ThreadParam &param = *(ThreadParam *)arg;
	 Shop &shop = param.shop;
	 int id = param.Cid;

 
	 int barber = -1;
	 if ( ( barber = shop.visitShop( id ) ) != -1 ) 
	 shop.leaveShop( id, bid );
}
int main(int argc,char** argv)
{
	
        sem_init(&busy_barber,0,0);
        sem_init(&var,0,0);
        sem_init(&isleep,0,0);
        sem_init(&bye,0,0);
	pthread_t tCustomer[100];
	pthread_t tBarber;
	struct ThreadParam Parameters;
	nBarbers=atoi(argv[1]);
	nChairs=atoi(argv[2]);
	nCustomers=atoi(argv[3]);
	serviceTime=atoi(argv[4]);
        Parameters.serviceTime=serviceTime;
	Parameters.Cid=1;
	Parameters.Bid=0;	
	Shop s(nBarbers,nChairs);
	pthread_create(&tBarber,NULL,&barber,&Parameters);
	for(int i=1;i<=nCustomers;i++)
	{
		  Parameters.Cid=i;
	        
	 
	  pthread_create(&tCustomer[i],NULL,&customer,&Parameters);
	  usleep(rand()%1000);
	}
	usleep(100000);
       	return 0;       
}
