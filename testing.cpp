#include <iostream>
#include <iomanip>
#include <cstring>
#define MAX 5

struct arduinoData {
    int value;
    arduinoData *next;
};

void removeLast(arduinoData* head) {
    if (head->next == NULL)
        std::cout << "Last node. Abort." << std::endl;
    else {
        while(head->next->next)
            head = head->next;
        delete head->next;
        head->next = NULL;
    }
}

void checkFull(arduinoData* head) {
    int count = 0;
    arduinoData* tempHead = head;
    while(tempHead != nullptr) {
        count++;
        tempHead = tempHead->next;
    }
    if (count >= MAX)
        removeLast(head);
}

void addData(arduinoData** headPtr, int input) {
    checkFull(*headPtr);
    arduinoData* newData = new arduinoData;
    newData->value = input;
    newData->next = *headPtr;
    *headPtr = newData;
}

void printStack(arduinoData* head) {
    std::cout << "+-------+---------+" << std::endl;
    std::cout << "| Index |" << "  Value  |" << std::endl;
    std::cout << "+-------+---------+" << std::endl;
    int index = 0;
    arduinoData* tempHead = head;
    while(tempHead) {
        //std::cout << "|" << index << "|" << std::setw(9) << tempHead->value << "|" << std::endl;
        std::cout << "|" << std::setw(7) << index << "|" << std::setw(9) << tempHead->value << "|" << std::endl;
        std::cout << "+-------+---------+" << std::endl;
        tempHead = tempHead -> next;
        index++;
    }
}

int main() {
    // arduinoData* head = NULL;
    // addData(&head, 20);
    // addData(&head, 40);
    // std::cout << "Current Stack Data: " << std::endl;
    // printStack(head);
    // removeLast(head);
    // std::cout << "Remove Last: " << std::endl;
    // printStack(head);
    // addData(&head, 60);
    // addData(&head, 80);
    // addData(&head, 100);
    // addData(&head, 120);
    // std::cout << "Current Stack Data: " << std::endl;
    // printStack(head);
    // std::cout << "Check if the last node is ommited after 5 nodes" << std::endl;
    // addData(&head, 140);
    // printStack(head);
    //std::cout << "Stack Full? " << checkFull(head) << std::endl;
    char boo[10] = "Hello";
    std::cout << boo << std::endl;
    strcpy(boo, "Bye");
    std::cout << boo << std::endl;
    return 0;
}