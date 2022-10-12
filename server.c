#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"

uint32_t index_s = 0;

// Implement server-side accounts' database //
ST_accountsDB_t extern accountRefrence[256];


enum EN_accountState_t running_or_blocked;

// Implement server-side transactions' database //
ST_transaction transaction[256] = {0};

ST_transaction TransactionData;

uint32_t transactionSequenceNumber = 0;

enum EN_serverError_t error;

// Implement recieveTransactionData function :
EN_transStat_t recieveTransactionData(ST_transaction* transData) {

    error = isValidAccount(cardHolderData, accountRefrence);
    if (error == ACCOUNT_NOT_FOUND) {
        printf("\n Fraud card \n");
        return FRAUD_CARD;
        exit(0);
    }
    error = isAmountAvailable(&terminalData, accountRefrence);
    if (error == LOW_BALANCE) {
        transaction[transactionSequenceNumber].transState = DECLINED;
        return  DECLINED_INSUFFECIENT_FUND;
        exit(0);
    }
    /// This part of code used only if the transaction is not saved due to internal error //
   //// error = getTransaction(transactionSequenceNumber, &TransactionData);
    ///if (error == SAVING_FAILED) {
       /// printf("\n Internal server error \n");
        ///return  INTERNAL_SERVER_ERROR;
    ///}
    running_or_blocked = isBlockedAccount(accountRefrence);
    if (running_or_blocked == BLOCKED) {
        printf("\n Declined stolen Card \n");
         return  DECLINED_STOLEN_CARD;
         exit(0);
    }
    else {
        accountRefrence[transactionSequenceNumber].balance = accountRefrence[transactionSequenceNumber].balance - terminalData.transAmount;

        printf("\n Transaction approved \n");
        printf("\nBalance after transaction is  %f \n", accountRefrence[transactionSequenceNumber].balance);
        return APPROVED;
    }
}



// Implement isValidAccount function //

EN_serverError_t isValidAccount(ST_cardData_t cardHolderData, ST_accountsDB_t accountRefrence[]) {
 
        for (index_s = 0; index_s < 256; index_s++) {
            if (strcmp(accountRefrence[index_s].primaryAccountNumber, cardHolderData.primaryAccountNumber) == 0) {
                // return the index of the valid account //
                transactionSequenceNumber = index_s;
                printf("\n Account reference is %d\n", index_s);
                printf("\n Account found \n");
                return SERVER_OK;
                break;
            }
        }
        printf("\n Account not found \n");
        return ACCOUNT_NOT_FOUND;
}


// Implement isAmountAvailable function : //
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t accountRefrence[]) {
    
    if (terminalData.transAmount > accountRefrence[transactionSequenceNumber].balance)
    {
            printf("\n Declined Insuffecient funds \n");
            return LOW_BALANCE;
    }
    else {
        printf("\n Amount available \n");
            return SERVER_OK;
    }
}

// Implement isBlockedAccount function : //
EN_accountState_t isBlockedAccount(ST_accountsDB_t accountRefrence[]) {
    running_or_blocked = accountRefrence[transactionSequenceNumber].state;
    if (running_or_blocked == BLOCKED) {
        printf("\n Account blocked \n");
        return  BLOCKED;
    }
    else {
        printf("\n Account not blocked \n");
        return  RUNNING;
    }
}


// Implement saveTransaction function //
EN_serverError_t saveTransaction(ST_transaction* transData) {

    transData->transactionSequenceNumber = transactionSequenceNumber;
    transaction[transactionSequenceNumber] = *transData;

    // If the transaction database is full and cannot save the transaction data return Saving Failed // 
    if (transactionSequenceNumber > 255) {
        printf("\n Server full cannot save \n");
        return SAVING_FAILED;
        exit(0);
    }
    else {
        // Saving transactions into transaction database //
        transaction[transData->transactionSequenceNumber].cardHolderData = cardHolderData;
        transaction[transData->transactionSequenceNumber].terminalData = terminalData;
        transaction[transData->transactionSequenceNumber].transactionSequenceNumber = transactionSequenceNumber;
        transaction[transData->transactionSequenceNumber].transState = transaction[transactionSequenceNumber].transState;
        
        // Checking whether transactions are saved in transactions database or not due to dropped connection//
        error = getTransaction(transactionSequenceNumber, &transaction[transData->transactionSequenceNumber]);
        if (error == SERVER_OK) {
            printf("\n Sequence number found \n");
            transactionSequenceNumber++;
            printf("The sequence number updated after sucessfull transaction save is: %u", transactionSequenceNumber);/// for testing purpose only  ** remove after test ////
            return SERVER_OK;
        }
        else {
            printf("\n Transaction Saving failed \n");
            return SAVING_FAILED;
            exit(0);
        }
        return 0;
    }
}

// implement getTransaction function //

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction* transData) {

    for (index_s = 0; index_s < 255; index_s++) {
        if (transactionSequenceNumber == transaction[index_s].transactionSequenceNumber) {
            printf("\n This is the transaction data found \n");
            printf("\n Card Holder Name:  %s \n ", transaction[index_s].cardHolderData.cardHolderName);
            printf("\n Card ExpirationDate:  %s \n ", transaction[index_s].cardHolderData.cardExpirationDate);
            printf("\n Primary Account Number %s \n ", transaction[index_s].cardHolderData.primaryAccountNumber);
            printf("\n Transaction Date %s \n ", transaction[index_s].terminalData.transactionDate);
            printf("\n Transaction Amount %f \n ", transaction[index_s].terminalData.transAmount);
            printf("\n Transaction Sequence Number %d \n ", transaction[index_s].transactionSequenceNumber);
            return SERVER_OK;
        }
    } printf("\n Transaction not found \n");
            return TRANSACTION_NOT_FOUND;
}



