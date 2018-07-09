#include "Li1DataHandler.h"
#include "Arduino.h"
class node {
private:
  char Packet[];
  node nextNode;

public:
  node(char packet[]) { Packet = packet; }
  char getData() { return Packet; }
  node getNext() { return nextNode; }
  void setNode(node NextNode) {
    delete nextNode;
    nextNode = NextNode;
  }
  void deleteNode() { delete nextNode; }
};

class queue {
private:
  node headNode;
  node *tailNode;
  unsigned int count;

public:
  queue() { count = 0; }
  void push(char packet[]) {
    if (headNode == NULL) {
      headNode = new node(packet);
      tailNode = &headNode;
    } else {
      node tempNode = new node(packet);
      tempNode.setNode(headNode);
      headNode = tempNode;
    }
    count++;
  }
  char *pop() {
    if (count == 0) {
      return NULL;
    }
    node tempNode = headNode;
    headNode = headNode.getNext();
    char returnChar[] = tempNode.getData();
    delete tempNode;
    if (headNode == NULL) {
      tailNode = NULL;
    }
    count-- return returnChar;
  }
  int getCount() { return count; }
};

class Li1DataHandler {
private:
  queue dataQueue;
  static Li1DataHandler theHandeler;
  /**
          returns data wrapped with the header and checksum
  **/
  char *createWraper(char data[], int size) {
    unsigned char wraper[size + 11];
    wraper[0] = 0x48;
    wraper[1] = 0x65;
    wraper[2] = 0x20;
    wraper[3] = TRANSMIT_DATA;
    wraper[4] = size >> 8 & 0xff;
    wraper[5] = size & 0xff;
    wraper[6] = 0;
    wraper[7] = 0 for (size_t i = 0; i < 6; i++) {
      wraper[6] += wraper[i];
      wraper[7] += wraper[6]
    }
    wraper[size + 8] = 0;
    wraper[size + 9] = 0;
    memcpy(&wraper[8], data, size);
    for (size_t i = 8; i < size + 8; i++) {
      wraper[size + 8] += wraper[i];
      wraper[size + 9] += wraper[size + 8];
    }
    wraper[size + 10] = '\0';
    return wraper;
  }

  Li1DataHandler() { dataQueue = new queue(); }

public:
  static Li1DataHandler getHandler() {
    if (theHandeler == null) {
      theHandeler = new Li1DataHandler();
    }
    return theHandeler;
  }
  void addData(char dataToAdd[],int size) {
    while (size > 0) {
      if (size < 256) {
        dataQueue.push(createWraper(dataToAdd, size));
        size = 0;
      } else {
        char first255[254];
        memcpy(first255, dataToAdd, 255);
        size -= 255;
        char tempArray[size];
        memcpy(tempArray, &dataToAdd[255], size);
        dataToAdd = tempArray;
      }
    }
  }
  void addData(char dataNullTerminated[]) {
    addData(dataNullTerminated, strlen(dataNullTerminated));
  }

  char *getData() { return dataQueue.pop(); }

  unsigned char hasNext() { return dataQueue.getCount() != 0; }
};
