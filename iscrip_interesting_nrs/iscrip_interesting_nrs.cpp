#include "pch.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer/timer.hpp>
#include <string>
#include <stdlib.h>

const int NO_OF_THREADS = 12;
const int MAX_TEST_VALUE = 85;

/*
	Shameless copy paste from stackoverflow: https://stackoverflow.com/a/16325469
*/
int my_atoi(char *p, int len) {
	int k = 0;
	for (int i = 0; i < len; i++) {
		k = (k << 3) + (k << 1) + (*p) - '0';
		p++;
	}
	return k;
}

void printNumber(int input) {
	//boost::timer::auto_cpu_timer t;
	long long checkedNr = 0;
	int sum = 0;
	//printf("[start] input: %d, sum: %d\n", input, sum);
	while (sum != input) {
		checkedNr += input;
		sum = 0;
		//std::string digits = boost::lexical_cast<std::string>(checkedNr);
		//char buffer[65];
		//ltoa(checkednr, buffer, 10);
		//std::string digits = std::string(buffer);
		std::string digits = std::to_string(checkedNr);
		for (char &c : digits) {
			sum += my_atoi(&c, 1);
		}
	}
	//printf("Output for %d: %lld\n", input, checkedNr); //
	std::stringstream msg;
	msg << "Output for " << input << ": " << checkedNr << "\n";
	std::cout << msg.str();
}

int main()
{
	boost::timer::auto_cpu_timer t;

	boost::asio::io_service ioService;
	boost::thread_group tgroup;

	boost::asio::io_service::work work(ioService);

	for (int i = 0; i < NO_OF_THREADS; i++) {
		tgroup.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
	}

	for (int i = 0; i <= MAX_TEST_VALUE; i++) {
		ioService.post(boost::bind(&printNumber, i));
	}

	ioService.stop();

	tgroup.join_all();
	return 0;
}