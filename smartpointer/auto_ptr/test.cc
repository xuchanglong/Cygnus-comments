#include <iostream>
#include <string>
#include "auto_ptr.hpp"

int main()
{
	auto_ptr<std::string> films[5] =
		{
			auto_ptr<std::string>(new std::string("肖申克的救赎")),
			auto_ptr<std::string>(new std::string("教父")),
			auto_ptr<std::string>(new std::string("霸王别姬")),
			auto_ptr<std::string>(new std::string("疯狂动物城")),
			auto_ptr<std::string>(new std::string("阿甘正传"))};

	auto pwin = films[1];

	std::cout << "世界最佳电影候选者如下：" << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		std::cout << *films[i] << "、";
	}
	std::cout << std::endl;
	std::cout << "胜利者是：" << *pwin << std::endl;

	std::cin.get();
	return 0;
}