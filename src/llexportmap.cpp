#include "../include/llexportmap.h"
#include "../include/llmaplist.h"

//constructor
llExportMap::llExportMap() : llMapWorker() {
	SetCommandName("ExportMap");
}

int llExportMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	filename = NULL;
	compress = 0;
	flipx = flipy = 0;

	return 1;
}

int llExportMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-filename", &filename);
	RegisterValue("-scale",    &scale);
	RegisterValue("-depth",    &bits);

	RegisterFlag ("-compress", &compress);
	RegisterFlag ("-FlipX",    &flipx);
	RegisterFlag ("-FlipY",    &flipy);
	
	return 1;
}

int llExportMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-filename"))
	    filename = (char *)"map.bmp";
	if (!Used("-depth"))
		bits = 24;

	//look for _install_dir:
	if (_llUtils()->GetValue("_install_dir")) {
		char *filename_tmp = new char[strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2];
		sprintf_s(filename_tmp, strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2, "%s\\%s", 
			_llUtils()->GetValue("_install_dir"), filename);
		//std::cout << _llUtils()->GetValue("_install_dir") << ":" << filename << std::endl;
		filename = filename_tmp;
	}


	FILE *fptr;

	if (fopen_s(&fptr, filename, "wb")) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"Unable to open BMP file '%s'\n", filename);
		return 0;
	}

	int x1 = map->GetRawX(_llUtils()->x00);
	int y1 = map->GetRawY(_llUtils()->y00);
	int x2 = map->GetRawX(_llUtils()->x11);
	int y2 = map->GetRawY(_llUtils()->y11);

	if (!map->IsUneven()) {
		x1 = map->GetRawX(_llUtils()->x00 + map->GetWidthXPerRaw()*.01f);
		y1 = map->GetRawY(_llUtils()->y00 + map->GetWidthYPerRaw()*.01f);
		x2 = map->GetRawX(_llUtils()->x11 - map->GetWidthXPerRaw()*.01f);
		y2 = map->GetRawY(_llUtils()->y11 - map->GetWidthYPerRaw()*.01f);
	}

	INFOHEADER infoheader;
	infoheader.width  = x2-x1+1;
	infoheader.height = y2-y1+1;

	//std::cout << x1 << ":" << y1 << ":" << x2 << ":" << y2 << ":" << infoheader.width << ":" << infoheader.height << std::endl;

	infoheader.compression=0;
	infoheader.size   = 40;  
	infoheader.planes = 1;       /* Number of colour planes   */
	int bytesPerLine;
	if (bits == 24)
		infoheader.bits = 24;         /* Bits per pixel            */
	else
		infoheader.bits = 32;
	if (bits == 24) {
		bytesPerLine = infoheader.width * 3;  /* (for 24 bit images) */
		/* round up to a dword boundary */
		//Thanks to 
		//http://www.siggraph.org/education/materials/HyperVis/asp_data/compimag/bmpfile.htm
		//for the hint
		if (bytesPerLine & 0x0003) {
			bytesPerLine |= 0x0003;
			++bytesPerLine;
		}		     
	} else
		bytesPerLine = infoheader.width * 4;
	infoheader.imagesize = (long)bytesPerLine*infoheader.height;          /* Image size in bytes  */
	infoheader.xresolution=100;
	infoheader.yresolution=100;     /* Pixels per meter          */
	infoheader.ncolours=0;          /* Number of colours         */
	//infoheader.importantcolours;    /* Important colours         */

	HEADER header;
	header.type='M'*256+'B';
	header.size = 14 + 40 + infoheader.imagesize;
	header.reserved1 = header.reserved2 = 0;
	WriteUShort(fptr, header.type, 0);
	WriteUInt  (fptr, header.size, 0);
	WriteUShort(fptr, header.reserved1, 0);
	WriteUShort(fptr, header.reserved2, 0);
	header.offset = 14+40;
	WriteUInt(fptr, header.offset, 0);

	/* Read and check the information header */
	if (fwrite(&infoheader, sizeof(INFOHEADER), 1, fptr) != 1) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "Failed to write BMP info header");
		return 0;
	}

	for (int y=y2; y>=y1; y--) {
		int my_bytesPerLine = bytesPerLine;
		for (int x=x1; x<=x2; x++) {
			
			unsigned char byte1;
			unsigned char byte2;
			unsigned char byte3;
			unsigned char byte4;

			int tupel = 0;

			int xl = x;
			int yl = y;
			if (flipx) xl = x2 - (x - x1);
			if (flipy) yl = y2 - (y - y1);

			if (Used("-scale")) {
				tupel = map->GetTupelScaled(xl, yl, &byte1, &byte2, &byte3, &byte4, scale);			
			} else {
				tupel = map->GetTupel(xl, yl, &byte1, &byte2, &byte3, &byte4);
			}

			if (tupel) {
				if (bits == 24) {
					my_bytesPerLine -= 3;
					fwrite(&byte1, 1, 1, fptr); //blue
					fwrite(&byte2, 1, 1, fptr); //green
					fwrite(&byte3, 1, 1, fptr); //red
					//std::cout << byte1 << ":" << byte2 << ":" << byte3 << std::endl;
				} else {
					fwrite(&byte1, 1, 1, fptr); //blue
					fwrite(&byte2, 1, 1, fptr); //green
					fwrite(&byte3, 1, 1, fptr); //red
					fwrite(&byte4, 1, 1, fptr); //alpha
					my_bytesPerLine -= 4;
				}
			}

#if 0
			float val = map->GetElementRaw(x,y);
			if (bits == 24) {
				my_bytesPerLine -= 3;
				unsigned int trunk1 = int(val) & 0xff;
				unsigned int trunk2 = (int(val) & 0xff00) >> 8;
				unsigned int trunk3 = (int(val) & 0xff0000) >> 16;
				fwrite(&trunk1, 1, 1, fptr); //blue
				fwrite(&trunk2, 1, 1, fptr); //green
				fwrite(&trunk3, 1, 1, fptr); //red
			} else {
				WriteUInt(fptr,unsigned int(val), 0);
				my_bytesPerLine -= 4;
			}
#endif
		}
		while(my_bytesPerLine) {
			my_bytesPerLine--;
			unsigned int trunk1 = 0xff;
			fwrite(&trunk1, 1, 1, fptr);
		}
	}
	fclose(fptr);

	if (strlen(_llUtils()->GetValue("_dds_tool")) > 1 && compress) {
		char command[1000];
		sprintf_s(command,1000,"%s %s \n", _llUtils()->GetValue("_dds_tool"), filename);
		_llLogger()->WriteNextLine(LOG_INFO, "Executing '%s %s'", _llUtils()->GetValue("_dds_tool"), filename);
		_llLogger()->Dump();
		FILE *tes = _popen(command, "rt");
		char c; 
		if (tes==NULL) {
			_llLogger()->WriteNextLine(LOG_ERROR, "Error calling '%s'", _llUtils()->GetValue("_dds_tool"));
		} else {
			int nn=0;
			do {
				c = fgetc (tes);
				if ((isprint(c) || isspace(c)) && nn==0)
					std::cout << c;
				else if (isprint(c) || isspace(c)) nn++;
				else nn--;

			} while (c != EOF);
			fclose (tes);
		}
	}

	return 1;
}
