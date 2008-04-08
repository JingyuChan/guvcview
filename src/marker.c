#include "huffman.h"
#include "prototype.h"
#include <string.h>
// Header for JPEG Encoder

UINT8* write_markers (struct JPEG_ENCODER_STRUCTURE * jpeg_encoder_structure, UINT8 *output_ptr,int huff, UINT32 image_width, UINT32 image_height)
{
	UINT16 i, header_length;
	UINT8 number_of_components;

	// Start of image marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xD8;
	//added from here 
	// Start of APP0 marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xE0;
	//header length
	*output_ptr++= 0x00;
	*output_ptr++= 0x10;//16 bytes
	
	//type
	if(huff) {//JFIF0 0x4A46494600
		*output_ptr++= 0x4A;
		*output_ptr++= 0x46;
		*output_ptr++= 0x49;
		*output_ptr++= 0x46;
		*output_ptr++= 0x00;
	} else { // AVI10 0x4156493100
		*output_ptr++= 0x41;
		*output_ptr++= 0x56;
		*output_ptr++= 0x49;
		*output_ptr++= 0x31;
		*output_ptr++= 0x00;
	}
	// version
	*output_ptr++= 0x01;
	*output_ptr++= 0x02;
	// density 0- no units 1- pix per inch 2- pix per mm
	*output_ptr++= 0x01;
	// xdensity - 120
	*output_ptr++= 0x00;
	*output_ptr++= 0x78;
	// ydensity - 120
	*output_ptr++= 0x00;
	*output_ptr++= 0x78;
	
	//thumb x y
	*output_ptr++= 0x00;
	*output_ptr++= 0x00;
	//to here
	
	// Quantization table marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xDB;

	// Quantization table length
	*output_ptr++ = 0x00;
	*output_ptr++ = 0x43;

	// Pq, Tq
	*output_ptr++ = 0x00;

	// Lqt table
	for (i=0; i<64; i++)
		*output_ptr++ = jpeg_encoder_structure->Lqt [i];

	// Quantization table marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xDB;
	
	// Quantization table length
	*output_ptr++ = 0x00;
	*output_ptr++ = 0x43;
	
	// Pq, Tq
	*output_ptr++ = 0x01;

	// Cqt table
	for (i=0; i<64; i++)
		*output_ptr++ = jpeg_encoder_structure->Cqt [i];

	if (huff) {
		// huffman table(DHT)
		
		*output_ptr++=0xff;
		*output_ptr++=0xc4;
		*output_ptr++=0x01;
		*output_ptr++=0xa2;
		memmove(output_ptr,&JPEGHuffmanTable,JPG_HUFFMAN_TABLE_LENGTH);/*0x01a0*/
		output_ptr+=JPG_HUFFMAN_TABLE_LENGTH;
		
	}

	number_of_components = 3;

	// Frame header(SOF)

	// Start of frame marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xC0;

	header_length = (UINT16) (8 + 3 * number_of_components);

	// Frame header length	
	*output_ptr++ = (UINT8) (header_length >> 8);
	*output_ptr++ = (UINT8) header_length;

	// Precision (P)
	*output_ptr++ = 0x08;/*8 bits*/

	// image height
	*output_ptr++ = (UINT8) (image_height >> 8);
	*output_ptr++ = (UINT8) image_height;

	// image width
	*output_ptr++ = (UINT8) (image_width >> 8);
	*output_ptr++ = (UINT8) image_width;

	// Nf
	*output_ptr++ = number_of_components;

	/* type 422*/
	*output_ptr++ = 0x01;
	*output_ptr++ = 0x21;
	

	*output_ptr++ = 0x00;
	*output_ptr++ = 0x02;
	*output_ptr++ = 0x11;
	*output_ptr++ = 0x01;

	*output_ptr++ = 0x03;
	*output_ptr++ = 0x11;
	*output_ptr++ = 0x01;


	// Scan header(SOF)

	// Start of scan marker
	*output_ptr++ = 0xFF;
	*output_ptr++ = 0xDA;

	header_length = (UINT16) (6 + (number_of_components << 1));

	// Scan header length
	*output_ptr++ = (UINT8) (header_length >> 8);
	*output_ptr++ = (UINT8) header_length;

	// Ns
	*output_ptr++ = number_of_components;

	/* type 422*/
	*output_ptr++ = 0x01;
	*output_ptr++ = 0x00;

	*output_ptr++ = 0x02;
	*output_ptr++ = 0x11;

	*output_ptr++ = 0x03;
	*output_ptr++ = 0x11;
	

	*output_ptr++ = 0x00;
	*output_ptr++ = 0x3F;
	*output_ptr++ = 0x00;
	
	return output_ptr;
}