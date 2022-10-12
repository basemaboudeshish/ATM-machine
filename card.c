#define _CRT_SECURE_NO_WARNINGS
#include "card.h"
#include "../Terminal/terminal.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


// getCardHolderName function definition //
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
    uint8_t tempHolderName[25] = { 0 }; // create a temporary array of strings to store Card holder Name//
    printf("\nPlease enter the Card Holder Name between 20 and 24 characters  \n");

     fgets(tempHolderName, 24, stdin);

    if (strlen(tempHolderName) > 24) {
        printf("\n Wrong name\n");
        return WRONG_NAME;
    }
    else if (strlen(tempHolderName) < 20) {
        printf("\n Wrong name\n");
        return WRONG_NAME;
    }
    else if (tempHolderName == NULL) {
        printf("\n Wrong name\n");
        return WRONG_NAME;
    }
    else {
        strcpy(cardHolderData.cardHolderName, tempHolderName); // Store the card holder name that met the conditions in ST_cardData_t structure
        printf("\n Card Name is is valid \n");
        return CARD_OK;
    }
}


// getCardExpiryDate function definition
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    uint8_t tempCardExpiry[6] = { 0 }; // create a temporary string to store Card expiry date in order to carry tests//

    // Flush stdin file //
    char ch = 0;
    while (ch = getchar() != '\n' && ch != EOF);
    printf("\n Please enter the Card Expiry Date starting from year 2023 in the format MM / YY, e.g 05/25   \n");

    fgets(tempCardExpiry, 6, stdin);


    if ((strlen(tempCardExpiry) == 5) && (tempCardExpiry[2] == 0x2f)) {

        // Check if the first character is equal to 0 or 1 using ASCII //
        if ((tempCardExpiry[0] == 0x30) || (tempCardExpiry[0] == 0x31)) {
            // Check possible values 0 to 9 of the second character //
            if ((tempCardExpiry[1] >= 0x30) && (tempCardExpiry[1] <= 0x39)) {
                // Check if both first and second characters are zeros using ASCII //
                if ((tempCardExpiry[0] == 0x30) && (tempCardExpiry[1] = 0x30)) {
                    printf("\n Wrong expiry date entry \n");
                    return WRONG_EXP_DATE;
                }
                // Check if month combination are more than 12 using ASCII //
                else if ((tempCardExpiry[0] >= 0x31) && (tempCardExpiry[1] > 0x32)) {
                    printf("\n Wrong expiry date entry \n");
                    return WRONG_EXP_DATE;
                }
                // check if the fourth and fifth characters represent year 2023 and after using ASCII//
                else if ((tempCardExpiry[3] >= 0x32) && (tempCardExpiry[3] <= 0x39)) {
                       if ((tempCardExpiry[4] > 0x32) && (tempCardExpiry[4] <= 0x39)) {
                        strcpy(cardHolderData.cardExpirationDate, tempCardExpiry); // Store the card expiration date that met the conditions in ST_cardData_t structure
                        printf("\n Card expiry data is correct \n");
                        return CARD_OK;
                       }
                       else {
                        printf("\n Card expired\n");
                        return WRONG_EXP_DATE;
                       }
                }
                else {
                    printf("\n Card expired\n");
                    return WRONG_EXP_DATE;
                }
            }
            else {
                printf("\n Card expired\n");
                return WRONG_EXP_DATE;
            }
        }
        else {
            printf("\n Card expired\n");
            return WRONG_EXP_DATE;
        }
    }
    printf("\n Wrong expiry date entry \n");
    return WRONG_EXP_DATE;
}

// getCardPAN function definition //
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    uint8_t tempCardPAN[20] = { 0 };
    int length = 0;

    // Flush stdin file //
    char ch = 0;
    while (ch = getchar() != '\n' && ch != EOF);
    // create a temporary a string to store Card PAN//
    printf("\n Please enter the Card PAN which is 19 character max, and 16 character min    \n");
    scanf("%19s", tempCardPAN);

    length = strlen(tempCardPAN);

    if (length <= 19 && length >= 16 && tempCardPAN != NULL) {
        strcpy(cardHolderData.primaryAccountNumber, tempCardPAN); // Store the card PAN that met the conditions in ST_cardData_t structure
        printf("\n Card PAN is OK \n");
        return CARD_OK;
    }
    else {
        printf("\n Card PAN is wrong \n");
        return WRONG_PAN;
    }
}

