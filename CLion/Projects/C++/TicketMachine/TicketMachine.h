//
// Created by 19839 on 2024/3/2.
// .h is used to create declarations:声明而非定义
/**
 * .h中只放声明不放定义是为了:
 *      避免多个.cpp去include同一个.h头文件时链接器遇到重复定义问题
**/
/* 声明:留个洞，告诉你有这么个东西，但不知道在哪
 * 如:
 * 1. extern variables
 * 2. function prototypes
 * 3. class/struct declaration
*/

/**
 * 标准头文件结构:
 *      避免同一个.cpp中多次引用下述声明报错
**/
#ifndef TICKETMACHINE_TICKETMACHINE_H
#define TICKETMACHINE_TICKETMACHINE_H


class TicketMachine {
public:
    //函数属于这个类但不属于这个类中的某个对象
    //函数属于对一个类的共同的操作，类里的不同对象拥有的成员变量有所不同，但在调用函数时执行的操作是一样的
    //在调用函数时，函数本身可以知道这个变量是那个对象对应的变量
    /*
     * From C++ to C
     * Point a;
     * a.print(); == Point::print(&a);
     */
    //构造函数，会在这个类的一个对象被创建时自动被调用
    TicketMachine();
    virtual ~TicketMachine();
    void showPrompt();
    void insertMoney(int money);
    void showBalance();
    void printTicket();
    void showTotal();
    int refundBlance();
private:
    //成员变量声明，成员变量存在于一个对象的生存周期，可被所有成员函数调用
    //成员变量在这个类的每一个对象里面，成员变量被定义即在定义对象时就完成了
    const int PRICE;
    int balance{};
    int total{};
};


#endif //TICKETMACHINE_TICKETMACHINE_H
