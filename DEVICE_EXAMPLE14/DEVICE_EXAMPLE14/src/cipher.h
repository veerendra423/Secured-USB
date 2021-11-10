/*
 * cipher.h
 *
 * Created: 8/17/2019 1:06:56 PM
 *  Author: user
 */ 


#ifndef CIPHER_H_
#define CIPHER_H_

void encrypt_write_data(uint8_t * block, iram_size_t block_size);

void decrypt_read_data(uint8_t * block, iram_size_t block_size);

void aes_callback(void);

void cipher_init(void);



#endif /* CIPHER_H_ */