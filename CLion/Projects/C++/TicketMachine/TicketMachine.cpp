//
// Created by 19839 on 2024/3/2.
// .cpp is used to create definitions:定义而非声明
/* 定义:把内容/东西塞进这个洞里
 * 如:
 * 1. int ...
*/

#include "TicketMachine.h"//当前目录下头文件(自定义库)
#include <iostream>//系统编译器目录下的头文件(官方库)
using namespace std;


TicketMachine::TicketMachine() : PRICE(0) {
    printf("TM::TicketMachine()--this=%p\n", this);
}

//TicketMachine::TicketMachine() = default;

//析构函数，在对象被消灭时调用
TicketMachine::~TicketMachine() = default;

void TicketMachine::showPrompt() {
    cout << "something";
}

void TicketMachine::insertMoney(int money) {
    balance += money; //TicketMachine::balance += money;
}

void TicketMachine::showBalance() {
//    cout << this->balance;
    cout << balance;
}

void TicketMachine::printTicket() {

}

void TicketMachine::showTotal() {

}

int TicketMachine::refundBlance() {
    //本地变量存在于这个函数的生存周期内，只能被此函数调用
    int amountToRefund;
    amountToRefund = balance;
    balance = 0;
    return amountToRefund;
}