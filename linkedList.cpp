#include <iostream>
#include <fstream>
#include <thread>
#include<bits/stdc++.h>
#include "rot13server_cpp/rot13server.cpp"

using namespace std;

struct node
{
	int value;
	node* next;
};

class linkedList
{
private:
	node* head;
	int size;
	int waitForResponse(int thisPort)
	{ 
		int iRet = 1;
		WSADATA wsaData;
		cout << "Initializing winsock... ";
		if (WSAStartup(MAKEWORD(REQ_WINSOCK_VER,0), &wsaData)==0)
		{
			// Check if major version is at least REQ_WINSOCK_VER
			if (LOBYTE(wsaData.wVersion) >= REQ_WINSOCK_VER)
			{
				cout << "initialized.\n";
				
				int port= thisPort;
				iRet= !RunServer(port);
			}
			else
			{
				cerr << "required version not supported!";
			}

			cout << "Cleaning up winsock... ";

			// Cleanup winsock
			if (WSACleanup()!=0)
			{
				cerr << "cleanup failed!\n";
				iRet = 1;
			}   
			cout << "done.\n";
		}
		else
		{
			cerr << "startup failed!\n";
		}
		return iRet;
	}
	bool writeInputArray(int* array, int size)
	{
  		ofstream myfile ("arrayInput.txt");
  		if (myfile.is_open())
  		{
    		for(int count= 0;count<size;count ++)
    		{
        		myfile << array[count] << " " ;
    		}
    		myfile.close();
    		return true;
  		}
  		return false;
	}
	void printArray(int* array, int n)
	{
		cout << "[";
		for(int i=0;i<n-1;i++)
		{
			cout << array[i] << ", ";
		}
		cout << array[n-1] << "]" << endl;
	}
	int* readOutputArray(string filename)
	{
		ifstream infile(filename);
		string sline;
		if(infile.good())
		{
			getline(infile, sline);
		}
		infile.close();
		string lengthArray= "";
		int i=0;
		for(i;i<sline.length();i++)
		{
			if(sline[i]==':')
			{
				break;
			}
			else
			{
				lengthArray= lengthArray+sline[i];
			}
		}
		int lengthOfSorted= stoi(lengthArray);
		int* sortedArray;
		sortedArray= new int[lengthOfSorted];
		int index= 0;
		string element= "";
		for(i=i+2;i<sline.length();i++)
		{
			if(sline[i]==' ')
			{
				sortedArray[index]= stoi(element);
				element= "";
				index++;
			}
			else
			{
				element= element+sline[i];
			}
		}
		return sortedArray;
	}
public:
	linkedList()
	{
		size= 0;
		head= NULL;
	}
	linkedList(const linkedList &objectList)
	{
		cout << "copy cons" << endl;
		node* here= objectList.head;
		int oldSize= objectList.size;
		for(int i=0;i<oldSize;i++)
		{
			this->addItem(here->value);
		}
		this->reverse();
	}
	~linkedList()
	{
		this->clear();
		delete head;
	}
	void addItem(int item)
	{
		node* newNode;
		newNode= new node;
		newNode->value= item;
		node* temp= head;
		head= newNode;
		newNode->next= temp;
		size++;
	}
	node* getHead()
	{
		return head;
	}
	void printList()
	{
		node* here= head;
		while(here->next!=NULL)
		{
			cout << here->value << " -> ";
			here= here->next;
		}
		cout << here->value << endl;
	}
	void reverse()
	{
		node* here= head;
		node* prev= NULL;
		while(here->next!=NULL)
		{
			node* temp= here->next;
			here->next= prev;
			prev= here;
			here= temp;
		}
		here->next= prev;
		head= here;
	}
	node* findItem(int item)
	{
		node* here= head;
		while(here->next!=NULL)
		{
			if(here->value==item)
			{
				return here;
			}
			here= here->next;
		}
		if(here->value==item)
		{
			return here;
		}
		return NULL;
	}
	void deleteHead()
	{
		node* here= head;
		here= here->next;
		delete head;
		head= here;
		size--;
	}
	void deleteTail()
	{
		if(size==0)
		{
			return;
		}
		if(size==1)
		{
			delete head;
			head= NULL;
		}
		else
		{
			node* here= head;
			node* next= here->next;
			while(next->next!=NULL)
			{
				here= next;
				next= next->next;
			}
			delete next;
			here->next= NULL;
		}
		size--;
	}
	void deleteItem(int item)
	{
		node* itemPtr= findItem(item);
		if(itemPtr==NULL)
		{
			return;
		}
		node* newNext= itemPtr->next;
		if(head->value==item)
		{
			deleteHead();
			return;
		}
		node* here= head;
		while(here->next->value!=item)
		{
			here=here->next;
		}
		delete here->next;
		here->next= newNext;
		size--;
	}
	int getSize()
	{
		return size;
	}
	void sort()
	{
		int array[size];
		node* here= head;
		int i= 0;
		while(here->next!=NULL)
		{
			array[i]= here->value;
			here= here->next;
			i++;
		}
		array[i]= here->value;
		writeInputArray(array,size);
		auto startSlave= [](int dummy) {system("python pythonSortSlave.py");};
		thread slaveThread(startSlave, 1);
		cout << "********** Starting Slave **********" << endl;
		int indicator= waitForResponse(5000);
		slaveThread.join();
		int* sortedArray= NULL;
		if(!indicator)
		{
			sortedArray= readOutputArray("arrayOutput.txt");
		}
		cout << "********** Ending Slave **********" << endl;
		head= NULL;
		int prevSize= size;
		size= 0;
		for(int i=prevSize-1;i>=0;i--)
		{
			addItem(sortedArray[i]);
		}
	}
	void clear()
	{
		int currSize= size;
		for(int i=0;i<currSize;i++)
		{
			this->deleteHead();
		}
	}
	void addArray(int* array, int arraySize)
	{
		for(int i=arraySize-1;i>=0;i--)
		{
			this->addItem(array[i]);
		}
	}
	int getElementAtIndex(int index)
	{
		if((index>=size)||(index<0))
		{
			throw std::out_of_range ("Index out of range");
		}
		else
		{
			node* here= head;
			for(int i=0;i<index;i++)
			{
				here= here->next;
			}
			return here->value;
		}
	}
	int indexOf(int query)
	{
		int index= 0;
		node* found= findItem(query);
		if(found)
		{
			node* here= head;
			while(here->value!=query)
			{
				index++;
				here= here->next;
			}
			return index;
		}
		return -1;
	}
	int count(int query)
	{
		int count= 0;
		node* here= head;
		for(int i=0;i<size;i++)
		{
			if(here->value==query)
			{
				count++;
			}
			here= here->next;
		}
		return count;
	}
	int max()
	{
		int max= INT_MIN;
		node* here= head;
		for(int i=0;i<size;i++)
		{
			if(here->value>max)
			{
				max= here->value;
			}
			here= here->next;
		}
		return max;
	}
	int min()
	{
		int min= INT_MAX;
		node* here= head;
		for(int i=0;i<size;i++)
		{
			if(here->value<min)
			{
				min= here->value;
			}
			here= here->next;
		}
		return min;
	}
	int sum()
	{
		int sum= 0;
		node* here= head;
		for(int i=0;i<size;i++)
		{
			sum= sum+ here->value;
			here= here->next;
		}
		return sum;
	}
};

int main()
{
	linkedList list;
	list.addItem(2);
	list.addItem(4);
	list.addItem(1);
	list.addItem(20);
	list.addItem(17);
	list.addItem(172);
	list.addItem(19);
	list.addItem(220);
	list.addItem(417);
	list.addItem(12);
	list.sort();
	list.printList();
	list.deleteItem(20);
	list.deleteItem(17);
	list.printList();
	list.addItem(210);
	list.addItem(-21);
	list.addItem(-292);
	list.addItem(82);
	list.printList();
	list.sort();
	list.printList();
	list.clear();
	int array[6]= {1,2,3,4,5,6};
	list.addArray(array,6);
	list.printList();

	linkedList copy;
	copy= list;
	copy.printList();
	cout << copy.getElementAtIndex(4) << endl;
	cout << copy.indexOf(3) << endl;
	copy.addItem(5);
	copy.addItem(5);
	copy.addItem(5);
	copy.addItem(5);
	copy.addItem(-90);
	cout << copy.count(5) << endl;
	copy.printList();
	cout << copy.sum() << endl;
}