#include <iostream>
#include <vector>
#include <string>
#include<typeinfo>
#include<stdexcept>
#include<fstream>
#include <time.h>
#include<cstring>
#include<stdexcept>
#include<fstream>

#define UNDEFINED -1
#define DATA 0
#define REQ 1
#define REP 2
#define UNKNOWN 3

using namespace std;

class Packet{
	char source_Identifier;
	char dest_Identifier;
	int packet_Type = UNDEFINED;	
	string route;
	public:
		Packet(char source_Identifier,char dest_Identifier,string iroute){	
			this->source_Identifier=source_Identifier;
			this->dest_Identifier=dest_Identifier;
			this->route=iroute;
		}
		void set_Source_Identifier(char source_Identifier){
			this->source_Identifier=source_Identifier;
		}
		void set_Dest_Identifier(char dest_Identifier){
			this->dest_Identifier=dest_Identifier;
		}	
		char get_Source_Identifier(){
			return source_Identifier;
		}
		char get_Dest_Identifier(){
			return dest_Identifier;
		}
		void set_Route(string iroute){
			this->route=iroute;	
		}
		string get_Route(){
		return route;
		}
		void set_Packet_Type(int packet_Type){
			this->packet_Type=packet_Type;	
		}
		int get_Packet_Type(){
			return this->packet_Type;	
		}
		virtual ostream& print(ostream& out)=0;
				
};

ostream& operator <<(ostream& out, Packet &object){
		   return object.print(out);			 
}

class Data:public Packet{
	string info;
	public:
		Data(char source_Identifier,char dest_Identifier,string iroute,string info):Packet(source_Identifier,dest_Identifier,iroute){
			this->set_Packet_Type(DATA);
			this->info = info;
		}
		string get_Info(){
			return info;
		}
		void set_Info(string info){
			this->info=info;
		}

		ostream& print(ostream& out){
		 	out<<endl<<"source_Identifier:"<<get_Source_Identifier()<<endl<<"dest_Identifier:"<<get_Dest_Identifier()<<
			endl<<"info:"<<get_Info()<<endl<<"Route:"<<get_Route()<<endl;
			return out;
		}		
};

class Request:public Packet{
	long t0;
	public:
		Request(char source_Identifier,char dest_Identifier,string iroute,int t0):Packet(source_Identifier,dest_Identifier,iroute){
			this->t0=t0;
			this->set_Packet_Type(REQ);
		}
		void set_t0(int t0){
			this->t0=t0;
		}
		long get_t0(){
			return t0;
		}
		ostream& print(ostream& out){
			return out<<endl<<"source_Identifier:"<<get_Source_Identifier()<<endl<<"dest_Identifier:"<<get_Dest_Identifier()<<
			endl<<"Route:"<<get_Route()<<endl<<"t0:"<<get_t0()<<endl;
		}		
};

class Reply:public Request{
	long t1,t2,t3;
	public:
		Reply(char source_Identifier,char dest_Identifier,string iroute,int t0,int t1,int t2):Request(source_Identifier,dest_Identifier,iroute,t0){
			this->t1=t1;this->t2=t2;
			this->set_Packet_Type(REP);
		}
		Reply(char source_Identifier,char dest_Identifier,string iroute,int t0):Request(source_Identifier,dest_Identifier,iroute,t0){
			this->set_Packet_Type(REP);
		}
		void set_t1(int t1){
			this->t1=t1;
		}
		long get_t1(){
			return t1;
		}
		void set_t2(int t2){
			this->t2=t2;
		}
		long get_t2(){
			return t2;
		}
		void set_t3(int t3){
			this->t3=t3;
		}
		long get_t3(){
			return t3;
		}
		ostream& print(ostream& out){
			return out<<endl<<"source_Identifier:"<<get_Source_Identifier()<<endl<<"dest_Identifier:"<<get_Dest_Identifier()<<
			endl<<"Route:"<<get_Route()<<endl<<"t0:"<<get_t0()<<endl<<"t1:"<<get_t1()<<
			endl<<"t2:"<<get_t2()<<endl<<"t3:"<<get_t3()<<endl;	
		}
		long time_Calculator(){
			return (get_t3()-get_t0())-(get_t2()-get_t1());
		}
		
};

class Unknown:public Packet{
	public:
		Unknown(char source_Identifier,char dest_Identifier,string iroute):Packet(source_Identifier, dest_Identifier, iroute )
		{
			this->set_Packet_Type(UNKNOWN);
		}
	ostream& print(ostream& out){
		 	out<<endl<<"source_Identifier:"<<get_Source_Identifier()<<endl<<"dest_Identifier:"<<get_Dest_Identifier()<<
			endl<<"Route:"<<get_Route()<<endl;
			return out;
		}
};

class Router;
class NetworkSimulator{
	vector<Router*> *routers;
	public:
		NetworkSimulator(vector<Router*> *routers){
			this->routers = routers;
		}
		void conduct(char dest_Identifier, Packet *packet);
};
 
class Router{
	char router_Identifier;
	NetworkSimulator* network;
	public:
	Router(char router_Identifier, NetworkSimulator* network)
	{
		this->router_Identifier = router_Identifier;
		this->network = network;
	}
	char get_Router_Identifier()
	{
		return this->router_Identifier;
	}
	void set_Router_Identifier(char router_Identifier)
	{
		this->router_Identifier = router_Identifier;
	}
	void receive(Packet* packet)
	{		
		if (packet->get_Dest_Identifier() != this->router_Identifier){
			forward_Packet(packet);
		}
		else{
			process_Packet(packet);
			if(dataExists(packet))
				cout<<"\tPacket Already Exists:"<<(*packet);
			save(packet);
		}	
	}
	void send(Packet* packet) { receive(packet); }
	private:
	long get_Time(){
		long t;
		cout<<"Please Enter t:"<<endl;
		cin>>t;
		return t;
	}
	void forward_Packet(Packet* packet)
	{
		char next_Router;
		int i=0;
		while(packet->get_Route()[i]!=this->get_Router_Identifier()){
			i++;
		}
		next_Router=packet->get_Route()[++i];
		cout<<"Forwarding Packet to "<<next_Router<<endl;
		network->conduct(next_Router,packet);	
	}
	void process_Packet(Packet* packet)throw(runtime_error)
	{
		try{
		if(packet->get_Packet_Type()==DATA){
			cout << *packet;
		}
		else if (packet->get_Packet_Type()==REQ){
			Reply rep2(packet->get_Dest_Identifier(),packet->get_Source_Identifier(),packet->get_Route(),
					((Request*)packet)->get_t0());
			Reply* rep = &rep2;
			string path=rep->get_Route();
			string path2;
			for (int a=path.length()-1;a>=0;a--){
				path2+=path[a];
			}
			rep->set_Route(path2);
			rep->set_t1(get_Time());
			rep->set_t2(get_Time());
			network->conduct(router_Identifier,rep);
		}
		else if(packet->get_Packet_Type()==REP){
			Reply* rep =(Reply *)packet;
			rep->set_t3(get_Time());
			cout<<rep->time_Calculator();	
		}
		else{
			throw runtime_error("An unknown type of packet passes into router!");
		}
		}
		catch(runtime_error a){
		cout<<a.what();
		cout<<packet;
		}
	}
	int getByteSize(Packet* packet)
	{
		if (packet->get_Packet_Type()==DATA){
			return sizeof(Data);
		}
		if (packet->get_Packet_Type()==REQ){
			 return sizeof(Request);
		}
		if (packet->get_Packet_Type()==REP){
			return sizeof(Reply);
		}	
	}
	void save(Packet *packet){
		fstream file("info.txt",ios::in|ios::out|ios::binary|ios::app);
		if(!file){
			cout<<"File not opened!";
			return;
		}
		int size = getByteSize(packet);
		file.write((char*)&size, sizeof(int));
		file.write((char*)&packet,size);
		file.close();
	}
	bool dataExists(Packet* packet)
	{
		if (packet->get_Packet_Type()!=DATA)
			return false;
		Data* data = (Data*) packet;
		int size = getByteSize(packet);
		fstream file("info.txt",ios::in|ios::out|ios::binary|ios::app);
		file.seekg(0);
		while(file.good())
		{
			int itemSize;
			file.read((char*)&itemSize, sizeof(int));
			if (itemSize != size)
			{
				file.seekg(size, ios_base::cur);
			}
			else
			{
				Data *temp = (Data*)malloc(size);
				file.read((char*)&temp, size);
				cout<<"YY   "<<temp->get_Info()<<endl;
				if (temp->get_Info()==data->get_Info())
				{
					return true;	
				}
			}
		}
		return false;
	}
};

void NetworkSimulator::conduct(char dest_Identifier, Packet *packet){
		for(int i=0; i<routers->size(); i++)
		{
			Router* r = routers->at(i);
			if (r->get_Router_Identifier() == dest_Identifier)
			{
				r->receive(packet);
			}
		}
}

int main()
{
	
	char source_Identifier;
	char dest_Identifier;
	cout<<"Enter source_Identifier(char):"<<endl;
	cin>>source_Identifier;
	cout<<"Enter dest_Identifier(char):"<<endl;
	cin>>dest_Identifier;

	int i;	
	srand (time(0));
	i=rand()%2;
//	cout<<sizeof(Data)<<endl<<sizeof(Request)<<endl<<sizeof(Reply)<<endl<<sizeof(Unknown)<<endl;
	vector<Router*> routers;
	NetworkSimulator ns(&routers);
	Router rA('a', &ns);
	Router rB('b', &ns);
	Router rC('c', &ns);
	Router rD('d', &ns);
	Router rE('e', &ns);
	routers.push_back(&rA);
	routers.push_back(&rB);
	routers.push_back(&rC);
	routers.push_back(&rD);
	routers.push_back(&rE);

if (i==0){
	cout<<"Packet type being sent is Data..."<<endl;
	Data d(source_Identifier,dest_Identifier,"abcde","777777");
	rA.send(&d);	
}
else if (i==1){
	cout<<"Packet type being sent is Request(ping)..."<<endl;
	Request r(source_Identifier,dest_Identifier,"abcde",6);
	rA.send(&r);	
}	
}
