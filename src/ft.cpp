#include"ft.h"

#define _USE_MATH_DEFINES
#include<cmath>

std::vector<std::complex<double> > ft(std::vector<std::complex<double> > input, int sgn)
{
    std::vector<std::complex<double> > output;
    std::complex<double> temp;
    for(unsigned int k=0;k<input.size();k++){
        temp=0;
        for(unsigned int t=0;t<input.size();t++){
            temp+=input[t]*std::complex<double>(cos(-2*M_PI*k*t/input.size()),sin(-2*M_PI*k*t/input.size()*sgn));
        }
        output.push_back(sgn>0?temp:temp/std::complex<double>(input.size(),0));
    }
    return output;
}
std::vector<std::complex<double> > ft(std::vector<double> input, int sgn)
{
    std::vector<std::complex<double> > output;
    for(unsigned int k=0;k<input.size();k++){
        output.push_back(std::complex<double>(input[k],0));
    }
    return ft(output,sgn);
}
std::vector<std::complex<double> > fft(std::vector<double> input, int sgn)
{
    std::vector<std::complex<double> > output;
    for(unsigned int k=0;k<input.size();k++){
        output.push_back(std::complex<double>(input[k],0));
    }
    return fft(output,sgn);
}

std::vector<std::complex<double> > fft2(std::vector<std::complex<double> > input, int sgn)
{
    if(input.size()==1)
        return input;

    std::vector<std::complex<double> > output;
    std::complex<double> temp;

    std::vector<std::complex<double> > pair;
    std::vector<std::complex<double> > impair;
    std::vector<std::complex<double> > fftpair;
    std::vector<std::complex<double> > fftimpair;
    for(unsigned int i=0; i<input.size()/2; i++){
        pair.push_back(input[2*i]);
        impair.push_back(input[2*i+1]);
    }
    fftpair=fft2(pair,sgn);
    fftimpair=fft2(impair,sgn);
    for(unsigned int i=0; i<input.size()/2; i++){
        output.push_back(fftpair[i]+std::complex<double>(cos(-2*M_PI*i/input.size()),sin(-2*M_PI*i/input.size()*sgn))*fftimpair[i]);
    }
    for(unsigned int i=0; i<input.size()/2; i++){
        output.push_back(fftpair[i]-std::complex<double>(cos(-2*M_PI*i/input.size()),sin(-2*M_PI*i/input.size()*sgn))*fftimpair[i]);
    }
    return output;
}

std::vector<std::complex<double> > fft(std::vector<std::complex<double> > input, int sgn)
{
    std::vector<std::complex<double> > output = fft2(input, sgn);

    if(sgn<0){
        for(unsigned int i=0; i<input.size(); i++){
            output[i]/=std::complex<double>(input.size(),0);
        }
    }
    return output;
}
