//rgb_buffer： 24位bmp数据（去掉bmp的head）
//调用完函数要free掉 *jpeg_buffer 
int rgb2jpeg(unsigned char* rgb_buffer, unsigned int width, unsigned int height, int quality,\
	unsigned char** jpeg_buffer, unsigned long* jpeg_size)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int row_stride = 0;
    JSAMPROW row_pointer[1];

    if (jpeg_buffer == NULL)
    {
        printf("you need a pointer for jpeg buffer.\n");
        return -1;
    }

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    jpeg_mem_dest(&cinfo, jpeg_buffer, jpeg_size);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, 1);  // todo 1 == true
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = width * cinfo.input_components;

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &rgb_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

	printf("jpg error code = %d\n", (int)cinfo.err);

    return 0;
}
