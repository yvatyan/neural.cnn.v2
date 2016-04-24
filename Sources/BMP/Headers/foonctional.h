#ifndef _FOONCTIONAL_H_
#define _FOONCTIONAL_H_

#include <iostream>
#include <vector>

namespace my{

	typedef unsigned char byte;
	typedef unsigned short word;
	typedef unsigned int dword;
	typedef unsigned long long qword;
	
	namespace BITMAP{
		enum TYPE{
			TYPE_NO = 0,
			TYPE_MONO = 1,
			TYPE_2 = 2,
			TYPE_4 = 4,
			TYPE_8 = 8,
			TYPE_16 = 16,
			TYPE_24 = 24,
			TYPE_32 = 32,
			TYPE_48 = 48,
			TYPE_64 = 64
		};
	}

	class RGB{
		public:
			byte R,G,B;
		public:
			RGB(byte _red = 0, byte _green = 0, byte _blue = 0) :
				R(_red), G(_green), B(_blue){};
			friend std::ostream& operator<<(std::ostream & _os, const RGB & _clr){
				_os << std::hex
					<< (_clr.R < 0x10 ? "00\b" : "0\b") << (word)_clr.R
					<< (_clr.G < 0x10 ? "00\b" : "0\b") << (word)_clr.G
					<< (_clr.B < 0x10 ? "00\b" : "0\b") << (word)_clr.B;
					_os << std::dec;
				return _os;
			}
			bool operator>(const RGB & _rhs) const {
				return (this->R > _rhs.R) && (this->G > _rhs.G) && (this->B > _rhs.B);
			}
			bool operator<(const RGB & _rhs) const {
				return (this->R < _rhs.R) && (this->G < _rhs.G) && (this->B < _rhs.B);
			}
			bool operator!=(const RGB & _rhs) const {
				return !(this->operator<(_rhs) || this->operator>(_rhs));
			}
	};
	class RGBA : public RGB{
		public:
			byte A;
		public:
			RGBA(byte _red = 0, byte _green = 0, byte _blue = 0, byte _alpha = 0) : RGB(_red, _green, _blue), A(_alpha){};
			RGBA(const RGB & _base_color) : RGB(_base_color), A(0){};
			friend std::ostream& operator<<(std::ostream & _os, const RGBA & _clr){
				_os << std::hex
					<< (_clr.R < 0x10 ? "00\b" : "0\b") << (word)_clr.R
					<< (_clr.G < 0x10 ? "00\b" : "0\b") << (word)_clr.G
					<< (_clr.B < 0x10 ? "00\b" : "0\b") << (word)_clr.B
					<< (_clr.A < 0x10 ? "00\b" : "0\b") << (word)_clr.A;
				_os << std::dec;
				return _os;
			}
			bool operator>(const RGBA & _rhs) const {
				return RGB::operator>(_rhs) && (this->A > _rhs.A);
			}
			bool operator<(const RGBA & _rhs) const{
				return RGB::operator<(_rhs) && (this->A < _rhs.A);
			}
			bool operator!=(const RGBA & _rhs)const {
				return !(this->operator<(_rhs) || this->operator>(_rhs));
			}
	};	
	template<typename T>
	class Matrix : private std::vector< std::vector<T> > {
		public:
			Matrix(int _height = 1, int _width = 1, T _default = T()) : 
				std::vector< std::vector<T> > (_height, std::vector<T> (_width, _default)) {};
			std::vector<T, std::allocator<T> > & operator[](int _index) {
				return std::vector< std::vector<T> >::operator[](_index);
			}
			const std::vector<T, std::allocator<T> > & operator[](int _index) const {
				return std::vector< std::vector<T> >::operator[](_index);
			}
			const int height()const {
				return std::vector< std::vector<T> >::size();
			}
			const int width()const {
				return std::vector< std::vector<T> >::operator[](0).size();
			}
	};
	class Functional{
		private:
			static byte * readFieldBE(byte _count, FILE * _img){
				byte * data = new byte[_count];
				std::cout << "AAA\n";
				for(byte i = 1; i <= _count; ++i){
					fread(&data[_count-i], 1, 1, _img);
					std::cout << (int)i << std::endl;}
				std::cout << "EXIT\n";
				return data;
			}
			static byte * readFieldLE(byte _count, FILE * _img){
			
				byte * data = new byte[_count];
				fread(data, 1, _count, _img);
				// for(byte i = 1; i < _count; ++i){
					// std::cout << (int)data[i-1] << std::endl;
				// }
				return data;
			}	
		public:
			static byte * read(byte _count, FILE * _img, bool _reverse){
				if(_reverse) return Functional::readFieldBE(_count, _img);
				else         return Functional::readFieldLE(_count, _img);				
			}
			static int    convert(const byte * _data, int){
				return Functional::convert(_data, (dword)2);
			}
			static short  convert(const byte * _data, short){
				return Functional::convert(_data, (word)2);
			}
			static dword  convert(const byte * _data, dword){
				dword result = 0;
				for(byte i = 0; i < 4; ++i)
					result = (result <<= 8) | (dword)_data[3-i];
				delete[] _data;
				return result;
			}
			static word   convert(const byte * _data, word){
				word result = 0;
				for(byte i = 0; i < 2; ++i)
					result = (result <<= 8) | (word)_data[1-i];
				//std::cout << std::hex << "l " << _data << " " << result << std::endl;
				delete[] _data;
				return result;
			}
			static byte   convert(const byte * _data, byte){
				//std::cout << std::dec << (int)_data[0] << ' ';
				byte data = _data[0];
				delete[] _data;
				return data;
			}
			static dword  quadDivGE(dword _val){
				return _val & 0x00000003 ? (_val & 0xFFFFFC) + 0x04 : _val;
			}
			static dword  absolute(int _val){
				return _val > 0 ? (dword)_val : (dword)-_val;
			}
			static byte   value1Bit(byte _array, byte _index){
				byte res = 0;
				if(_index < 8){
					_array >>= _index;
					res |= (_array & 1);
				}
				return res;
			}
			static byte   value2Bit(byte _array, byte _index){
				byte res = 0;
				if(_index < 4){
					_array >>= (_index << 1);
					res |= (_array & 3);
				}
				return res;
			}
			static byte   value4Bit(byte _array, byte _index){
				byte res = 0;
				if(_index < 2){
					_array >>= (_index << 2);
					res |= (_array & 15);
				}
				return res;
			}
			static byte   value5Bit(word _array, byte _index){
				word res = 0;
				if(_index < 3){
					_array >>= (_index * 5);
					res |= (_array & 31);
				}
				return (byte)res;
			}
			static byte	  value8Bit(byte _array, byte _index){
				if(_index == 0) return _array;
				return 0;
			}
			static byte   compressionLE(byte * _array, byte _element_size, byte _quantity){
				
				byte result = 0;
				if(_element_size <= 8){
					for(signed char i = _quantity - 1; i >= 0; --i){
						result <<= _element_size;
						result |= _array[i];
					}
				}
				else{
					// throw exception
				}
				return result;
			}
			static byte   compressionBE(byte * _array, byte _element_size, byte _quantity){
				
				byte result = 0;
				if(_element_size <= 8){
					for(signed char i = _quantity - 1; i >= 0; --i){
						result >>= _element_size;
						result |= (_array[i] << (8 - _element_size));
					}
				}
				else{
					// throw exception
				}
				return result;
			}
			static dword  used_bytes(const dword _used_bits){
				return _used_bits & 7 ? (_used_bits >> 3) + 1 : _used_bits >> 3;
			}
		};
}

#endif
