#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <tuple>
#include <sstream>
#include <iomanip>


long long pow_10(int exp)
{
	long long res = 1;
	for (int i = 0; i < exp; i++)
	{
		res *= 10;
	}
	return res;
}


std::string double_to_string(double input, long precision)
{
	std::stringstream strs;
	strs << std::setprecision(precision);
	strs << input;
	std::string strg = strs.str();
	strg.erase(strg.find_last_not_of('0') + 1, std::string::npos);
	return strg;
}


std::tuple<std::string, long> find_recurrence(std::string fract_strg)
{
	long div = 2;
	long substr_size = (fract_strg.size() - 2);
	std::string found_substring = fract_strg.substr(2, substr_size);
	long found_pos = 2;

	while (true)
	{
		if (substr_size == 1) break;

		substr_size = (fract_strg.size() - 2) / div;
		std::string substring = fract_strg.substr(fract_strg.size() - substr_size, substr_size);
		long com_pos = fract_strg.size() - 2 * substr_size;

		while (true)
		{
			if (com_pos < 2) break;

			std::string substring_compr = fract_strg.substr(com_pos, substr_size);

			if (substring == substring_compr)
			{
				found_pos = com_pos;
				found_substring = substring;
			}

			if (substring != substring_compr)
			{
				break;
			}

			com_pos -= substr_size;
		}

		div++;
	}

	return std::tuple<std::string, long>(found_substring, found_pos);
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage:\nfraction-to-ration <floating point number>" << std::endl;
		return EXIT_FAILURE;
	}

	double input = atof(argv[1]);

	bool negative = (input < 0) ? true : false;
	input = fabs(input);

	std::string input_strg = double_to_string(input, 10);

	long pos_dot = input_strg.find_first_of('.');
	if (pos_dot < 1)
	{
		if (negative) std::cout << '-';
		std::cout << input_strg << std::endl;
		return EXIT_SUCCESS;
	}

	std::string fract_strg = "0" + input_strg.substr(pos_dot, input_strg.size() - pos_dot);
	std::string integ_strg = input_strg.substr(0, pos_dot);
	std::tuple<std::string, long> recurrence_result = find_recurrence(fract_strg);
	long pos_recurrence = std::get<1>(recurrence_result);
	long length_recurrence = std::get<0>(recurrence_result).size();
	long long numerator = 0;
	long long denominator = 0;

	if ((length_recurrence < fract_strg.size() - 2) && (fract_strg.size() - pos_recurrence > 3))
	{
		std::string modf_input_strg1 = integ_strg + fract_strg.substr(2, pos_recurrence - 2);
		std::string modf_input_strg2 = integ_strg + fract_strg.substr(2, pos_recurrence - 2 + length_recurrence);
		numerator = std::stoll(modf_input_strg2) - std::stoll(modf_input_strg1);
		denominator = pow_10(pos_recurrence - 2 + length_recurrence) - pow_10(pos_recurrence - 2);
	}
	else
	{
		numerator = -std::stoll(integ_strg) + std::stoll(fract_strg.substr(2, fract_strg.size() - 2));
		denominator = pow_10(fract_strg.size() - 2);
	}

	while (true)
	{
		if ((numerator % 2 != 0) || (denominator % 2 != 0)) break;
		else
		{
			numerator /= 2;
			denominator /= 2;
		}
	}

	while (true)
	{
		if ((numerator % 3 != 0) || (denominator % 3 != 0)) break;
		else
		{
			numerator /= 3;
			denominator /= 3;
		}
	}

	while (true)
	{
		if ((numerator % 5 != 0) || (denominator % 5 != 0)) break;
		else
		{
			numerator /= 5;
			denominator /= 5;
		}
	}

	if (negative) std::cout << '-';
	std::cout << numerator << "/" << denominator << std::endl;
	return EXIT_SUCCESS;
}
