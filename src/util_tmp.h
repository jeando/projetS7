#ifndef UTIL_TMP_H
#define UTIL_TMP_H

//#include<vector>
#include<algorithm>

//template<template<class,class> class TT, class T>
//inline T get_max_val(TT<T, std::allocator<T> > vect)
//{
//	return vect[0];
//}


//template<template<typename> typename V,typename U>
//template<template<template<class V> class U> class T>
template<template<class,class> class V, template<class, class> class U,class T>
inline T get_max_val(V<U<T, std::allocator<T> >, std::allocator<U<T, std::allocator<T> > > > vect)
{
	T max(*std::max_element(vect[0].begin(),vect[0].end()));
	T max_tmp;
	for(typename V<U<T, std::allocator<T> >, std::allocator<U<T, std::allocator<T> > > >::iterator it= vect.begin()+1;it!=vect.end();it++)
	{
		max_tmp = *std::max_element(it->begin(),it->end());
		if(max_tmp>max)
			max=max_tmp;
	}
	return max;
}


#endif
