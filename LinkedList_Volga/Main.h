#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <map>
#include <ctime>
#include <thread>
using namespace std;
class Node;
class NodesNetwork {
public:
	vector<Node*> nodes;
	void addNode(Node* inNode);
	void Simulate();
	void Update();
	bool isEmpty = false;
	float probabilities[5];
};
[event_source(native)]
[event_receiver(native)]
class Node {
public:
	__event void CreateEvent(Node* inNode);
	
	void hookEvent(Node* inNode)
	{
		__hook(&Node::CreateEvent, inNode, &Node::Handler);

	}
	void unhookEvent(Node* inNode)
	{
		__unhook(&Node::CreateEvent, inNode, &Node::Handler);
	}
	void Handler(Node* inNode)
	{
		subscriptionsAndSum[inNode] += inNode->data;
		cout << inNode->name<<"->"<<name<<":"<< subscriptionsAndSum[inNode] <<endl;
	};


	void RandData();
	void Subscribe(Node* inNode);
	void Unsubscribe(Node* inNode);
	void CreateNewNodeAndSubscribe(NodesNetwork* Network);

	__event void MyEvent(int nValue);

	string name;
	int data;
	map<Node*, int> subscriptionsAndSum;
	vector<Node*> subscribers;
	vector<Node*> neighbors;
};
