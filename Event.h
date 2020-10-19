#ifndef EVENT_H
#define EVENT_H

#endif // EVENT_H
#include <QMainWindow>
#include <QString>

class Event
{
private:
  std::string name;
  int numres;
  double chance;

public:
  Event(const int& number)
  {
    name = "New name " + std::to_string(number);
    numres = number;
    chance = 1.0*number;
  }

  Event(const std::string& NewName, const int NewNumberOfResults, const double& NewChance)
  {
    name = NewName;
    numres = NewNumberOfResults;
    chance = NewChance;
  }

  std::string GetName() const
  {
    return name;
  }

  int GetNumberOfResults()const
  {
    return numres;
  }

  double GetChance() const
  {
    return chance;
  }
};

