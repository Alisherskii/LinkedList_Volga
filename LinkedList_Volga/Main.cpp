#include "Main.h"

void Node::RandData()
{
	data = rand() % 100 + 1;
	cout << "created rand data for "<<name<<":" << data<<endl;
	CreateEvent(this);
}

void Node::Subscribe(Node* inNode)
{
	if (inNode != this)
	{
		subscriptionsAndSum[inNode] = 0;
		hookEvent(inNode);
		inNode->neighbors.push_back(this);
		neighbors.push_back(inNode);
	}
}

void Node::Unsubscribe(Node* inNode)
{
	subscriptionsAndSum.erase(inNode);
	unhookEvent(inNode);
	auto it = find(inNode->neighbors.begin(), inNode->neighbors.end(), this);
	inNode->neighbors.erase(it);
	it = find(neighbors.begin(), neighbors.end(), inNode);
	neighbors.erase(it);
}

void Node::CreateNewNodeAndSubscribe(NodesNetwork *Network)
{
	auto newNode = new Node();
	Subscribe(newNode);
	Network->nodes.push_back(newNode);
}





void NodesNetwork::addNode(Node* inNode)
{
	nodes.push_back(inNode);
}

int main()
{
	//Nodes and network initialization
	int steps = 0;
	Node* testNode1 = new Node();
	Node* testNode2 = new Node();
	Node* testNode3 = new Node();
	Node* testNode4 = new Node();
	Node* testNode5 = new Node();

	testNode1->name = "Travis";
	testNode2->name = "John";
	testNode3->name = "Alex";
	testNode4->name = "Sid";
	testNode5->name = "Howie";
	
	testNode1->Subscribe(testNode2);
	testNode2->Subscribe(testNode3);
	testNode3->Subscribe(testNode4);
	testNode4->Subscribe(testNode5);
	testNode5->Subscribe(testNode1);

	NodesNetwork* Network = new NodesNetwork();
	Network->addNode(testNode1);
	Network->addNode(testNode2);
	Network->addNode(testNode3);
	Network->addNode(testNode4);
	Network->addNode(testNode5);
	//probabilities initialization
	cout << "Simulation steps amount = ";
	cin >> steps;
	cout << "probability of random number generation:";
	cin >> Network->probabilities[0];
	cout << "Subscribe probability:";
	cin >> Network->probabilities[1];
	Network->probabilities[1] += Network->probabilities[0];
	cout << "Unsubscribe probability:";
	cin >> Network->probabilities[2];
	Network->probabilities[2] += Network->probabilities[1];
	cout << "Create new node probability:";
	cin>> Network->probabilities[3];
	Network->probabilities[3] += Network->probabilities[2];
	
	Network->probabilities[4] = 100;
	//network simulation
	for (int i = 0; i < steps; i++)
	{
		cout << "Step: " << i + 1 << endl;
		Network->Simulate();
		Network->Update();
		if (Network->isEmpty)
		{
			break;
		}
	}
	return 0;

}

void NodesNetwork::Simulate()
{
	
	for (int i = 0; i < nodes.size(); i++)
	{
		
		int action = rand() % 100;
		cout << "ACTION:" << i << endl; 
		
		if(action<probabilities[0])
		{
			nodes[i]->RandData();
			cout << "Generated random integer for " <<nodes[i]->name<< endl;
			continue;
		}
		if (action > probabilities[0] && action < probabilities[1])
		{
			if (nodes[i]->subscriptionsAndSum.size() == 0)
			{
				cout << "Subscribe for " << nodes[i]->name<<" failed"<< endl;
				continue;
			}
			auto it = nodes[i]->subscriptionsAndSum.begin();
			int randIterator = rand() % nodes[i]->subscriptionsAndSum.size();
			advance(it, randIterator);
			auto it2 = it->first->subscriptionsAndSum.begin();
			if (it->first->subscriptionsAndSum.size()==0)
			{
				cout << "Subscribe for " << nodes[i]->name << " failed" << endl;
				continue;
			}
			
			randIterator = rand() % it->first->subscriptionsAndSum.size();
			advance(it2, randIterator);
			nodes[i]->Subscribe(it2->first);
			cout <<nodes[i]->name<< " subscribed on " <<it2->first->name<< endl;
			continue;
		}
		
		if (action > probabilities[1]  && action < probabilities[2])
		{
			if (nodes[i]->subscriptionsAndSum.size() == 0)
			{
				cout << "Unubscribe for " << nodes[i]->name << " failed" << endl;
				continue;
			}
			auto it = nodes[i]->subscriptionsAndSum.begin();
			int randIterator = rand() % nodes[i]->subscriptionsAndSum.size();
			advance(it, randIterator);
			auto lName = it->first->name;
			nodes[i]->Unsubscribe(it->first);
			cout << nodes[i]->name << " unsubscribed from " << lName << endl;
			continue;
		}
		
		if (action > probabilities[2] && action < probabilities[3])
		{
			cout << "Created new node" << endl;
			nodes[i]->CreateNewNodeAndSubscribe(this);
			continue;
		}
		if (action > probabilities[3] && action < probabilities[4])
		{
			cout << "Node sleep" << endl;
			continue;
		}
		
	}
	
}
void NodesNetwork::Update()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->neighbors.empty())
		{
			nodes.erase(nodes.begin() + i);
			i--;
		}
	}
	if (nodes.empty())
	{
		cout << "Network is empty. Terminating." << endl;
		isEmpty = true;
	}
	cout << "CURRENT NODES AMOUNT = " << nodes.size() << endl;
}