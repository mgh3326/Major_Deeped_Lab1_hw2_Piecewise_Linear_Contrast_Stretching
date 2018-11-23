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

unsigned char** padding(unsigned char** in, int n_height, int n_width, int n_filter_size)
{
	int n_pad_size = (int)(n_filter_size / 2);
	unsigned char** pad = mem_alloc2_d(n_height + 2 * n_pad_size, n_width + 2 * n_pad_size, 0);

	for (int h = 0; h < n_height; h++)
	{
		for (int w = 0; w < n_width; w++)
		{
			pad[h + n_pad_size][w + n_pad_size] = in[h][w];
		}
	}

	for (int h = 0; h < n_pad_size; h++)
	{
		for (int w = 0; w < n_width; w++)
		{
			pad[h][w + n_pad_size] = in[0][w];
			pad[h + (n_height - 1)][w + n_pad_size] = in[n_height - 1][w];
		}
	}

	for (int h = 0; h < n_height; h++)
	{
		for (int w = 0; w < n_pad_size; w++)
		{
			pad[h + n_pad_size][w] = in[h][0];
			pad[h + n_pad_size][w + (n_width - 1)] = in[h][n_width - 1];
		}
	}

	for (int h = 0; h < n_pad_size; h++)
	{
		for (int w = 0; w < n_pad_size; w++)
		{
			pad[h][w] = in[0][0];
			pad[h + (n_height - 1)][w] = in[n_height - 1][0];
			pad[h][w + (n_width - 1)] = in[0][n_width - 1];
			pad[h + (n_height - 1)][w + (n_width - 1)] = in[n_height - 1][n_width - 1];
		}
	}

	return pad;
}
unsigned char LinearContrastStretching(unsigned char input) {
	//직선 기울기에 따라 바뀌는걸 만들어보자
// ( 50,10) , (200,210)
	// (100,10) (150,200)
	// (50,30) (200, 230)
// y = (1/5)x
// y= 4x -190
// y = (10/21)x
	unsigned char out;
	/*if (input < 50.0)
	{
		out = (30.0 / 50.0)*input;
		return out;

	}
	else if (input < 200.0)
	{
		out = ((230.0-30.0)/(200.0-50.0))*(input - 50.0) + 30.0 ;
		return out;

	}
	else if(input <256) {
		out = ((255.0-230.0/255.0-200.0)) * (input- 200.0) + 230.0;
		return out;

	}*/
	if (input <= 50) out = round((25.0 / 50.0)*input);//25	50
	else if (input > 50 && input <= 120) out = round((double)((200.0-25.0) / (120.0-50.0)) * (input - 50.0) + 25.0);//200	120
	else if (input > 120) out = round((double)((255.0-200.0)/(255-120)) * (input - 120.0) + 200.0);//255	255
	return out;

}
int main(void)
{
	unsigned char** in_data = mem_alloc2_d(X_MAX, Y_MAX, 0);


	FILE* in = fopen("lena512.rgb", "rb");
	if (in == NULL)
	{
		printf("File not found!!\n");
		return 0;
	}


	/*FILE* in_input = fopen("filter_input.raw", "rb");*/



	int nHeight_in = Y_MAX;
	int nWidth_in = X_MAX;
	unsigned char** ch_in_1 = mem_alloc2_d(nHeight_in, nWidth_in, 0);

	unsigned char** ch_in_2 = mem_alloc2_d(nHeight_in, nWidth_in, 0);

	unsigned char** ch_in_3 = mem_alloc2_d(nHeight_in, nWidth_in, 0);



	typedef struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} RGB;

	RGB** rgb = new RGB*[512];
	for (int n = 0; n < 512; n++)
	{
		rgb[n] = new RGB[512];
		memset(rgb[n], 0, sizeof(RGB) * 512);
	}
	for (int i = 0; i < Y_MAX; i++)
	{
		fread(rgb[i], sizeof(RGB), X_MAX, in);
	}
	fclose(in);
	for (int h = 0; h < nHeight_in; h++)

	{
		for (int w = 0; w < nWidth_in; w++)

		{
			ch_in_1[h][w] = rgb[h][w].r;
			ch_in_2[h][w] = rgb[h][w].g;
			ch_in_3[h][w] = rgb[h][w].b;
		}
	}
	for (int h = 0; h < nHeight_in; h++)
	{
		for (int w = 0; w < nWidth_in; w++)

		{
			ch_in_1[h][w] = LinearContrastStretching(ch_in_1[h][w]);
			ch_in_2[h][w] = LinearContrastStretching(ch_in_2[h][w]);
			ch_in_3[h][w] = LinearContrastStretching(ch_in_3[h][w]);
		}
	}
	//FILE* outfile1 = fopen("input.raw", "w+b");
	//for (int i = 0; i < Y_MAX; i++)
	//{
	//	fwrite(rgb[i], sizeof(RGB), X_MAX, outfile1);
	//}
	//직선 기울기에 따라 바뀌는걸 만들어보자
	// ( 50,10) , (100,210)
	// y = (1/5)x
	// y= 4x -190
	// y = (10/21)x
	/*for (int h = 0; h < nHeight_in; h++)

	{
		for (int w = 0; w < nWidth_in; w++)

		{
		
			ch_in_1[h][w] = rgb[h][w].r;
			ch_in_2[h][w] = rgb[h][w].g;
			ch_in_3[h][w] = rgb[h][w].b;
		}
	}*/
	/*fclose(in_input);*/
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
			out[h][w].r = ch_in_1[h][w];
			out[h][w].g = ch_in_2[h][w];
			out[h][w].b = ch_in_3[h][w];
			
		}
	}
	FILE* outfile = fopen("ouput2.raw", "w+b");
	for (int i = 0; i < Y_MAX; i++)
	{
		fwrite(out[i], sizeof(RGB), X_MAX, outfile);
	}

	//sharpening_filter(in_data_Filter1);
	//in_input = fopen("lena256_n5.raw", "rb");
	//unsigned char** in_data_2 = mem_alloc2_d(X_MAX, Y_MAX, 0);

	//if (in_input == NULL)
	//{
	//	printf("File not found!!\n");
	//	return 0;
	//}
	//for (int i = 0; i < Y_MAX; i++)
	//{
	//	fread(in_data_2[i], sizeof(char), X_MAX, in_input);
	//}
	//fclose(in_input);
	//median_filter(3, in_data_2, "n5");
	//in_input = fopen("lena256_n10.raw", "rb");

	//unsigned char** in_data_3 = mem_alloc2_d(X_MAX, Y_MAX, 0);

	//if (in_input == NULL)
	//{
	//	printf("File not found!!\n");
	//	return 0;
	//}
	//for (int i = 0; i < Y_MAX; i++)
	//{
	//	fread(in_data_3[i], sizeof(char), X_MAX, in_input);
	//}
	//fclose(in_input);
	//median_filter(3, in_data_3, "n10");
	//in_input = fopen("lena256_n25.raw", "rb");


	//unsigned char** in_data_4 = mem_alloc2_d(X_MAX, Y_MAX, 0);

	//if (in_input == NULL)
	//{
	//	printf("File not found!!\n");
	//	return 0;
	//}
	//for (int i = 0; i < Y_MAX; i++)
	//{
	//	fread(in_data_4[i], sizeof(char), X_MAX, in_input);
	//}
	//fclose(in_input);
	//median_filter(3, in_data_4, "n25");
	//

	return 0;
}