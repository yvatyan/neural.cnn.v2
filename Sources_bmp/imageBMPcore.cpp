#include "Headers/imageBMPcore.h"

#define _CRT_SECURE_NO_WARNINGS

my::dword my::ImageBMPcore::correction() {
	dword used_bits  = this->ImWidth() * this->bitCount;
	dword used_bytes = Functional::used_bytes(used_bits);
	return Functional::quadDivGE(used_bytes) - used_bytes;				
}
my::byte * my::ImageBMPcore::read(my::byte _count) {
	return Functional::read(_count, this->image, this->bigEndian);
}
my::dword my::ImageBMPcore::colorTableOffset() {
	if(this->imageOffset != 0x0000001A)
		if(this->bitCount <= 8) return 0x0000001A;
	return 0x00000000;
}
my::dword my::ImageBMPcore::ImageSize() {
	return (Functional::used_bytes(Functional::absolute(this->imageWidth)*this->bitCount) + 
				this->correction())*Functional::absolute(this->imageHeight);
}
my::dword my::ImageBMPcore::ImWidth() {
	return Functional::absolute(this->imageWidth);
}
my::dword my::ImageBMPcore::ImHeight() {
	return Functional::absolute(this->imageHeight);
}
my::ImageBMPcore::ImageBMPcore() {
	this->image = NULL;
	this->compatibility = true;
}
void my::ImageBMPcore::Create(const char *_path, const my::Matrix<my::RGB>  &_cmatrix) {
	
	if(this->image) fclose(this->image);
	this->image = fopen(_path, "wb");
	if(this->image == NULL) {
		throw std::invalid_argument("Can not open the file.");
	}
	this->bitCount = BITMAP::TYPE_24;
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
void my::ImageBMPcore::Create(const char *_path, const my::Matrix<my::byte> &_imatrix, const std::vector<my::RGB> &_ctable, BITMAP::TYPE _type) {

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

	std::vector<RGB> new_ctable(_ctable);
	if(_ctable.size() > (1 << this->bitCount))
		new_ctable.resize(1 << this->bitCount);

	dword correction = this->correction();
	fseek(this->image, 0x02, SEEK_SET);
	dbuffer = (Functional::used_bytes(this->imageWidth*this->bitCount) + correction)*this->imageHeight + this->fileSize + 3*new_ctable.size();
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
void my::ImageBMPcore::Create(const char *_path, const Matrix<bool> &_mmatrix) {

	std::vector<RGB> ctable(2);
	ctable[0] = RGB(0, 0, 0);
	ctable[1] = RGB(255, 255, 255);
	Matrix<byte> imatrix(_mmatrix.height(), _mmatrix.width());

	for(int i = 0; i < _mmatrix.height(); ++i) {
		for(int j = 0; j < _mmatrix.width(); ++j) {
			imatrix[i][j] = _mmatrix[i][j];
		}
	}
	return this->Create(_path, imatrix, ctable, BITMAP::TYPE_MONO);
}
void my::ImageBMPcore::Create(const char *_path, const my::Matrix<my::byte>  &_gmatrix) {
	
	Matrix<RGB> colored(_gmatrix.height(), _gmatrix.width());
	
	for(dword i = 0; i < colored.height(); ++i) {
		for(dword j = 0; j < colored.width(); ++j) {
			byte c = _gmatrix[i][j];
			colored[i][j] = RGB(c, c, c);
		}
	}
	return this->Create(_path, colored);
}
void my::ImageBMPcore::load() {

	this->fileSize = my::Functional::convert(this->read(4), (dword)2);
	fseek(this->image, 4, SEEK_CUR);
	this->imageOffset = my::Functional::convert(this->read(4), (dword)2);
	this->infoSize = my::Functional::convert(this->read(4), (dword)2);

	if(this->infoSize == 12){
		this->imageWidth = (int) my::Functional::convert(this->read(2), (word)2);
		this->imageHeight = (int) my::Functional::convert(this->read(2), (word)2);
	}
	else{
		if(this->compatibility) {
			this->imageWidth = my::Functional::convert(this->read(4), (int)2);
			this->imageHeight = my::Functional::convert(this->read(4), (int)2);
		}
		else throw std::invalid_argument("Compatibility with newer versions is turned off.");
	}

	this->ico = my::Functional::convert(this->read(2), (word)2);
	this->bitCount = my::Functional::convert(this->read(2), (word)2);
	this->clrTblOffset = colorTableOffset();
	
	this->clrTblSize = 1 << this->bitCount;
	this->clrTblUsed = this->clrTblSize;
}
void my::ImageBMPcore::Info() {

	std::cout << std::hex;
	std::cout << "data  offset: " << this->imageOffset << std::endl;
	if(this->clrTblOffset != 0)
		std::cout << "table offset: " << this->clrTblOffset << std::endl;
	else
		std::cout << "table offset: " << "doesn\'t exist" << std::endl;
	std::cout << std::dec;
	
	switch(this->infoSize){
		case 12		:	std::cout << "version: CORE" << std::endl; break;
		case 40		:	std::cout << "version: 3" << std::endl; break;
		case 108	:	std::cout << "version: 4" << std::endl; break;
		case 124	:	std::cout << "version: 5" << std::endl; break;
		default		:	std::cout << "version: not identified" << std::endl; break;
	};
	std::cout << "file size: " << this->fileSize << " bytes" << std::endl;
	std::cout << "picture size: " << this->imageWidth << 'x' << this->imageHeight << " pixels" << std::endl;
	std::cout << "data size: " << this->ImageSize() << " bytes" << std::endl;
	std::cout << "bits for pixel: " << this->bitCount << std::endl;
}
void my::ImageBMPcore::writeCommonHeader() {
	
	word wbuffer;
	dword dbuffer;
	fseek(this->image, 0, SEEK_SET);
	
	wbuffer = 0x4D42;
	fwrite(&wbuffer, 2, 1, this->image);
	
	dbuffer = 26;
	fwrite(&dbuffer, 4, 1, this->image);
	this->fileSize = dbuffer;
	
	wbuffer = 0;
	fwrite(&wbuffer, 2, 1, this->image);
	fwrite(&wbuffer, 2, 1, this->image);
	
	dbuffer = 26;
	fwrite(&dbuffer, 4, 1, this->image);
	this->imageOffset = dbuffer;

	this->clrTblOffset = 0;
	return;
}
void my::ImageBMPcore::writeCoreBody() {

	word wbuffer;
	dword dbuffer;
	fseek(this->image, 0x0E, SEEK_SET);
	
	dbuffer = 12;
	fwrite(&dbuffer, 4, 1, this->image);
	this->infoSize = dbuffer;
	
	wbuffer = 0;
	fwrite(&wbuffer, 2, 1, this->image);
	fwrite(&wbuffer, 2, 1, this->image);
	this->imageWidth = this->imageHeight = wbuffer;
	
	wbuffer = 1;
	fwrite(&wbuffer, 2, 1, this->image);
	this->ico = wbuffer;
	
	wbuffer = this->bitCount;
	fwrite(&wbuffer, 2, 1, this->image);
	
	this->clrTblSize = this->clrTblUsed = 0;
	return;
}
void my::ImageBMPcore::writeCoreData(const my::Matrix<my::RGB> & _cmatrix) {
	
	byte buffer;
	
	fseek(this->image, this->imageOffset, SEEK_SET);
	dword height = _cmatrix.height();
	dword width = _cmatrix.width();
	dword correction = this->correction();
	
	for(dword i = height - 1; i != UINT_MAX; --i){
		for(dword j = 0; j < width; ++j) {
			byte rgb[3];
			rgb[0] = _cmatrix[i][j].B;
			rgb[1] = _cmatrix[i][j].G;
			rgb[2] = _cmatrix[i][j].R;
			fwrite(rgb, 1, 3, this->image);
		}
		buffer = 0;
		for(int j = 0; j < correction; ++j){
			fwrite(&buffer, 1, 1, this->image);
		}
	}
	return;
}
void my::ImageBMPcore::writeCoreData(const my::Matrix<byte> &_imatrix, const std::vector<my::RGB> &_ctable) {
	
	fseek(this->image, 26, SEEK_SET);
	dword dbuffer;
	dword correction = this->correction();
	byte  nulls[4] = {0, 0, 0, 0};
	this->clrTblOffset = 0x1a;
	this->clrTblSize = 1 << this->bitCount;
	
	dword ct_i;
	for(ct_i = 0; ct_i < _ctable.size(); ++ct_i){
	
		byte colors[3];
		colors[0] = _ctable[ct_i].B;
		colors[1] = _ctable[ct_i].G;
		colors[2] = _ctable[ct_i].R;
		fwrite(colors, 1, 3, this->image);
	}
	for(; ct_i < this->clrTblSize; ++ct_i){
		fwrite(&nulls, 1, 3, this->image);
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
void my::ImageBMPcore::lowChannelCMatrix(my::Matrix<my::RGB> & _matrix, const std::vector<my::RGB> & _colorTable){
	
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
void my::ImageBMPcore::highChannelCMatrix(my::Matrix<my::RGB> &_matrix){

	dword correction = this->correction();
	dword height = this->ImHeight();
	dword width = this->ImWidth();
	dword d;
	if(this->imageHeight < 0) d = 0;
	else 					  d = this->ImHeight() - 1;
	
	fseek(this->image, this->imageOffset, SEEK_SET);
	byte red, green, blue;
	for(dword i = 0; i < height; ++i){
		for(dword j = 0; j < width; ++j){
			if(this->bitCount == 16){
				word colorBitArray = Functional::convert(this->read(2), (word)2);
				red = Functional::value5Bit(colorBitArray, 2);
				green = Functional::value5Bit(colorBitArray, 1);
				blue = Functional::value5Bit(colorBitArray, 0);
			}
			else if(this->bitCount == 24){
				blue = Functional::convert(this->read(1), (byte)2);
				green = Functional::convert(this->read(1), (byte)2);
				red = Functional::convert(this->read(1), (byte)2);
			}
			else if(this->bitCount == 48){
				blue = Functional::convert(this->read(2), (word)2) >> 8;
				green = Functional::convert(this->read(2), (word)2) >> 8;
				red = Functional::convert(this->read(2), (word)2) >> 8;
			}
			RGB color(red, green, blue);
			_matrix[(i-d) - 2*(i-d)*(d != 0)][j] = color;
		}
		fseek(this->image, correction, SEEK_CUR);
	}
	return;
}
void my::ImageBMPcore::lowChannelIMatrix(my::Matrix<my::byte> &_matrix){

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
			byte * x = this->read(1);
			byte colorBitArray = Functional::convert(x, (byte)2);
			// std::cout << std::hex << (int)colorBitArray << " -- " << ftell(this->image) << " -- " << (int)x[0] << std::endl << std::endl;
			for(signed char h = max_bits; h >= 0 && j < width; --h, ++j){
				_matrix[(i-d) - 2*(i-d)*(d != 0)][j] = bit_value_func(colorBitArray, h);
			}
		}
		fseek(this->image, correction, SEEK_CUR);
	}
	return;
}
my::Matrix<my::RGB> my::ImageBMPcore::ColorMatrix() {
	
	Matrix<RGB> result(this->ImHeight(), this->ImWidth());
	
	if(this->bitCount <= 8){
		std::vector<RGB> table(this->ColorTable());
		this->lowChannelCMatrix(result, table);
	}
	else{
		this->highChannelCMatrix(result);
	}
	return result;
}
my::Matrix<my::byte> my::ImageBMPcore::IndexMatrix(){

	Matrix<byte> result(this->ImHeight(), this->ImWidth());
	if(this->bitCount <= 8)
		this->lowChannelIMatrix(result);
	return result;
}
my::Matrix<bool> my::ImageBMPcore::MonoMatrix() {

	if(this->bitCount != 1) throw std::invalid_argument("Default function is for mono bitmaps.");

	Matrix<byte> imatrix = this->IndexMatrix();
	Matrix<bool> result(this->ImHeight(), this->ImWidth());
	for(dword i = 0; i < this->ImHeight(); ++i) {
		for(dword j = 0; j < this->ImWidth(); ++j) {
			result[i][j] = imatrix[i][j];
		}
	}
	return result;
}
my::Matrix<bool> my::ImageBMPcore::MonoMatrix(const std::vector<bool> _mask){

	Matrix<bool> result(this->ImHeight(), this->ImWidth());
	if(this->bitCount <= 8 && _mask.size() >= (1U << this->bitCount)){
		Matrix<unsigned char> indexes(this->ImHeight(), this->ImWidth());
		this->lowChannelIMatrix(indexes);
		for(unsigned int i = 0; i < this->ImHeight(); ++i)
			for(unsigned int j = 0; j < this->ImWidth(); ++j)
				result[i][j] = _mask[indexes[i][j]];
	}
	return result;
}
my::Matrix<bool> my::ImageBMPcore::MonoMatrix(const my::RGB & _separator, bool _crossedToNull) {

	Matrix<bool> result(this->ImHeight(), this->ImWidth());
	Matrix<RGB> colors(this->ColorMatrix());
	
	for(dword i = 0; i < this->ImHeight(); ++i)
		for(dword j = 0; j < this->ImWidth(); ++j)
			result[i][j] = (colors[i][j] > _separator) || ((colors[i][j] != _separator) && !_crossedToNull);
	return result;
}
my::Matrix<my::byte> my::ImageBMPcore::GreyMatrix() {
	
	Matrix<RGB> colored = this->ColorMatrix();
	Matrix<byte> grey(colored.height(), colored.width());
	
	for(dword i = 0; i < colored.height(); ++i) {
		for(dword j = 0; j < colored.width(); ++j) {
			grey[i][j] = byte(0.299*colored[i][j].R + 0.587*colored[i][j].G + 0.114*colored[i][j].B);
		}
	}
	return grey;
}
my::Matrix<bool> my::ImageBMPcore::MonoMatrix(my::byte _grey_separator, bool _crossedToNull) {
	
	Matrix<RGB> colored = this->ColorMatrix();
	Matrix<bool> mono(colored.height(), colored.width());
	
	for(dword i = 0; i < colored.height(); ++i) {
		for(dword j = 0; j < colored.width(); ++j) {
			byte grey_shade = byte(0.299*colored[i][j].R + 0.587*colored[i][j].G + 0.114*colored[i][j].B);
			mono[i][j] =  ((grey_shade > _grey_separator) || (grey_shade == _grey_separator && !_crossedToNull));
		}
	}
	return mono;
}
bool my::ImageBMPcore::Open(const char * _path) {
	
	if(this->image) fclose(this->image);
	this->image = fopen(_path, "rb");
	if(this->image == NULL) {
		throw std::invalid_argument("Can not open the file.");
	}
	word type = Functional::convert(Functional::read(2, this->image, false), (word)2);
		 if(type == 0x4D42) this->bigEndian = false;
	else if(type == 0x424D) this->bigEndian = true;
	else {
		throw std::invalid_argument("Invalid bmp file.");
	}
	this->load();
	return true;
}
std::vector<my::RGB> my::ImageBMPcore::ColorTable () {
	
	if(this->bitCount > 8) throw std::invalid_argument("Can not get color table of 8bit greater bitmap.");

	fseek(this->image, this->clrTblOffset, SEEK_SET);
	std::vector<RGB> table(this->clrTblSize);
	
	for(dword i = 0; i < table.size(); ++i) {
		byte blue = Functional::convert(this->read(1), (byte)2);
		byte green = Functional::convert(this->read(1), (byte)2);
		byte red = Functional::convert(this->read(1), (byte)2);
		RGB color(red, green, blue);
		table[i] = color;
	}
	
	return table;
}
void my::ImageBMPcore::Compatibility(bool _enable) {
	this->compatibility = _enable;
	return;
}
my::ImageBMPcore::~ImageBMPcore() {
	fclose(this->image);
}

#undef _CRT_SECURE_NO_WARNINGS
