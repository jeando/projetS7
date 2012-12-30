#ifndef FT_H_INCLUDED
#define FT_H_INCLUDED

#include<vector>
#include<complex>

std::vector<std::complex<double> > ft(std::vector<std::complex<double> > input, int sgn=1);
std::vector<std::complex<double> > ft(std::vector<double> input, int sgn=1);

std::vector<std::complex<double> > fft(std::vector<std::complex<double> > input, int sgn=1);
std::vector<std::complex<double> > fft(std::vector<double> input, int sgn=1);

#endif // FT_H_INCLUDED
