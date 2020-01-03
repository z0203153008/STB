
//字符串二维码转换成bmp 565格式
bool ui_qrcode_to_pic(char *p_input, char *name)
{
	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char*	pRGBData, *pSourceData, *pDestData;
	QRcode*			pQRC;
	handle_t		f;

	// Compute QRCode
	if ((pQRC = QRcode_encodeString(p_input, 0, QR_ECLEVEL_L, QR_MODE_8, 1)) != NULL)
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;

		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;

		unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

		// Allocate pixels buffer

		if (!(pRGBData = (unsigned char*)GxCore_Malloc(unDataBytes)))
		{
			printf("Errorr ! Out of memory!\n");
			return false;
		}

		// Preset to white

		memset(pRGBData, 0xff, unDataBytes);

		// Convert QrCode bits to bmp pixels
		pSourceData = pQRC->data;

		for(y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;

			for(x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for(l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
					{
						for(n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
						{
							*(pDestData +		n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_B;
							*(pDestData + 1 +	n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_G;
							*(pDestData + 2 +	n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_R;
						}
					}
				}

				pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
				pSourceData++;
			}
		}

		QRcode_free(pQRC);

		// Part.1 Create Bitmap File Header
	    BITMAPFILEHEADER fileHeader;

	    fileHeader.bfType = 0x4D42;
	    fileHeader.bfReserved1 = 0;
	    fileHeader.bfReserved2 = 0;
	    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RgbQuad) * 3 + unDataBytes;
	    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RgbQuad) * 3;

		printf("%lx %lx\n", fileHeader.bfSize, fileHeader.bfOffBits);

		// Part.2 Create Bitmap Info Header
	    BITMAPINFOHEADER bitmapHeader = { 0 };
	    memset(&bitmapHeader,0,sizeof(BITMAPINFOHEADER));
	    bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	    bitmapHeader.biHeight = unWidth * OUT_FILE_PIXEL_PRESCALER;
	    bitmapHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
	    bitmapHeader.biPlanes = 1;
	    bitmapHeader.biBitCount = 16;
	    bitmapHeader.biSizeImage = unDataBytes/3*2;
	    bitmapHeader.biCompression = BI_BITFIELDS; //BI_BITFIELDS

		printf("%lx %lx\n", bitmapHeader.biSize, bitmapHeader.biSizeImage);

		RgbQuad bmiClr[3]; //定义调色板

		//RGB565格式掩码
		bmiClr[0].rgbBlue = 0;
		bmiClr[0].rgbGreen = 0xF8;
		bmiClr[0].rgbRed = 0;
		bmiClr[0].rgbReserved = 0;

		bmiClr[1].rgbBlue = 0xE0;
		bmiClr[1].rgbGreen = 0x07;
		bmiClr[1].rgbRed = 0;
		bmiClr[1].rgbReserved = 0;

		bmiClr[2].rgbBlue = 0x1F;
		bmiClr[2].rgbGreen = 0;
		bmiClr[2].rgbRed = 0;
		bmiClr[2].rgbReserved = 0;

		if (GxCore_FileExists(name) == GXCORE_FILE_EXIST)
		{
			printf("%s exist, delete!\n", name);
			GxCore_FileDelete(name);
		}

	    // Write to file
	    if((f = GxCore_Open(name, "wb")) >= 0)
	    {
			GxCore_Write(f, &fileHeader, sizeof(BITMAPFILEHEADER), 1);
			GxCore_Write(f, &bitmapHeader, sizeof(BITMAPINFOHEADER), 1);
			GxCore_Write(f, &bmiClr, sizeof(RgbQuad), 3);
			unsigned short rgb565data = 0;

			for (x=0; x < unWidth*OUT_FILE_PIXEL_PRESCALER; x++)
			{
				for (y=0; y < unWidth*OUT_FILE_PIXEL_PRESCALER; y++)
				{
					//888 r|g|b -> 565 b|g|r
					rgb565data =(((pRGBData[2] >> 3) & 0x1F) << 11)//b
								|(((pRGBData[1] >> 2) & 0x3F) << 5)//g
								|(((pRGBData[0] >> 3) & 0x1F) << 0);//r

					GxCore_Write(f, &rgb565data, sizeof(rgb565data), 1);
					pRGBData += 3;
				}
			}

			GxCore_Close(f);
	        printf("Create file %s!\n", name);
	    }
	    else
	    {
	        printf("Cannot open file!\n");
	    }

		// Free data
		GxCore_Free(pRGBData);
	}
	else
	{
		printf("Error! NULL returned\n");
		return false;
	}

	return true;
}
