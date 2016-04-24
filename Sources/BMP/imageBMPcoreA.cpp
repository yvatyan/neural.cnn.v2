#include "Headers/imageBMPcore.h"

#define _CRT_SECURE_NO_WARNINGS

void my::ImageBMPcore::writeCoreData(const my::Matrix<my::RGBA> & _cmatrix) {
	
	byte buffer;
	
	fseek(this->image, this->imageOffset, SEEK_SET);
	dword height = _cmatrix.height();
	dword width = _cmatrix.width();
	dword correction = this->correction();
	
	for(dword i = 0; i < height; ++i){
		for(dword j = 0; j < width; ++j){
			byte rgb[4];
			rgb[0] = _cmatrix[i][j].B;
			rgb[1] = _cmatrix[i][j].G;
			rgb[2] = _cmatrix[i][j].R;
			rgb[3] = _cmatrix[i][j].A;
			fwrite(rgb, 1, 4, this->image);
		}
		buffer = 0;
		for(int j = 0; j < correction; ++j){
			fwrite(&buffer, 1, 1, this->image);
		}
	}
	return;
}
void my::ImageBMPcore::Create(const char *_path, const my::Matrix<my::RGBA> &_cmatrix) {
	
	if(this->image) fclose(this->image);
	this->image = fopen(_path, "wb");
	if(this->image == NULL) {
		throw std::invalid_argument("Can not open the file.");
	}
	this->bitCount = BITMAP::TYPE_32;
	this->bigEndian = false;
	
	this->writeCommonHeader();
	this->writeCoreBody();
	
	dword dbuffer;
	word wbuffer;
	
	fseek(this->image, 0x12, SEEK_SET);
	wbuffer = _cmatrix.width();
	fwrite(&wbuffer, 2, 1, this->image);
	this->imageWidth = wbuffer;
	
	wbuffer = _cmatrix.height();
	fwrite(&wbuffer, 2, 1, this->image);
	this->imageHeight = wbuffer;
	
	dword correction = this->correction();
	fseek(this->image, 0x02, SEEK_SET);
	dbuffer = (Functional::used_bytes(this->imageWidth*this->bitCount) + correction)*this->imageHeight + this->fileSize;
	fwrite(&dbuffer, 4, 1, this->image);
	this->fileSize = dbuffer;
	
	this->writeCoreData(_cmatrix);
	fclose(this->image);
	this->image = fopen(_path, "rb");
	if(this->image == NULL) {
		throw std::invalid_argument("Can not open the file.");
	}
	return;
}
void my::ImageBMPcore::Create(const char *_path, const my::Matrix<my::byte> & _imatrix, const std::vector<my::RGBA> & _ctable, BITMAP::TYPE _type) {

	if(this->image) fclose(this->image);
	this->image = fopen(_path, "wb");
	if(this->image == NULL) {
		throw std::invalid_argument("Can not open the file.");
	}
	this->bitCount = _type;
	this->bigEndian = false;
	
	this->writeCommonHeader();
	this->writeCoreBody();
	
	dword dbuffer;
	word wbuffer;
	
	fseek(this->image, 0x12, SEEK_SET);
	wbuffer = _imatrix.width();
	fwrite(&wbuffer, 2, 1, this->image);
	this->imageWidth = wbuffer;
	
	wbuffer = _imatrix.height();
	fwrite(&wbuffer, 2, 1, this->image);
	this->imageHeight = wbuffer;

	std::vector<RGBA> new_ctable(_ctable);
	if(_ctable.size() > (1 << this->bitCount))
		new_ctable.resize(1 << this->bitCount);

	dword correction = this->correction();
	fseek(this->image, 0x02, SEEK_SET);
	dbuffer = (Functional::used_bytes(this->imageWidth*this->bitCount) + correction)*this->imageHeight + this->fileSize + 4*new_ctable.size();
	fwrite(&dbuffer, 4, 1, this->image);
	this->fileSize = dbuffer;

	this->writeCoreData(_imatrix, new_ctable);
	fclose(this->image);
	this->image = fopen(_path, "rb");
	if(this->image == NULL) {
		throw std::invalid_argument("Can not open the file.");
	}
	return;
}
void my::ImageBMPcore::writeCoreData(const my::Matrix<byte> & _imatrix, const std::vector<my::RGBA> &_ctable) {
	
	fseek(this->image, 0x1a, SEEK_SET);
	dword dbuffer;
	dword correction = this->correction();
	byte  nulls[4] = {0, 0, 0, 0};
	this->clrTblOffset = 0x1a;
	this->clrTblSize = 1 << this->bitCount;
	this->clrTblUsed = this->clrTblSize;
	
	dword ct_i;
	for(ct_i = 0; ct_i < _ctable.size(); ++ct_i){
	
		byte colors[4];
		colors[0] = _ctable[ct_i].B;
		colors[1] = _ctable[ct_i].G;
		colors[2] = _ctable[ct_i].R;
		colors[3] = _ctable[ct_i].A;
		fwrite(colors, 1, 4, this->image);
	}
	for(; ct_i < this->clrTblSize; ++ct_i){
		fwrite(&nulls, 1, 4, this->image);
	}
	
	this->imageOffset = ftell(this->image);
	
	byte q = 8 / this->bitCount; // how much max should be compressed in byte (can be stored in byte)
	for(int i = _imatrix.height() - 1; i >= 0; --i){
		for(dword j = 0; j < _imatrix.width();){
			byte buffer[8], h;
			for(h = 0; h < q && j < _imatrix.width(); ++j, ++h){
				buffer[h] = _imatrix[i][j];
			}
			byte data = Functional::compressionBE(buffer, this->bitCount, h+1);
			fwrite(&data, 1, 1, this->image);
		}
		for(dword i = 0; i < correction; ++i) fwrite(&nulls, 1, 1, this->image);
	}
	dbuffer = this->imageOffset;
	fseek(this->image, 0x0A, SEEK_SET);
	fwrite(&dbuffer, 4, 1, this->image);
	return;
}
void my::ImageBMPcore::lowChannelCMatrix(my::Matrix<my::RGBA> & _matrix, const std::vector<my::RGBA> & _colorTable){
	
	dword correction = this->correction();
	dword height = this->ImHeight();
	dword width = this->ImWidth();
	dword d;
	if(this->imageHeight < 0) d = 0;
	else 					  d = this->ImHeight() - 1;
	dword max_bits;
	byte (*bit_value_func)(byte, byte);
	switch(this->bitCount){
		case	1	:	max_bits = 7; bit_value_func = Functional::value1Bit; break;
		case	2	:	max_bits = 3; bit_value_func = Functional::value2Bit; break;
		case	4	:	max_bits = 1; bit_value_func = Functional::value4Bit; break;
		case	8	:	max_bits = 0; bit_value_func = Functional::value8Bit; break;
	};
	
	fseek(this->image, this->imageOffset, SEEK_SET);
	for(dword i = 0; i < height; ++i){
		for(dword j = 0; j < width;){
			byte colorBitArray = Functional::convert(this->read(1), (byte)2);
			
			for(signed char h = max_bits; h >= 0 && j < width; --h, ++j)
				_matrix[(i-d) - 2*(i-d)*(d != 0)][j] = _colorTable[bit_value_func(colorBitArray, h)];
		}
		fseek(this->image, correction, SEEK_CUR);
	}
	return;
}
void my::ImageBMPcore::highChannelCMatrix(my::Matrix<my::RGBA> & _matrix){

	dword correction = this->correction();
	dword height = this->ImHeight();
	dword width = this->ImWidth();
	dword d;
	if(this->imageHeight < 0) d = 0;
	else 					  d = this->ImHeight() - 1;
	
	fseek(this->image, this->imageOffset, SEEK_SET);
	byte red, green, blue, alpha;
	for(dword i = 0; i < height; ++i) {
		for(dword j = 0; j < width; ++j) {
			if(this->bitCount == 32) {
				blue = Functional::convert(this->read(1), (byte)2);
				green = Functional::convert(this->read(1), (byte)2);
				red = Functional::convert(this->read(1), (byte)2);				
				alpha = Functional::convert(this->read(1), (byte)2);
			}
			else if(this->bitCount == 64) {
				blue = Functional::convert(this->read(2), (word)2) >> 8;
				green = Functional::convert(this->read(2), (word)2) >> 8;
				red = Functional::convert(this->read(2), (word)2) >> 8;
				alpha = Functional::convert(this->read(2), (word)2) >> 8;
			}
			RGBA color(red, green, blue, alpha);
			_matrix[(i-d) - 2*(i-d)*(d != 0)][j] = color;
		}
		fseek(this->image, correction, SEEK_CUR);
	}
	return;
}
my::Matrix<my::RGBA> my::ImageBMPcore::ColorMatrixA() {
	
	Matrix<RGBA> result(this->ImHeight(), this->ImWidth());
	
	if(this->bitCount <= 8){
		std::vector<RGBA> table(this->ColorTableA());
		this->lowChannelCMatrix(result, table);
	}
	else{
		this->highChannelCMatrix(result);
	}
	return result;
}
my::Matrix<bool> my::ImageBMPcore::MonoMatrix(const my::RGBA & _separator, bool _crossedToNull) {

	Matrix<bool> result(this->ImHeight(), this->ImWidth());
	Matrix<RGBA> colors(this->ColorMatrixA());
	
	for(dword i = 0; i < this->ImHeight(); ++i)
		for(dword j = 0; j < this->ImWidth(); ++j)
			result[i][j] = (colors[i][j] > _separator) || ((colors[i][j] != _separator) && !_crossedToNull);
	return result;
}
std::vector<my::RGBA> my::ImageBMPcore::ColorTableA () {
	
	if(this->bitCount > 8) throw std::invalid_argument("Can not get color table of 8bit greater bitmap.");

	fseek(this->image, this->clrTblOffset, SEEK_SET);
	std::vector<RGBA> table(this->clrTblSize);
	
	for(dword i = 0; i < table.size(); ++i){
		byte blue = Functional::convert(this->read(1), (byte)2);
		byte green = Functional::convert(this->read(1), (byte)2);
		byte red = Functional::convert(this->read(1), (byte)2);
		byte alpha = Functional::convert(this->read(1), (byte)2);
		RGBA color(red, green, blue, alpha);
		table[i] = color;
	}
	std::cout << this->image << std::endl
			  << this->clrTblOffset << std::endl 
			  << this->clrTblSize << std::endl;
	return table;

}

#undef _CRT_SECURE_NO_WARNINGS
