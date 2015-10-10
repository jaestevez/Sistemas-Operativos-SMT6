#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable> // std::condition_variable
#include <unistd.h> //sleep

using namespace std;

mutex mtx; //exclusividad

mutex mtx_prd;
condition_variable empty;
mutex mtx_cns;
condition_variable full;

const int nthread = 20;

const int max_buffer = 30;
int buffer[max_buffer];
int pointer = 0;

void consumidor()
{
    while(1)
    {
		sleep((1 + rand() % 10));
		unique_lock<mutex> lck(mtx_cns);
		if (pointer <= 0) cout<<"sleep consumidor "<<this_thread::get_id()<<endl;
		while (pointer <= 0) empty.wait(lck);
		
		mtx.lock();
		pointer--;
		cout<<"eliminando: \tCONTADOR["+to_string(pointer)+"]\n";
		mtx.unlock();
		full.notify_one();
    }
}
void productor()
{
    while(1)
    {
		sleep((1 + rand() % 10));
		unique_lock<mutex> lck(mtx_prd);
		if (pointer >= max_buffer) cout<<"sleep productor "<<this_thread::get_id()<<endl;
		while (pointer >= max_buffer ) full.wait(lck);
		
		mtx.lock();
		buffer[pointer] = (1 + rand() % 100);
	    pointer++;
	    cout<<"creando nuevo \tCONTADOR["+to_string(pointer)+"]\n";
		mtx.unlock();
		empty.notify_one();
    }
}
int main()
{
	int n_case = 0;
	cout<<"Seleccione (1)equilibrio  (2)menos productores (3)menos consumidores:  ";
	cin>>n_case;
	switch(n_case){
		case 1:{
		    thread vthread[nthread];
		    for(int i = 0;i<nthread;i+=2){
		        vthread[i] = thread(productor);
				vthread[i+1] = thread(consumidor);
			}
			for(int i = 0;i<nthread;i++) vthread[i].join();
		break;
		}
		case 2:{
			thread vthreads[]={
				thread(consumidor), thread(consumidor),
				thread(consumidor), thread(productor) };
		    for(int i = 0;i<(int) (sizeof(vthreads)/sizeof(*vthreads));i++) vthreads[i].join();
		break;
		}
		case 3:{
			thread vthreads[]={
				thread(consumidor), thread(productor),
				thread(productor), thread(productor) };
		    for(int i = 0;i<(int) (sizeof(vthreads)/sizeof(*vthreads));i++) vthreads[i].join();
		break;
		}
	}

    return 0;
}

//g++ consumidor_productor.cpp -o consumidor_productor.o -pthread -std=c++11
