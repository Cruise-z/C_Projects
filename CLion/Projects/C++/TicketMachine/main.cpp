#include <iostream>
#include "TicketMachine.h"

int main() {
    /* 对对象的定义,即:
     * TicketMachine tm; == tm.TicketMachine();
     */
    TicketMachine tm;
    tm.insertMoney(100);
    tm.showBalance();
    return 0;
}
