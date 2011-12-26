#include "jpeg_fun.h"

unsigned char *raw_image = NULL; // this pointer to store raw, uncompressed image

unsigned char *raw_gray = NULL; //this pointer to store downlscaled version of the image

unsigned int *raw_integral = NULL; //this pointer to store integral image of grayscaled image

//predetermined dimension , can be changed later
int width;// = 1600;
int height;// = 1200;
int bytes_per_pixel;// = 3;
int color_space = JCS_RGB;

/** read_jpeg_file reads from a jpeg file on disk and saves into raw_image buffer in an uncompressed format
    * \returns positive integer if successful, -1 otherwise
    * \param *filename char string specifiying the file name to read from
*/

int read_jpeg_file( char *filename )
{
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1];

	FILE *infile = fopen( filename, "rb" );
	unsigned long location = 0;
	int i = 0;

	if ( !infile )
	{
		printf("Error opening jpeg file %s\n!", filename );
		return -1;
	}
	/* here we set up the standard libjpeg error handler */
	cinfo.err = jpeg_std_error( &jerr );
	/* setup decompression process and source, then read JPEG header */
	jpeg_create_decompress( &cinfo );
	/* this makes the library read from infile */
	jpeg_stdio_src( &cinfo, infile );
	/* reading the image header which contains image information */
	jpeg_read_header( &cinfo, TRUE );

     /* Assign width, height and bytes_per_pixel for output image.*/
	width = cinfo.image_width; height = cinfo.image_height; bytes_per_pixel = cinfo.num_components;

	/* Uncomment the following to output image information, if needed. */

	printf( "JPEG File Information: \n" );
	printf( "Image width and height: %d pixels and %d pixels.\n", width, height );
	printf( "Color components per pixel: %d.\n", bytes_per_pixel );
	printf( "Color space: %d.\n", cinfo.jpeg_color_space );

	/* Start decompression jpeg here */
	jpeg_start_decompress( &cinfo );



	/* allocate memory to hold the uncompressed image */
	raw_image = (unsigned char*)malloc( width*height*bytes_per_pixel );
	/* now actually read the jpeg into the raw buffer */
	row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
	/* read one scan line at a time */
	while( cinfo.output_scanline < cinfo.image_height )
	{
		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
			raw_image[location++] = row_pointer[0][i];
	}
	/* wrap up decompression, destroy objects, free pointers and close open files */
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	free( row_pointer[0] );
	fclose( infile );
	/* yup, we succeeded! */
	return 1;
}

/**
 * write_jpeg_file Writes the raw image data stored in the raw_image buffer
 * to a jpeg image with default compression and smoothing options in the file
 * specified by *filename.
 *
 * \returns positive integer if successful, -1 otherwise
 * \param *filename char string specifying the file name to save to
 *
 */
int write_jpeg_file( char *filename )
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];
	FILE *outfile = fopen( filename, "wb" );

	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename );
		return -1;
	}
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	/* Setting the parameters of the output file here */
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = bytes_per_pixel;
	cinfo.in_color_space = color_space;
    /* default compression parameters, we shouldn't be worried about these */
	jpeg_set_defaults( &cinfo );
	/* Now do the compression .. */
	jpeg_start_compress( &cinfo, TRUE );
	/* like reading a file, this time write one row at a time */
	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &raw_image[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}
	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );
	/* success code is 1! */
	return 1;
}


int save_pixels(char *filename)
{
    int i=0,j=0, pixel_per_row =  width*bytes_per_pixel;
    int count = 0;
	unsigned char * rows_point;

    FILE *txtfile =  fopen(filename,"w");  //opening a textfile to store pixel values
    //return -1 if txtfile could not be opened
    if (!txtfile)
	{
		printf("Error opening output jpeg file %s\n!", filename );
		return -1;
	}
	else
        printf("\nfile with name %s is successfully created\n",filename);

    /* allocate memory to hold the grayscale image */
	raw_gray = (unsigned char*)malloc( width*height);

    //following code prints the grayscale values in txtfile
    for(i=0;i<height;i++)
    {
        rows_point = &raw_image[i*width*bytes_per_pixel];
        for(j = 0;j<pixel_per_row; j = j+3,count++)
        {
            //printf("%u ",*(rows_point+j)*59/100 + *(rows_point+j+1)*30/100 + *(rows_point+j+2)*11/100);
            *(raw_gray + count) = (unsigned char)(*(rows_point+j)*59/100 + *(rows_point+j+1)*30/100 + *(rows_point+j+2)*11/100);
            fprintf(txtfile,"%u ",(unsigned char)*(raw_gray+count));
        }
        //fprintf(txtfile,"%s","\n");
    }

    fclose(txtfile);
	return 0;

}


int write_grayscale(char *filename)
{
    struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];
	FILE *outfile = fopen( filename, "wb" );

	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename );
		return -1;
	}
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	/* Setting the parameters of the output file here */
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 1;
	cinfo.in_color_space = JCS_GRAYSCALE;
    /* default compression parameters, we shouldn't be worried about these */
	jpeg_set_defaults( &cinfo );
	/* Now do the compression .. */
	jpeg_start_compress( &cinfo, TRUE );
	/* like reading a file, this time write one row at a time */
	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &raw_gray[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}
	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );
	/* success code is 1! */
	return 1;
	}

int integral_raw(char *filename)
{
    int i,j,count=0, pixel_per_row =  width;
    unsigned int *raw_integral_temp;
    unsigned char * rows_point;
    raw_integral = (unsigned int*)malloc(height*width*sizeof(int));
    raw_integral_temp = (unsigned int*)malloc(height*width*sizeof(int));

//loop for converting char values to int value
    for(i=0;i<height;i++)
    {
        rows_point = &raw_image[i*width];
        for(j = 0;j<pixel_per_row;j++)
        {
            *(raw_integral_temp+i+j) = (int)*(raw_gray+i+j);
            //printf("%u ",*(raw_integral_temp+i+j));
        }
    }

//loop for calculating integral image

    for(i=0;i<height;i++)
    {
        rows_point = &raw_image[i*width];
        for(j = 0;j<pixel_per_row;j++,count++)
        {
            if(i==0&&j==0)
                *(raw_integral+i+j) = *(raw_integral_temp+i+j);
            else
            {
                if(i==0)
                {
                    *(raw_integral+i+j)= *(raw_integral+i+(j-1)) + *(raw_integral_temp+i+j);
                    continue;
                }
                else if(j==0)
                    *(raw_integral+i+j) = *(raw_integral+(i-1)+j)+*(raw_integral_temp+i+j);
                else
                    *(raw_integral+i+j) = *(raw_integral+i+(j-1)) + *(raw_integral+(i-1)+j)-*(raw_integral+(i-1)+(j-1))+*(raw_integral_temp+i+j);
            }

        //printf("%u ",*(raw_integral+i+j));
        }
    }


    printf("\ntotal no. of elements %d\n",count);
    return 0;
}
