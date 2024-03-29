#include <string.h>
#include <malloc.h>

#include "Text_Operations.h"
#include "types.h"

#define DELIMITER 0xff
#define EOT	0x00

void XORText(ubyte_t *dest, const ubyte_t *op, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		dest[i] ^= op[i];
	}
}

int FormatPlainText(const ubyte_t *text, ubyte_t **out, int length)
{
	int i;
	int out_pos = 0;
	int del_count = 0;
	int block_count;
	int out_length;
	ubyte_t * out_temp;

	for (i = 0; i < length; ++i) {
		if (text[i] == DELIMITER)
			++del_count;
	}

	block_count = (length+del_count+2+BLOCK_BYTE_SIZE-1) / BLOCK_BYTE_SIZE;
	out_length = block_count * BLOCK_BYTE_SIZE;

	out_temp = (ubyte_t *) malloc(sizeof(ubyte_t)*out_length);

	out_pos = 0;
	for (i = 0; i < length; ++i) {
		if (text[i] == DELIMITER) {
			out_temp[out_pos++] = DELIMITER;
			out_temp[out_pos++] = DELIMITER;
		} else {
			out_temp[out_pos++] = text[i];
		}
	}

	// the "end of text" delimiter
	out_temp[out_pos++] = DELIMITER;
	out_temp[out_pos++] = EOT;

	*out = out_temp;

	return block_count;
}

int ParsePlainText(const ubyte_t *text, ubyte_t **out, int length)
{
	int i;
	int del_count;
	int out_length;
	int out_pos;
	ubyte_t * out_temp;

	del_count = 0;
	for (i = 0; i < length; ++i) {
		if (text[i] == DELIMITER) {
			++i;
			if (text[i] == DELIMITER) {
				++del_count;
			} else if (text[i] == EOT) {
				out_length = i - del_count - 1;
				break;
			} else {
				fprintf(stderr, "Shouldn't reach here 1\n");
			}
		}
	}
	out_length = i - del_count - 1;

	out_temp = (ubyte_t *) malloc(sizeof(ubyte_t)*out_length + 1);
	out_temp[out_length] = 0x00;

	out_pos = 0;
	for (i = 0; i < length; ++i) {
		if (text[i] == DELIMITER) {
			++i;
			if (text[i] == DELIMITER) {
				out_temp[out_pos++] = DELIMITER;
				continue;
			} else if (text[i] == EOT) {
				break;
			} else {
				fprintf(stderr, "Shouldn't reach here 2\n");
			}
		} else {
			out_temp[out_pos++] = text[i];
		}
	}
	*out = out_temp;
	return out_length;
}
