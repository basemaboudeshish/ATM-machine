#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Card/card.h"
#include "../Server/server.h"
#include "terminal.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>




EN_terminalError_t getTransactionDate(ST_terminalData_t termData) {

    uint8_t temptransDate[11]; // create a temporary array of strings to store transaction date//

    // Flush stdin file //
    char ch = 0;
    while (ch = getchar() != '\n' && ch != EOF);

    printf("\n Please enter the Transaction Date in the format DD / MM / YYYY, e.g 25/05/2022 starting from year 2021 \n");
    scanf("%10s", temptransDate);



    if ((strlen(temptransDate) == 10) && (temptransDate != NULL) && (temptransDate[2] == 0x2f) && (temptransDate[5] == 0x2f)) {
        // We will check every single character and its possible values in combination with next character
        // We will begin with day number checking//
        // check if the first character of day is equal to zero to three using ASCII//
        if ((temptransDate[0] >= 0x30) && (temptransDate[0] <= 0x33)) {
            // check possible values 1 to 9 of the second character using ASCII //
            if ((temptransDate[1] >= 0x30) && (temptransDate[1] <= 0x39)) {
                // check if the first character of month is equal to zero or one using ASCII//    
                if ((temptransDate[3] == 0x30) || (temptransDate[3] == 0x31)) {
                    // check possible values 1 to 9 of the second character of the month using ASCIIm//
                    if ((temptransDate[4] >= 0x30) && (temptransDate[4] <= 0x39)) {
                        // check that year entered is 2021 or more using ASCII//    
                        if ((temptransDate[6] == 0x32) && (temptransDate[7] == 0x30) && (temptransDate[8] >= 0x32) && (temptransDate[9] >= 0x31)) {
                            strcpy(terminalData.transactionDate, temptransDate);
                            // Store the card expiration date that met the conditions in ST_cardData_t structure
                            puts(terminalData.transactionDate);
                            printf("\n Transaction Date is correct \n");
                            return TERMINAL_OK;
                        }
                        else
                            printf("\n Wrong Date \n");
                        return WRONG_DATE;
                        
                    }
                    else
                        printf("\n Wrong Date \n");
                    return WRONG_DATE;
                    
                }
                else
                    printf("\n Wrong Date \n");
                return WRONG_DATE;
                
            }
            else
                printf("\n Wrong Date \n");
            return WRONG_DATE;
            
        }
        else
            printf("\n Wrong Date \n");
        return WRONG_DATE;
        
    }
    else
        printf("\n Wrong Date \n");
    return WRONG_DATE;
    
}



EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
    uint8_t tmptransDate[11] = { 0 };
    uint8_t tmpexpDate[6] = { 0 };

    strcpy(tmptransDate, terminalData.transactionDate);
    strcpy(tmpexpDate, cardHolderData.cardExpirationDate);

// Card expiry year is after the transaction year //
    if (tmpexpDate[4] > tmptransDate[9]) {
        if (tmpexpDate[3] >= tmptransDate[8]) {
            printf("\n Card is not Expired \n");
            return TERMINAL_OK;
        }
        // Card expiry year is equal to the transaction year //
    } else if (tmpexpDate[4] == tmptransDate[9]) {
        if (tmpexpDate[3] == tmptransDate[8]) {
            if (tmpexpDate[1] >= tmptransDate[4]) {
                if (tmpexpDate[0] > tmptransDate[3]) {
                    printf("\n Card is not Expired \n");
                    return TERMINAL_OK;
                }
            }
        }
    } else {
        printf("\n Expired Card \n");
        return EXPIRED_CARD;
    }
    return 0;
}



EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

    // Flush stdin file //
    char ch = 0;
    while (ch = getchar() != '\n' && ch != EOF);

    printf(" \n Please enter the Transaction amount\n");
    float temptransAmount;

    scanf("%f", &temptransAmount);


    terminalData.transAmount = temptransAmount;
        if (temptransAmount <= 0.0) {
            printf("\n Invalid amount \n");
            return INVALID_AMOUNT;
            exit(0);
        }
        else {
            return TERMINAL_OK;
        }
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {


   if (terminalData.transAmount > terminalData.maxTransAmount) {
       printf("\n Declined Amount Exceeding limit");
        return EXCEED_MAX_AMOUNT;
        exit(0);
   }
    else {
         return TERMINAL_OK;
        }
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
    terminalData.maxTransAmount = 20000.0;   //set max transaction limit to 20000 //
          if (terminalData.maxTransAmount <= 0.0) {
              printf("\n Invalid maximum amount \n");
                return INVALID_MAX_AMOUNT;
          }
          else {
                return TERMINAL_OK;
          }
      return 0;
}