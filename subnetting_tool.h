char* calculate_net_id(char* ip, char* net_mask);
char* calculate_broadcast_address(char* ip, char* net_mask);
int count_ones(char* net_mask);
int max_hosts(int ones);
char* makeNetMask(int ones);
void splitIPAddress(char *ipAddres, char* ipParts[]);
char* fromDecimalToBinary(int decimal);
char* binaryToDotDecimal(char* binaryNumber);