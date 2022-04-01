// Hash Table Implementation by Bhim KC 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

using namespace std;


//************************* NODE CLASS *******************************/
class ListNode{
public:
    int data;
    ListNode *next;
    
/*------------------ ListNode Constructors -------------------------*/
    ListNode(int value){
       this -> data = value;
       this -> next = NULL;
    }

	ListNode(){
        this->data = 0;
        this->next = NULL;
    }

/*------------------- PrintNode Method ------------------------*/
    void printNode() {
        if(next == NULL){
            cout << data << ", NULL "<< endl;
        }
        else{
            cout << data << ", " << next -> data << endl;
        }
    }
    
};

//************************* HASH TABLE CLASS *******************************/
class HTable{
   public: 
    char op;
    int bucketSize;
    ListNode **hashTable;


/*------------------ Hash Table Constructor -------------------------*/
    HTable(int bsize){
        ListNode temp;
        bucketSize = bsize;
        hashTable = new ListNode*[bucketSize];
        for(int i=0;i<bucketSize;i++){
            ListNode *dummyNode = new ListNode;
            dummyNode->data = -9999;
            dummyNode->next = NULL;
            hashTable[i] = dummyNode;
        }
    }
/*------------------ Hash Function -------------------------*/
    int hashInt(int value){
        int digit, sum = 0, i = 1;
        while(value > 0){
            digit = value % 10;
            sum += digit * i;
            value /= 10;
            i++;
        }
        return sum % bucketSize;

    }

/*------------------ Information Processing function -------------------------*/
    void informationProcessing(ifstream& inFile, ofstream& outFile1, ofstream& outFile2){
		
		int data, index;
		while(!inFile.eof()){
			inFile >> op;
			inFile >> data;
			index = hashInt(data);
			printList(index, outFile1);
			if(op == '+'){
				hashInsert(index, data, outFile1, outFile2);
			}else if(op == '-'){
				hashDelete(index, data, outFile1, outFile2);
			}else if(op == '?'){
				hashRetrieval(index, data, outFile1, outFile2);
			}else{
				outFile1 << "op is an unrecognize operation!"<<endl;
			}
			outFile1 << "After one op "<<endl;
			printList(index, outFile1);
		}
	}
/*------------------ Find Spot Function -------------------------*/

   ListNode* findSpot(int index, int data){
        ListNode* spot = new ListNode;
        spot = hashTable[index];
        while((spot->next != NULL) && (spot->next->data < data)){
                spot = spot->next;
        }
        return spot;
    }

/*------------------ Insert Function -------------------------*/
   void hashInsert(int index, int data, ofstream & outFile1,  ofstream & liberoutFile2){

        ListNode* spot = findSpot(index, data);
        if((spot->next != NULL) && (spot->next->data == data)){
            outFile1 << "*** Data is already in the hashTable, no insertion takes place!"<<endl;
        }else{
            ListNode *newNode = new ListNode;
            newNode->data = data;
            newNode->next = spot->next;
            spot->next = newNode;
        }
    }

/*------------------ Delete Function -------------------------*/

    void hashDelete(int index, int data, ofstream & outFile1,  ofstream & outFile2){
        ListNode* spot = new ListNode;
        spot = findSpot(index, data);
        if((spot->next != NULL) && (spot->next->data == data)){
            ListNode* temp = new ListNode;
            temp = spot->next; 
            spot->next = temp->next; 
            temp->next = NULL;
        }else{
            outFile1 << "*** Warning, the data is *not* in the database!"<<endl; 
        }
    
    }

/*------------------ Retrieve Function -------------------------*/
    void hashRetrieval(int index, int data, ofstream & outFile1,  ofstream & outFile2){
		ListNode* spot = findSpot(index, data);
		if((spot->next != NULL) && (spot->next->data == data)){
			outFile1 << "*** Yes, the data is in the hashTable!"<<endl; 
		}else{
			outFile1 << "*** No, the data is *not* in the hashTable!"<<endl; 
		}
		
	}

/*------------------ Linked List in one Index Print Function -------------------------*/
    void printList(int index, ofstream &outFile1){
		ListNode* temp = hashTable[index];
		outFile1 << "hashTable[" <<index<<"]:";
		while(temp->next != NULL){
		    outFile1 << " (" << temp->data <<","<<temp->next->data<<") -> "; 
		    temp = temp->next;
		}
		outFile1 << endl<<endl;
	}

/*------------------ Hash Table Print Function -------------------------*/
   void printHashTable(ofstream & outFile1){
		for(int i=0;i<bucketSize;i++){
			printList(i, outFile1);
		}
	}


};


int main(int argc, const char * argv[]) {
    
    if(argc != 5){
		cout << "need one inFile, a bucketSize and two outFiles " <<endl;
	}else{
	ifstream inFile;
	inFile.open(argv[1]);

	ofstream outFile1, outFile2;
	int bSize = atoi(argv[2]);
	outFile1.open(argv[3], ofstream::out | ofstream::trunc); 
	outFile2.open(argv[4], ofstream::out | ofstream::trunc);

	HTable hashTable(bSize);
	hashTable.informationProcessing(inFile, outFile1, outFile2);
	hashTable.printHashTable(outFile1);

	inFile.close();
	outFile1.close();
	outFile2.close();
	}

    return 0;    
}

