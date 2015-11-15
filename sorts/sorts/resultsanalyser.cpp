#include "resultsanalyser.h"

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
//            is >> str;
            std::getline(is, str);
            std::vector<std::string> sp = split(str, ' ');
            if(sp.size() != 7) continue;

            int n = std::stoi(sp[0]);
            double insTime = std::stod(sp[1]);
            for(auto i = 2; i < sp.size(); ++i)
            {
                double time = std::stod(sp[i]);
                if(nums[i-2] == -1 && time < insTime)
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

std::vector<int> findInsertionEqualN()
{
    std::vector<int> res;
    auto nums = analyseFile("../sortsdata_without_insertion/ins3q_heap");
    auto nums2 = analyseFile("../sortsdata_without_insertion/minimum_equals");
    auto nums3 = analyseFile("../sortsdata_without_insertion/much_equals");

    for(int i = 0; i < 5; ++i)
        res.push_back([&]() -> int
                {
                    auto funcMin = [](int a, int b) -> int { return a < b && a != -1 ? a : b; };
                    int min = funcMin(nums[i], nums2[i]);
                    return funcMin(nums3[i], min);
                }());

    return res;
}

struct InsTime
{
    int n;
    double time;

    InsTime(int _n, double _time) : n(_n), time(_time) {}
};

void analyseInsertion(const std::vector<InsTime> &data, int &n)
{
    int k = 0;
    double min = data[k].time;
    if(n != -1) n = data[k].n;
    for(int i = 0; i < data.size(); ++i)
    {
        double t = data[i].time;
        if(t < min)
        {
            min = t;
            n = data[i].n;
        }
    }
}

std::vector<InsTime> getData(std::string fileName, int n)
{
    std::vector<InsTime> data;
    std::cout << "Insertions best " << fileName;
    std::ifstream is(fileName);
    if(is.is_open())
    {
        std::cout << " analysing start ... ";
        while(is.good())
        {
            std::string str;
            std::getline(is, str);
            std::vector<std::string> sp = split(str, ' ');
            if(sp.size() != 2) continue;
            data.push_back(InsTime(std::stoi(sp[0]), std::stod(sp[1])));
        }
    }
    else
        std::cout << " not found" << std::endl;

    return data;
}

std::vector<int> findInsertionBest()
{
    std::vector<int> res{1200, 610, 730, 730, 580};

    std::vector<InsTime> dataMerge = getData("../sortsdata/analyse_n/merge", res[MERGE]);
    std::vector<InsTime> dataMedian = getData("../sortsdata/analyse_n/median", res[MEDIAN_QUICK]);
    std::vector<InsTime> dataQuick = getData("../sortsdata/analyse_n/quick", res[QUICK]);
    std::vector<InsTime> dataSmart = getData("../sortsdata/analyse_n/smart", res[SMART_QUICK]);

    analyseInsertion(dataMerge, res[MERGE]);
    analyseInsertion(dataMedian, res[MEDIAN_QUICK]);
    analyseInsertion(dataQuick, res[QUICK]);
    analyseInsertion(dataSmart, res[SMART_QUICK]);

    return res;
}
