#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Event.h"
#include <sstream>
#include <QListWidgetItem>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_ButtonNewEvent_clicked();

  void on_ButtonSimulate_clicked();

  void on_ButtonClear_clicked();

  void on_ButtonExit_clicked();

  void on_Button_Normalise_clicked();

  void on_ListEvents_itemDoubleClicked(QListWidgetItem *item);

  void on_ButtonCreate_clicked();

  void on_ButtonNormalizeShown_clicked();

  void on_ButtonSimulateShown_clicked();

  void on_ButtonClearShown_clicked();

  void on_ButtonExitShown_clicked();

  void on_ButtonClearShown_2_clicked();

  void on_ButtonExitShown_2_clicked();

  void on_ButtonDoIt1_clicked();

  void on_EnterSeed_returnPressed();

private:
  Ui::MainWindow *ui;
  std::vector<Event> events;
  std::map<Event, int> stats;
  int number_of_simulation = 0;
  int seed_number = time(NULL);
  int number_no_named = 1;

  void UpdateEvents();
  void PrintList()const;
  void PrintStats()const;
  void AddHistory(const std::string& name)const;
  bool DialogMessage(const std::string& quest, const std::string instruction);
  void Clear();
};
#endif // MAINWINDOW_H
