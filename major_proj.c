#include "jpeg_fun.h"

int main(int argc, char *argv[])
{
	char *infilename = argv[1];
	char *outfilename = "../test_out.jpg", *textfile = "../textfile.txt", *inte_file = "../integral_image", *gray_file = "../gray_image.jpg"; /* Try opening a jpeg*/
	if( read_jpeg_file( infilename ) > 0 )
	{
		/* then copy it to another file */
		if( write_jpeg_file( outfilename ) < 0 ) return -1;
		if(save_pixels(textfile)<0) return -1;
		if(integral_raw(inte_file)<0) return -1;
		if(write_grayscale(gray_file)<0) return -1;
	}
	else return -1;
	return 0;

}
