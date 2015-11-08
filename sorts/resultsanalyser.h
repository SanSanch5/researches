#ifndef RESULTSANALYSER
#define RESULTSANALYSER

#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <iostream>

enum SortType 
{ 
	MERGE=0, HEAP, QUICK, MEDIAN_QUICK, SMART_QUICK 
}; 

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
  	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<int> analyseFile(std::string fileName)
{
	std::vector<int> nums{-1, -1, -1, -1, -1};
 	std::cout << "File " << fileName; 
	std::ifstream is(fileName);
	if(is.is_open())
	{
		std::cout << " analysing start ... ";
		while(is.good())
		{
			std::string str;
			is >> str;
			std::vector<std::string> sp = split(str, ' ');
			if(sp.size() != 7) continue;

			int n = std::stoi(sp[0]);	
			double insTime = std::stod(sp[1]);
			std::vector<double> times;
			for(unsigned int i = 2; i < sp.size(); ++i)
			{
				double time = std::stod(sp[i]);
				if(nums[i-2] != -1 && time < insTime)
					nums[i-2] = n-10;
			}
		}
		std::cout << "analysing end -> N: ";
		for(int n : nums)
			std::cout << n << " ";
		std::cout << std::endl;
	}
	else
		std::cout << " not found" << std::endl;

	return nums;
}

std::vector<int> analyse()
{
	std::vector<int> res;

	std::vector<int> nums = analyseFile("sortsdata/ins3q_heap");
	std::vector<int> nums2 = analyseFile("sortsdata/minimum_equals");
	std::vector<int> nums3 = analyseFile("sortsdata/much_equals");
	for(int i = 0; i < 5; ++i)
		res.push_back([&]() -> int 
				{
					auto funcMin = [](int a, int b) -> int { return a < b && a != -1 ? a : b; };
			   		int min = funcMin(nums[i], nums2[i]);
					return funcMin(nums3[i], min);	
			   	}());	

	return res;
}

#endif
