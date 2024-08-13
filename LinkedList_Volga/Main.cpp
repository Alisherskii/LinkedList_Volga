#include "Main.h"

void Node::RandData()
{
	data = rand() % 100 + 1;
	cout << "created rand data for "<<name<<":" << data<<endl;
	CreateEvent(this);
}

void Node::Subscribe(shared_ptr<Node> inNode)
{
	if (inNode.get()!= this)
	{
		subscriptionsAndSum[inNode] = 0;
		hookEvent(inNode.get());
		inNode->neighbors.push_back(shared_from_this());
		neighbors.push_back(inNode);
	}
}

void Node::Unsubscribe(shared_ptr<Node> inNode)
{
	subscriptionsAndSum.erase(inNode);
	unhookEvent(inNode.get());
	auto it = find(inNode->neighbors.begin(), inNode->neighbors.end(), shared_from_this());
	inNode->neighbors.erase(it);
	it = find(neighbors.begin(), neighbors.end(), inNode);
	neighbors.erase(it);
}

void Node::CreateNewNodeAndSubscribe(NodesNetwork *Network)
{
	shared_ptr<Node> newNode(new Node());
	Subscribe(newNode);
	Network->nodes.push_back(newNode);
}





void NodesNetwork::addNode(shared_ptr<Node> inNode)
{
	nodes.push_back(inNode);
}

int main()
{
	//Nodes and network initialization
	int steps = 0;
	
	NodesNetwork* Network = new NodesNetwork();
	Network->InitializeNetwork();
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

void NodesNetwork::InitializeNetwork()
{
	shared_ptr<Node> testNode1(new Node());
	shared_ptr<Node> testNode2(new Node());
	shared_ptr<Node> testNode3(new Node());
	shared_ptr<Node> testNode4(new Node());
	shared_ptr<Node> testNode5(new Node());

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

	
	addNode(testNode1);
	addNode(testNode2);
	addNode(testNode3);
	addNode(testNode4);
	addNode(testNode5);
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
