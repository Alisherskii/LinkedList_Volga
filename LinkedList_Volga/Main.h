#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <map>
#include <ctime>
#include <thread>
#include <memory>
using namespace std;
class Node;
class NodesNetwork {
public:
	vector<shared_ptr<Node>> nodes;
	void addNode(shared_ptr<Node> inNode);
	void InitializeNetwork();
	void Simulate();
	void Update();
	bool isEmpty = false;
	float probabilities[5];
};
[event_source(native)]
[event_receiver(native)]
class Node : public std::enable_shared_from_this <Node>
{
public:
	__event void CreateEvent(Node* inNode);
	
	void hookEvent(Node* inNode)
	{
		__hook(&Node::CreateEvent,  inNode, &Node::Handler);

	}
	void unhookEvent(Node* inNode)
	{
		__unhook(&Node::CreateEvent, inNode, &Node::Handler);
	}
	void Handler(Node* inNode)
	{
		subscriptionsAndSum[inNode->shared_from_this()] += inNode->data;
		cout << inNode->name<<"->"<<name<<":"<< subscriptionsAndSum[inNode->shared_from_this()] << endl;
	};


	void RandData();
	void Subscribe(shared_ptr<Node> inNode);
	void Unsubscribe(shared_ptr<Node> inNode);
	void CreateNewNodeAndSubscribe(NodesNetwork* Network);


	string name;
	int data;
	map<shared_ptr<Node>, int> subscriptionsAndSum;
	vector<shared_ptr<Node>> subscribers;
	vector<shared_ptr<Node>> neighbors;
};
