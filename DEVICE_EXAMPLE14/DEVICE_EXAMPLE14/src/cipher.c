
/**
 *
 * \file
 *
 * \brief AES example for SAM.
 *
 * This file defines a useful set of functions for the AES on SAM devices.
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <string.h>

#define AES_EXAMPLE_REFBUF_SIZE 4

/**
 * \name sample data from NIST-800-38A appendix F
 */
/* @{ */


/** Cipher 256 bits key array. */
const uint32_t key256[8] = {
	0x16157e2b,
	0xa6d2ae28,
	0x8815f7ab,
	0x3c4fcf09,
	0x6125ac54,
	0xe567f6da,
	0xa54d68ab,
	0x53ae56d2
};

/** Init vector array for CTR mode. */
const uint32_t init_vector_ctr[4] = {
	0xf3f2f1f0,
	0xf7fef5f4,
	0xfbfaf9f8,
	0xfffefdfc
};

/* @} */

/* Output data array */
static uint32_t output_data[AES_EXAMPLE_REFBUF_SIZE];

/* State indicate */
volatile bool state = false;

uint8_t * block_buf;
uint16_t chunk_count;

/** AES configuration */
static struct aes_config g_aes_cfg;

/**
 * \brief The AES interrupt call back function.
 */
void aes_callback(void)
{
	/* Read the output. */
	aes_read_output_data(AES, (uint32_t*) (block_buf + chunk_count));
	state = true;
}


void cipher_init(void)
{
		/* Enable the AES module. */
		aes_get_config_defaults(&g_aes_cfg);
		aes_init(AES, &g_aes_cfg);
		aes_enable();

		/* Enable AES interrupt. */
		aes_set_callback(AES, AES_INTERRUPT_DATA_READY,
		aes_callback, 1);
}

void encrypt_write_data(uint8_t * block, iram_size_t block_size)
{
	//printf("eblock:%x\t%x\t%x\t%d\r\n",block[0],block[1],block[2],block_size);
    state = false;

	/* Configure the AES. */
	g_aes_cfg.encrypt_mode = AES_ENCRYPTION;
	//g_aes_cfg.key_size = AES_KEY_SIZE_128;
	g_aes_cfg.key_size = AES_KEY_SIZE_256;
	g_aes_cfg.start_mode = AES_AUTO_START;
	g_aes_cfg.opmode = AES_CTR_MODE;
	g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
	g_aes_cfg.lod = false;
	aes_set_config(AES, &g_aes_cfg);

	/* Set the cryptographic key. */
	aes_write_key(AES, key256);

	/* Set the initialization vector. */
	aes_write_initvector(AES, init_vector_ctr);

    block_buf = block;
	chunk_count = 0;

    while (chunk_count < block_size) 
    {
		state = false;
        /* Write the data to be ciphered to the input data registers. */
    	aes_write_input_data(AES, (uint32_t*) (block+chunk_count));

    	/* Wait for the end of the encryption process. */
    	while (false == state) {
    	}

		//memcpy( (uint32_t*) (block+count), output_data, sizeof(output_data));

        chunk_count += 16;
    }
}

void decrypt_read_data(uint8_t * block, iram_size_t block_size)
{
	//printf("dblock:%x\t%x\t%x\t%d\r\n",block[0],block[1],block[2],block_size);
    state = false;

	/* Configure the AES. */
	g_aes_cfg.encrypt_mode = AES_DECRYPTION;
	g_aes_cfg.key_size = AES_KEY_SIZE_256;
	g_aes_cfg.start_mode = AES_AUTO_START;
	g_aes_cfg.opmode = AES_CTR_MODE;
	g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
	g_aes_cfg.lod = false;
	aes_set_config(AES, &g_aes_cfg);

	/* Set the cryptographic key. */
	aes_write_key(AES, key256);

	/* Set the initialization vector. */
	aes_write_initvector(AES, init_vector_ctr);

	block_buf = block;
	chunk_count = 0;

	while (chunk_count < block_size)
	{
		state = false;

	   /* Write the data to be ciphered to the input data registers. */
	   aes_write_input_data(AES, (uint32_t*) (block + chunk_count));

	   /* Wait for the end of the encryption process. */
	   while (false == state) {
	   }
	  
	   //memcpy( (uint32_t*) (block+count), output_data, sizeof(output_data));

	   chunk_count += 16;
   }
}

