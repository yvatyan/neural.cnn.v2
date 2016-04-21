#ifndef MULTIDDATA_H_
#define MULTIDDATA_H_

#include <vector>

template<typename T>
class matrix1d : private std::vector<T> {
	public:
		matrix1d()
			: std::vector<T>() {};
		matrix1d(size_t _size, T templ = T())
			: std::vector<T>(_size, templ) {};
		matrix1d(const matrix1d& _copy)
			: std::vector<T>(_copy) {};
		
		size_t width() const {
			return std::vector<T>::size();
		}
		T& operator[](int _index) {
			return std::vector<T>::operator[](_index);
		}
		const T& operator[](int _index) const {
			return std::vector<T>::operator[](_index);
		}
};
template<typename T>
class matrix2d : private std::vector< std::vector<T> > {
	public:
		matrix2d()
			: std::vector<std::vector<T> >() {};
		matrix2d(size_t _height, size_t _width, T templ = T())
			: std::vector<std::vector<T> > (_height, std::vector<T>(_width, templ)) {};
		matrix2d(const matrix2d& _copy)
			: std::vector<std::vector<T> > (_copy) {};
		std::vector<T, std::allocator<T> > & operator[](int _index) {
				return std::vector< std::vector<T> >::operator[](_index);
		}
		const std::vector<T, std::allocator<T> > & operator[](int _index) const {
			return std::vector< std::vector<T> >::operator[](_index);
		}
		const int height() const {
			return std::vector< std::vector<T> >::size();
		}
		const int width() const {
			return std::vector< std::vector<T> >::operator[](0).size();
		}
};
template<typename T>
class matrix3d :	private std::vector<std::vector< std::vector<T> > > {
	public:
		matrix3d()
			: std::vector<std::vector<std::vector<T> > >() {};
		matrix3d(size_t _depth, size_t _height, size_t _width, T templ = T())
			: std::vector<std::vector<std::vector<T> > > (_depth, std::vector< std::vector<T> >(_height, std::vector<T>(_width, templ))) {};
		matrix3d(const matrix3d& _copy)
			: std::vector<std::vector<std::vector<T> > > (_copy) {};
		std::vector<std::vector<T, std::allocator<T> > >& operator[](int _index) {
				return std::vector<std::vector< std::vector<T> > >::operator[](_index);
		}
		const std::vector<std::vector<T, std::allocator<T> > >& operator[](int _index) const {
			return std::vector<std::vector< std::vector<T> > >::operator[](_index);
		}
		const int depth() const {
			return std::vector<std::vector< std::vector<T> > >::size();
		}
		const int height() const {
			return std::vector<std::vector< std::vector<T> > >::operator[](0).size();
		}
		const int width() const {
			return std::vector<std::vector< std::vector<T> > >::operator[](0).operator[](0).size();
		}
};

#endif
