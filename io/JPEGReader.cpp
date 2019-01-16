/*
 Morphometry with Automatic Extraction of Landmarks (MAELab)
 Copyright (C) 2017  Le Van Linh (van-linh.le@u-bordeaux.fr)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "JPEGReader.h"

Matrix<RGB> decompress_JPEG(const char* filename, int &rows, int &cols) {
	//std::cout << "\nBegin decompress JPEG image.\n";

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char* buffer;
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// step 2
	FILE * infile;
	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "cannot open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	Matrix<RGB> rgbMatrix((int)cinfo.output_height, (int)cinfo.output_width);
	rows = rgbMatrix.get_Rows();
	cols = rgbMatrix.get_Cols();

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = new unsigned char[cinfo.output_height * row_stride];
	//int i = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		int row = cinfo.output_scanline;
		unsigned char* p = buffer
				+ cinfo.output_scanline * cinfo.image_width
						* cinfo.num_components;
		jpeg_read_scanlines(&cinfo, &p, 1);
		for (int i = 0; i < cinfo.output_width; i++) {
			unsigned int k = (i + cinfo.output_width * row) * 3;
			RGB rgb;
			rgb.set_R((unsigned short int) buffer[k]);
			rgb.set_G((unsigned short int) buffer[k + 1]);
			rgb.set_B((unsigned short int) buffer[k + 2]);
			rgbMatrix.set_At_Position(row, i, rgb);
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	//std::cout << "\nFinished de-compress JPEG.\n";
	return rgbMatrix;
}
void save_JPEG(const char* filename, unsigned char *buffer, int width, int height,int components, J_COLOR_SPACE colorspace) {
	//std::cout << "\nBegin compress JPEG image.\n";

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	// step 2
	FILE * outfile;
	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "cannot open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = components;
	cinfo.in_color_space = colorspace;

	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

	JSAMPROW row_pointer[1];
	int row_stride = width * 3;

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);

	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
}
void RGB_2_JPEG(const char* filename, ptr_RGBMatrix rgbMatrix) {

	unsigned char *buffer = new unsigned char[rgbMatrix->get_Rows()
			* rgbMatrix->get_Cols() * 3];

	for (int r = 0; r < rgbMatrix->get_Rows(); r++) {
		for (int c = 0; c < rgbMatrix->get_Cols(); c++) {
			RGB value = rgbMatrix->get_At_Position(r, c);
			int i = (c + rgbMatrix->get_Cols() * r) * 3;
			buffer[i] = (unsigned char) value.get_R();
			buffer[i + 1] = (unsigned char) value.get_G();
			buffer[i + 2] = (unsigned char) value.get_B();
		}
	}

	save_JPEG(filename,buffer,rgbMatrix->get_Cols(),rgbMatrix->get_Rows(),3,JCS_RGB);
}
void Gray_2_JPEG(const char* filename, ptr_IntMatrix grayMatrix) {

	unsigned char *buffer = new unsigned char[grayMatrix->get_Rows()
			* grayMatrix->get_Cols() * 3];

	for (int r = 0; r < grayMatrix->get_Rows(); r++) {
		for (int c = 0; c < grayMatrix->get_Cols(); c++) {
			int value = grayMatrix->get_At_Position(r, c);
			int i = (c + grayMatrix->get_Cols() * r) * 3;
			buffer[i] = value;
			buffer[i + 1] = value;
			buffer[i + 2] = value;
		}
	}

	save_JPEG(filename,buffer,grayMatrix->get_Cols(),grayMatrix->get_Rows(),3,JCS_RGB);
}
