#include <stdio.h>

#include "key.h"
#include "ECB.h"
#include "CBC.h"
#include "CFB.h"
#include "OFB.h"

#define TEST_DATA_LEN 1024

#define ECB (0)
#define CBC (0)
#define CFB (0)
#define OFB (0)
#define CTR (1)

int main()
{
	ubyte_t data[TEST_DATA_LEN];
	ubyte_t *c_data;
	ubyte_t *p_data;
	int i;
	int c_len;
	int p_len;
	FILE *f1, *f2, *f3;

	f1 = fopen("ORG.dat", "wb");
	f2 = fopen("C_DATA.dat", "wb");
	f3 = fopen("P_DATA.dat", "wb");

	/*printf("ORG:\n");*/
	for (i = 0; i < TEST_DATA_LEN; i++) {
		fprintf(f1, " %02X", data[i]);
		if (i % 30 == 29)
			fprintf(f1, "\n");
	}

	if (ECB) {
		c_len = ECBEncrypt(data, test_key, &c_data, TEST_DATA_LEN);
	} else if (CBC) {
		c_len = CBCEncrypt(data, test_key, test_V, &c_data, TEST_DATA_LEN);
	} else if (CFB) {
		c_len = CFBEncrypt(data, test_key, test_V, &c_data, TEST_DATA_LEN);
	} else if (OFB) {
		c_len = OFBEncrypt(data, test_key, test_V, &c_data, TEST_DATA_LEN);
	} else if (CTR) {
		c_len = CTREncrypt(data, test_key, test_V, &c_data, TEST_DATA_LEN);
	}
	printf("Encrypt done: %d bytes\n", c_len);
	for (i = 0; i < c_len; i++) {
		fprintf(f2, " %02X", c_data[i]);
		if (i % 30 == 29)
			fprintf(f2, "\n");
	}

	printf("write done\n");

	if (ECB) {
		p_len = ECBDecrypt(c_data, test_key, &p_data, c_len);
	} else if (CBC) {
		p_len = CBCDecrypt(c_data, test_key, test_V, &p_data, c_len);
	} else if (CFB) {
		p_len = CFBDecrypt(c_data, test_key, test_V, &p_data, c_len);
	} else if (OFB) {
		p_len = OFBDecrypt(c_data, test_key, test_V, &p_data, c_len);
	} else if (CTR) {
		p_len = CTRDecrypt(c_data, test_key, test_V, &p_data, c_len);
	}
	printf("Decrypt done: %d bytes\n", p_len);
	for (i = 0; i < p_len; i++) {
		fprintf(f3, " %02X", p_data[i]);
		if (i % 30 == 29)
			fprintf(f3, "\n");
	}
	printf("write done\n");

	fclose(f1);
	fclose(f2);
	fclose(f3);
	return 0;
}