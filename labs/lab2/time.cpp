#include <iostream>
#include "time.h"

Time::Time() {
	hour = 0;
	minute = 0;
	second = 0;
}

Time::Time(int h, int m, int s) {
	hour = h;
	minute = m;
	second = s;
}

int Time::getHour() const {
	return hour;
}

int Time::getMinute() const {
	return minute;
}

int Time::getSecond() const {
	return second;
}

void Time::setHour(int h) {
	hour = h;
}

void Time::setMinute(int m) {
	minute = m;
}

void Time::setSecond(int s) {
	second = s;
}

void Time::PrintAMPM() const {
	//determine if am or pm
	bool pm = false;
	if (hour >= 12) {
		pm = true;
	}
	//print hour
	if (hour == 0) {
		std::cout << 12 << ':';
	} else if (pm and hour != 12) {
		std::cout << hour-12 << ':';
	} else {
		std::cout << hour << ':';
	}
	//print minute
	if (minute < 10) {
		std::cout << 0 << minute << ':';
	} else {
		std::cout << minute << ':';
	}
	//print second
	if (second < 10) {
		std::cout << 0 << second << ' ';
	} else {
		std::cout << second << ' ';
	}
	//print am or pm
	if (pm) {
		std::cout << "pm" << std::endl;
	} else {
		std::cout << "am" << std::endl;
	}
}

bool IsEarlierThan(const Time& t1, const Time& t2) {
	if (t1.getHour() < t2.getHour()) {
		return true;
	} else if (t1.getHour() == t2.getHour() and t1.getMinute() < t2.getMinute()) {
		return true;
	} else if (t1.getHour() == t2.getHour() and t1.getMinute() == t2.getMinute() and
		t1.getSecond() < t2.getSecond()) {
		return true;
	}
	return false;
}