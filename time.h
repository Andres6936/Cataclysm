#ifndef _TIME_H_
#define _TIME_H_

#include <string>

enum Season
{
  SEASON_SPRING,
  SEASON_SUMMER,
  SEASON_AUTUMN,
  SEASON_WINTER
};

Season lookup_season(std::string name);
std::string season_name(Season season);

// TODO: Pull this stuff from a config file?

// Days in a season - 1/4 of this is a moon cycle!
#define DAYS_IN_SEASON 20
// Seconds per turn; changing this will wildly alter gameplay!
#define SECONDS_IN_TURN 6
// The year the game starts in
#define STARTING_YEAR 2086
// Functions for converting minutes, hours, days into # of turns
#define MINUTES(x)  ( ((x) * 60)    / SECONDS_IN_TURN )
#define HOURS(x)    ( ((x) * 3600)  / SECONDS_IN_TURN )
#define DAYS(x)     ( ((x) * 86400) / SECONDS_IN_TURN

class Time
{
public:
  Time();
  ~Time();

  Time(const Time& copy);
  Time(int _second, int _minute, int _hour = 0, int _day = 0,
       Season _season = SEASON_SPRING, int _year = 0);
  Time(int _turn);

  bool operator==(const Time &other) const;
  bool operator!=(const Time &other) const;

  Time& operator+=(const Time &rhs);
  Time& operator+=(const int  &rhs);
  Time& operator-=(const Time &rhs);
  Time& operator-=(const int  &rhs);

  int get_turn() const;
  operator int() const; // Returns get_turn()
  std::string get_text(bool twentyfour = false); // "Spring, Day 4, 2:37 PM"

  void increment(); // turn++, second += SECONDS_IN_TURN
  void standardize(); // Ensure seconds < 60, hours < 24, etc

private:
  int year;
  Season season;
  int day;
  int hour;
  int minute;
  int second; // Should always be a multiple of SECONDS_IN_TURN

  int turn;   // We track turn seperately, so we don't have to calculate it

  void set_date_from_turn();
  void set_turn_from_date();
};

inline Time operator+(Time lhs, const Time& rhs)
{
  lhs += rhs;
  return lhs;
}

inline Time operator-(Time lhs, const Time& rhs)
{
  lhs += rhs;
  return lhs;
}

#endif
