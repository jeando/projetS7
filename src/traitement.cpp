#include"traitement.h"

std::vector<double> autocorrelation(std::vector<double> input)
{
	std::vector<double> output;
	double temp;
    for(unsigned int tau = 0; tau<input.size(); tau++)
    {
        temp = 0;
        for(unsigned t = tau; t<input.size(); t++)
        {
            temp+=input[t]*input[t-tau];
        }
        output.push_back(temp);
    }
	return output;
}
std::vector<double> decoupage(std::vector<double> input)
{
	std::vector<double> output;
	return output;
}
std::vector<std::vector<double> > spectrogramme(std::vector<double> input)
{
	std::vector<std::vector<double> > output;
	return output;
}
