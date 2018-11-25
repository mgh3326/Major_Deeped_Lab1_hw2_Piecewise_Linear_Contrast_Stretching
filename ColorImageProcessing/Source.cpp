#include<iostream>
#include<string>
#include<algorithm>//for sort
using namespace std;

#define X_MAX 512
#define Y_MAX 512


unsigned char** mem_alloc2_d(int n_height, int n_width, unsigned char nlnit_val);

unsigned char** mem_alloc2_d(int n_height, int n_width, unsigned char nlnit_val)
{
	unsigned char** rtn = new unsigned char*[n_height];
	for (int n = 0; n < n_height; n++)
	{
		rtn[n] = new unsigned char[n_width];
		memset(rtn[n], nlnit_val, sizeof(unsigned char)*n_width);
	}
	return rtn;
}

unsigned char LinearContrastStretching(unsigned char input) {
	unsigned char out;
	if (input <= 50) out = round((25.0 / 50.0)*input);//25	50
	else if (input > 50 && input <= 120) out = round((double)((200.0 - 25.0) / (120.0 - 50.0)) * (input - 50.0) + 25.0);//200	120
	else if (input > 120) out = round((double)((255.0 - 200.0) / (255 - 120)) * (input - 120.0) + 200.0);//255	255
	return out;

}
typedef struct _RGB {
	unsigned char b;//받을때 bgr순서라서 이렇게 함
	unsigned char g;
	unsigned char r;
} RGB;
int readRGB(unsigned char** r, unsigned char** g, unsigned char** b, int nHeight_in, int nWidth_in, string file_name)
{
	RGB** rgb = new RGB*[nHeight_in];

	FILE* in = fopen(file_name.c_str(), "rb");
	if (in == NULL)
	{
		printf("File not found!!\n");
		exit(1);
		return 1;
	}

	for (int n = 0; n < nHeight_in; n++)
	{
		rgb[n] = new RGB[nWidth_in];
		memset(rgb[n], 0, sizeof(RGB) * nWidth_in);
	}
	for (int i = 0; i < nHeight_in; i++)
	{
		fread(rgb[i], sizeof(RGB), nWidth_in, in);
	}
	fclose(in);
	for (int h = 0; h < nHeight_in; h++)

	{
		for (int w = 0; w < nWidth_in; w++)

		{
			r[h][w] = rgb[h][w].r;
			g[h][w] = rgb[h][w].g;
			b[h][w] = rgb[h][w].b;
		}
	}
	return 0;
}
int writeRGB(unsigned char** r, unsigned char** g, unsigned char** b, int nHeight_in, int nWidth_in, string file_name)
{
	RGB** out = new RGB*[512];
	for (int n = 0; n < 512; n++)
	{
		out[n] = new RGB[512];
		memset(out[n], 0, sizeof(RGB) * 512);
	}
	for (int h = 0; h < nHeight_in; h++)

	{
		for (int w = 0; w < nWidth_in; w++)

		{
			out[h][w].r = r[h][w];
			out[h][w].g = g[h][w];
			out[h][w].b = b[h][w];

		}
	}
	FILE* outfile = fopen(file_name.c_str(), "w+b");
	for (int i = 0; i < Y_MAX; i++)
	{
		fwrite(out[i], sizeof(RGB), X_MAX, outfile);
	}

	return 0;


}
int main(void)
{
	int nHeight_in = Y_MAX;
	int nWidth_in = X_MAX;
	unsigned char** ch_in_r = mem_alloc2_d(nHeight_in, nWidth_in, 0);//rgb가 아니라 bgr로 들어오는것 같기도 하다. 한번 알아보고 해야겠다.
	unsigned char** ch_in_g = mem_alloc2_d(nHeight_in, nWidth_in, 0);
	unsigned char** ch_in_b = mem_alloc2_d(nHeight_in, nWidth_in, 0);


	readRGB(ch_in_r, ch_in_g, ch_in_b, nHeight_in, nWidth_in, "lena512.rgb");

	for (int h = 0; h < nHeight_in; h++)
	{
		for (int w = 0; w < nWidth_in; w++)

		{
			ch_in_r[h][w] = LinearContrastStretching(ch_in_r[h][w]);
			ch_in_g[h][w] = LinearContrastStretching(ch_in_g[h][w]);
			ch_in_b[h][w] = LinearContrastStretching(ch_in_b[h][w]);
		}
	}

	writeRGB(ch_in_r, ch_in_g, ch_in_b, nHeight_in, nWidth_in, "out.raw");

	return 0;
}