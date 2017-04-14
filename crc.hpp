
#ifndef CRC_HPP_
#define CRC_HPP_


void encode(unsigned char* msg, int len, unsigned int p, int g ,unsigned char* e_msg);

bool validate(unsigned char* msg, int len, unsigned int p, int g);

#endif /* CRC_HPP_ */
