class Time {
public:
  Time();
  Time(int h, int m, int s);

  // ACCESSORS
  int getHour() const;
  int getMinute() const;
  int getSecond() const;

  // MODIFIERS
  void setHour(int h);
  void setMinute(int m);
  void setSecond(int s);

  // other member functions
  void PrintAMPM() const;

private:
  int hour;
  int minute;
  int second;
};

//non member function
bool IsEarlierThan(const Time& t1, const Time& t2);