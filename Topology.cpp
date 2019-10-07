#include <iostream>
#include <vector>

using namespace std;

class Topology{
	private:	
		vector< vector<int> > nodes;
//		int numberOfNodes=0;
		string name;
	public:
		Topology(string name){
			this->name=name;
		}
		void add_Node(){
			nodes.push_back(vector<int>());
		}
		
		void addTrace(int NodeNumber,int Destination){
			nodes[NodeNumber].push_back(Destination);
			nodes[Destination].push_back(NodeNumber);
		}
		
		bool is_Trace(int SourceNode,int DestNode){
			for(int i=0;i<nodes[SourceNode].size();i++){
				if(nodes[SourceNode][i]==DestNode)
					return 1;
			}
			return 0;
		}
		void set_Name(string nodeName){
			name=nodeName;
		}
		string get_Name(){
			return name;
		}
		void display(){
			for(int i=0;i<nodes.size();i++){
				cout<<i<<": ";
				for(int j=0;j<nodes[i].size();j++){
					cout<<nodes[i][j]<<'\t';
				}
				cout<<endl;
			}
		}
		void delete_Trace(int SourceNode,int DestNode){
			for(int i=0;i<nodes[SourceNode].size();i++){
				if(nodes[SourceNode][i]==DestNode){
				
					 for(i;i<nodes[SourceNode].size();i++){
					 	nodes[SourceNode][i]=nodes[SourceNode][i+1];
					 	if(i=nodes[SourceNode].size()){
					 		nodes[SourceNode].pop_back();
						 }
					 }
		    	}
			}
			for(int i=0;i<nodes[DestNode].size();i++){
				if(nodes[DestNode][i]==SourceNode){
				
					 for(i;i<nodes[DestNode].size();i++){
					 	nodes[DestNode][i]=nodes[DestNode][i+1];
					 	if(i=nodes[DestNode].size()){
					 		nodes[DestNode].pop_back();
						 }
					 }
		    	}
			}
		}				
};

int main(){
	Topology t("name");
	t.add_Node();
	t.add_Node();
	t.add_Node();
	t.add_Node();
	t.add_Node();
	t.addTrace(4,1);
	t.addTrace(4,3);
	t.addTrace(0,1);
	t.addTrace(1,2);
	//t.delete_Trace(1,2);
//	t.delete_Node(2);
	t.display();
}
