#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "subnetting_tool.h"




char* binaryToDotDecimal(char* binaryNumber) {
    char* res = (char*)malloc(16 * sizeof(char)); // 4 parts * 3 digits + 3 dots + 1 null-termination

    // Check for allocation failure
    if (res == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    int low = 0;
    int high = 8;
    int i;

    for (i = 0; i < 4; i++) {
        char part[9]; // 8 bits + 1 null-termination
        strncpy(part, binaryNumber + low, 8);
        part[8] = '\0'; // Null-terminate the string

        int decimalPart = strtol(part, NULL, 2);

        sprintf(res + strlen(res), "%d.", decimalPart);

        low += 8;
        high += 8;
    }

    // Remove the trailing dot
    res[strlen(res) - 1] = '\0';

    return res;
}

char* fromDecimalToBinary(int decimal) {
   char* binary = (char*)malloc(9 * sizeof(char)); // 8 bits + 1 null-termination
    
    // Check for allocation failure
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    binary[8] = '\0'; // Null-terminate the string

    for (int i = 7; i >= 0; i--) {
        binary[i] = (decimal % 2) + '0';
        decimal /= 2;
    }

    return binary;
}


char* makeBroadcastMask(char* net_mask) {
    
    char* result = (char*)malloc(16 * sizeof(char)); // 4 parts * 3 digits + 3 dots + 1 null-termination
    //result[0] = '\0';

    // Split net mask
    char** netParts = (char**)malloc(4 * sizeof(char*));
    if (netParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }
    splitIPAddress(net_mask, netParts);
    char part[4];
    // Make broadcast mask
    for (int i = 0; i < 4; i++) {
        
        sprintf(part, "%d", (atoi(netParts[i]) ^ 255));
        strcat(result, part);
        strcat(result, ".");
    }

    // Remove the trailing dot
    result[strlen(result) - 1] = '\0';
    free(netParts);
    return strdup(result);
}

char* calculate_broadcast_address(char* ip, char* net_mask) {
    // Calculate broadcast mask
    char* broadcastMask = makeBroadcastMask(net_mask);
    char* result = (char*)malloc(16 * sizeof(char)); // 4 parts * 3 digits + 3 dots + 1 null-termination

    // Split broadcast mask and IP address
    char** ipParts = (char**)malloc(4 * sizeof(char*));
    if (ipParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    splitIPAddress(ip, ipParts);
    // Allocate memory for mask parts
    char** maskParts = (char**)malloc(4 * sizeof(char*));
    if (maskParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    splitIPAddress(broadcastMask, maskParts);
    // Calculate broadcast address
   
    result[0] = '\0';
    int i=0;
    char part[4]; // 3 digits + 1 null-termination
    for (i = 0; i < 4; i++) {
        
        sprintf(part, "%d", atoi(ipParts[i]) | atoi(maskParts[i]));
        strcat(result, part);
        strcat(result, ".");
    }

    // Remove the trailing dot
    result[strlen(result) - 1] = '\0';

    // Don't forget to free the dynamically allocated memory for strings
    free(ipParts);
    free(maskParts);
    free(broadcastMask);

    // Return the result
    return strdup(result);
}

void splitIPAddress(char *ipAddres, char* ipParts[]) {
    char ipAddressCopy[16]; // Assuming IPv4 address, so 15 characters at most plus null-termination
    
    strncpy(ipAddressCopy, ipAddres, sizeof(ipAddressCopy));
    ipAddressCopy[sizeof(ipAddressCopy) - 1] = '\0'; // Ensure null-termination

    char *token = strtok(ipAddressCopy, ".");
    int i = 0;

    while (token != NULL && i < 4) {
        ipParts[i] = (char*)malloc(strlen(token));
        strcpy(ipParts[i], token);
        token = strtok(NULL, ".");
        i++;
    }
    return;
}

char* calculate_net_id(char* ip, char* net_mask) {
    char* result = (char*)malloc(16 * sizeof(char));
     // Check for allocation failure
    if (result == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }
    char** ipParts = (char**)malloc(4 * sizeof(char*));
    if (ipParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    splitIPAddress(ip, ipParts);

    char** maskParts = (char**)malloc(4 * sizeof(char*));
    if (maskParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    splitIPAddress(net_mask, maskParts);

    result[0] = '\0';
    int i =0;
    char part[4]; // 3 digits + 1 null-termination
    for (i = 0; i < 4; i++) {
        
        sprintf(part, "%d", atoi(ipParts[i]) & atoi(maskParts[i]));
        strcat(result, part);
        strcat(result, ".");
    }

    // Remove the trailing dot
    result[strlen(result) - 1] = '\0';
    free(ipParts);
    free(maskParts);
    return result;

    
}


 int count_ones(char* net_mask) {
    char** maskParts = (char**)malloc(4 * sizeof(char*));
    char* binaryRepresentation;
    int totalCount = 0; 
    splitIPAddress(net_mask, maskParts);
    // Split net mask and convert each part to binary
    for (int i = 0; i < 4; i++) {
         
        binaryRepresentation = fromDecimalToBinary(atoi(maskParts[i]));

        // Count ones in the binary representation
        int count = 0;
        for (int j = 0; j < 8; j++) {
            if (binaryRepresentation[j] == '1') {
                count++;
            }
        }

        // Print the count for each part (optional)
        printf("Count of ones in part %d: %d\n", i + 1, count);

        free(binaryRepresentation);
    

    // Calculate the total count of ones
    
       totalCount+=count;
    
    }
    

    free(maskParts);
    return totalCount;
 }

 int max_hosts(int ones) {
    return (int)(pow(2, (32 - ones)) - 2);

 }

char* makeNetMask(int ones) {
    char* curr = (char*)malloc(33 * sizeof(char)); // 32 bits + 1 for null-termination

    // Check for allocation failure
    if (curr == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    int i;
    for (i = 0; i < ones; i++) {
        curr[i] = '1';
    }

    while (i < 32) {
        curr[i] = '0';
        i++;
    }

    curr[32] = '\0'; // Null-terminate the string

    // Convert binary string to dot-decimal notation
    char* dotDecimal = binaryToDotDecimal(curr);

    // Free the memory allocated for binary string
    free(curr);

    return dotDecimal;
}

 void calculate_all_parameters(char* ip, char* net_mask) {
    // Calculate net ID
    char* ipID = calculate_net_id(ip, net_mask);
    char* broadcast = calculate_broadcast_address(ip, net_mask);

    char** ipParts = (char**)malloc(4 * sizeof(char*));
    if (ipParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    splitIPAddress(ip, ipParts);
    int lastPart = atoi(ipParts[3])+1;
    sprintf(ipParts[3], "%d", lastPart);

    char** broadParts = (char**)malloc(4 * sizeof(char*));
    if (broadParts == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    splitIPAddress(broadcast, broadParts);
    int broadLastPart = atoi(broadParts[3])-1;
    sprintf(broadParts[3], "%d", broadLastPart);

    char* first =  (char*)malloc(16* sizeof(char));
    char* last =  (char*)malloc(16* sizeof(char));
    //first[0] = last[0] = '\0';
    int i;
    for (i = 0; i < 4; i++) {
        strcat(first, ipParts[i]);
        strcat(last, broadParts[i]);
        strcat(first, ".");
        strcat(last, ".");
    }

    first[strlen(first) - 1] = last[strlen(last) - 1] = '\0'; // Removing trailing dot

    int maxNumberOfHosts = max_hosts(count_ones(net_mask));

    printf("IP ID: %s\n", ipID);
    printf("First: %s\n", first);
    printf("Last: %s\n", last);
    printf("Broadcast: %s\n", broadcast);
    printf("Max Number of Hosts: %d\n", maxNumberOfHosts);
    free(first);
    free(last);
    free(ipParts);
    free(broadParts);
}


int main() {
    char ip[16]; // Assuming IPv4 address (xxx.xxx.xxx.xxx) with null-termination
    char net[16]; // Assuming net mask with null-termination

    printf("Enter IP: ");
    scanf("%15s", ip);

    printf("Enter number of ones or net mask: ");
    scanf("%15s", net);

    char* netMask;
    if (strchr(net, '.') == NULL) {
        // If the input doesn't contain a dot, assume it's the number of ones
        netMask = makeNetMask(atoi(net));
    } else {
        // Otherwise, assume it's already a net mask
        netMask = strdup(net);
    }

    if (netMask == NULL) {
        fprintf(stderr, "Error creating net mask\n");
        return 1;
    }

    calculate_all_parameters(ip, netMask);
    free(netMask);
    return 0;
}