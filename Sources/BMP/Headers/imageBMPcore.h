#ifndef IMAGEBMPCORE_H_
#define IMAGEBMPCORE_H_

#include "foonctional.h"
#include <stdexcept>
#include <climits>

namespace my {
	class ImageBMPcore {
		protected:
			FILE * image;
			dword clrTblOffset;
			//BITMAPHEADER
			bool   bigEndian;
			dword  fileSize;
			dword  imageOffset;
			//BITMAPINFO
			dword  infoSize;
			int    imageWidth;
			int    imageHeight;
			word   ico;
			word   bitCount;
			dword  clrTblSize;
			dword  clrTblUsed;
			bool   compatibility;
			
			byte * read(byte _count) const;
			void writeCommonHeader();
			dword correction() const;
		private:	
			virtual void load();
			virtual void writeCoreBody();
			virtual void writeCoreData(const Matrix<RGB> & _cmatrix);
			virtual void writeCoreData(const Matrix<RGBA> & _cmatrix);
			virtual void writeCoreData(const Matrix<byte> & _imatrix, const std::vector<RGB> &_ctable);
			virtual void writeCoreData(const Matrix<byte> & _imatrix, const std::vector<RGBA> &_ctable);
			virtual dword colorTableOffset();
			void lowChannelCMatrix(Matrix<RGBA> & _matrix, const std::vector<RGBA> & _colorTable) const;
			void highChannelCMatrix(Matrix<RGBA> & _matrix) const;
			void lowChannelCMatrix(Matrix<RGB> & _matrix, const std::vector<RGB> & _colorTable) const;
			void highChannelCMatrix(Matrix<RGB> & _matrix) const;
			void lowChannelIMatrix(Matrix<byte> & _matrix) const;
		public:
			ImageBMPcore();
			virtual void Info() const;
			virtual dword ImageSize() const;
			virtual std::vector<RGB> ColorTable() const;
			virtual std::vector<RGBA> ColorTableA() const;
			
			virtual dword ImWidth() const;
			virtual dword ImHeight() const;
			
			virtual Matrix<RGB> ColorMatrix() const;
			virtual Matrix<RGBA> ColorMatrixA() const;
			virtual Matrix<byte> IndexMatrix();
			virtual Matrix<bool> MonoMatrix();
			virtual Matrix<bool> MonoMatrix(const std::vector<bool> _mask);
			virtual Matrix<bool> MonoMatrix(const RGBA &_separator, bool _crossedToNull);
			virtual Matrix<bool> MonoMatrix(const RGB &_separator, bool _crossedToNull);
			virtual Matrix<bool> MonoMatrix(byte _grey_separator, bool _crossedToNull);
			virtual Matrix<byte> GreyMatrix();
			virtual void Create(const char *_path, const Matrix<RGB> &_cmatrix);
			virtual void Create(const char *_path, const Matrix<RGBA> &_cmatrix);
			virtual void Create(const char *_path, const Matrix<byte> &_imatrix, const std::vector<RGB> &_ctable, BITMAP::TYPE _type);
			virtual void Create(const char *_path, const Matrix<byte> &_imatrix, const std::vector<RGBA> &_ctable, BITMAP::TYPE _type);
			virtual void Create(const char *_path, const Matrix<bool> &_mmatrix);
			virtual void Create(const char *_path, const Matrix<byte> &_gmatrix);
			virtual bool Open(const char *_path);
			virtual void Compatibility(bool _enable);
			virtual ~ImageBMPcore();
			
			void debug() {
				std::cout << image << std::endl;
				std::cout << clrTblOffset << std::endl;
				//BITMAPHEADER
				std::cout <<  bigEndian << std::endl;
				std::cout <<  fileSize << std::endl;
				std::cout <<  imageOffset << std::endl;
				//BITMAPINFO
				std::cout <<  infoSize << std::endl;
				std::cout <<  imageWidth << std::endl;
				std::cout <<  imageHeight << std::endl;
				std::cout <<  ico << std::endl;
				std::cout <<  bitCount << std::endl;
				std::cout <<  clrTblSize << std::endl;
				std::cout <<  clrTblUsed << std::endl;
			}
	};
}

#include "../imageBMPcore.cpp"
#include "../imageBMPcoreA.cpp"


#endif
