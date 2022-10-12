#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../Application/app.h"
#include "../Card/card.h"
#include "../Server/server.h"
#include "app.h"


// Implement server - side accounts' database //
ST_accountsDB_t accountRefrence[256] = { {2000.0, RUNNING, "8989374615436851"}, {100000.0, BLOCKED, "5807007076043875"}, {1000.0, RUNNING, "5307008886043875"}, {2000000.0, BLOCKED, "7589373456436851"}, {3000.0, BLOCKED, "3769374618996851"}, {78000.0, RUNNING, "2189374613436851"}, {56000.0, BLOCKED, "5689373675436851"}, {0000.0, RUNNING, "9089523815436851"}, {670000.0, BLOCKED, "9089598415436851"}, {340000.0, RUNNING, "2189523865436851"} };

// Implement server-side transactions' database //
extern ST_transaction transactionDatabase[256] = {0};

extern uint32_t transactionSequenceNumber;
enum EN_status_t status;
enum EN_terminalError_t entry;


void appstart()
{
	getCardHolderName(&cardHolderData);
	getCardExpiryDate(&cardHolderData);
    // if PAN of the Card is wrong, it will continue the check // 
    status = getCardPAN(&cardHolderData);
    if (status == WRONG_PAN) {
       printf("\n Account not found \n");
       return ACCOUNT_NOT_FOUND;
        exit(0);
    }
    else {
        getTransactionDate(terminalData);
        entry = isCardExpired(&cardHolderData, &terminalData);
        if (entry == EXPIRED_CARD) {
            exit(0);
        }
        else {
            getTransactionAmount(&terminalData);
            setMaxAmount(&terminalData);
            isBelowMaxAmount(&terminalData);
            recieveTransactionData(&terminalData);
            saveTransaction(&TransactionData);
        }
    }
    return 0;
}



