#include "mainwindow.h"
#include "ui_mainwindow.h"


bool operator>(const Event& e1,const Event& e2)
{
  return e1.GetName()>e2.GetName() || (e1.GetName() == e2.GetName() && e1.GetNumberOfResults() > e2.GetNumberOfResults());
}

bool operator==(const Event& e1, const Event& e2)
{
  return e1.GetName()==e2.GetName() && e1.GetNumberOfResults() == e2.GetNumberOfResults();
}

bool operator<(const Event& e1,const Event& e2)
{
  return e1.GetName()<e2.GetName() || (e1.GetName() == e2.GetName() && e1.GetNumberOfResults() < e2.GetNumberOfResults());
}

bool EnterNumber(const std::string& number)
{
	std::istringstream date_stream(number);
	bool ok = true;
	int new_number;
	ok = ok && (date_stream >> new_number);
	ok = ok && date_stream.eof();

	return ok;
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  srand(seed_number);
  ui->setupUi(this);
  ui->EnterChanceLabel->hide();
  ui->EnterChanceLine->hide();
  ui->EnterNameLabel->hide();
  ui->EnterNameLine->hide();
  ui->ButtonCreate->hide();
  ui->ButtonClear->hide();
  ui->ButtonExit->hide();
  ui->ButtonSimulate->hide();
  ui->Button_Normalise->hide();
  ui->ButtonClearShown_2->hide();
  ui->ButtonExitShown_2->hide();
  ui->EnterNumberOfSimulationLabel->hide();
  ui->EnterNumberOfSimulationLine->hide();
  ui->ButtonDoIt1->hide();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::UpdateEvents()
{
  double sum = 0.0;
  for (int i = 0; i < int(events.size());i++)
    sum+=events[i].GetNumberOfResults();
  for (int i = 0; i < int(events.size());i++)
    {
      double chance = events[i].GetNumberOfResults()/sum;
      int numres = events[i].GetNumberOfResults();
      std::string name = events[i].GetName();
      events[i] = Event(name, numres, chance);
    }
}

void MainWindow::PrintList()const
{
  ui->ListEvents->clear();
  for(int i = 0; i < int(events.size());i++)
    {
      QString event = QString("Name: %1, chance - %2").arg(
          QString::fromStdString(events[i].GetName()),
          QString::fromStdString(std::to_string(events[i].GetChance())));
      ui->ListEvents->addItem(event);
    }
}

void MainWindow::PrintStats()const
{
  ui->ListStatistic->clear();
  for(auto i: stats)
    {
      QString event = QString("Name: %1, number of occurrences - %2").arg(
          QString::fromStdString(i.first.GetName()),
          QString::fromStdString(std::to_string(i.second)));
      ui->ListStatistic->addItem(event);
    }
}

void MainWindow::AddHistory(const std::string& name)const
{
  ui->ListResult->addItem(QString::fromStdString(name));
}

bool MainWindow::DialogMessage(const std::string& quest, const std::string instruction)
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, QString::fromStdString(quest), QString::fromStdString(instruction),
      QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes)
    return true;
  else return false;
}

void MainWindow::Clear()
{
  events.clear();
  stats.clear();
  PrintList();
  PrintStats();
  ui->ListResult->clear();
  number_of_simulation = 0;
  number_no_named = 1;
}

void MainWindow::on_ButtonNewEvent_clicked()
{
  ui->EnterChanceLabel->show();
  ui->EnterChanceLine->show();
  ui->EnterNameLabel->show();
  ui->EnterNameLine->show();
  ui->ButtonCreate->show();
  ui->ButtonClear->show();
  ui->ButtonExit->show();
  ui->ButtonSimulate->show();
  ui->Button_Normalise->show();
  ui->ButtonClearShown->hide();
  ui->ButtonExitShown->hide();
  ui->ButtonSimulateShown->hide();
  ui->ButtonNormalizeShown->hide();
  ui->EnterNumberOfSimulationLabel->hide();
  ui->EnterNumberOfSimulationLine->hide();
  ui->ButtonDoIt1->hide();
  ui->ButtonClearShown_2->hide();
  ui->ButtonExitShown_2->hide();
}

void MainWindow::on_Button_Normalise_clicked()
{
    UpdateEvents();
    PrintList();
}

void MainWindow::on_ButtonSimulate_clicked()
{
  ui->EnterChanceLabel->hide();
  ui->EnterChanceLine->hide();
  ui->EnterNameLabel->hide();
  ui->EnterNameLine->hide();
  ui->ButtonCreate->hide();
  ui->ButtonClear->hide();
  ui->ButtonExit->hide();
  ui->ButtonSimulate->hide();
  ui->Button_Normalise->hide();
  ui->ButtonClearShown->hide();
  ui->ButtonExitShown->hide();
  ui->ButtonSimulateShown->show();
  ui->ButtonNormalizeShown->show();
  ui->EnterNumberOfSimulationLabel->show();
  ui->EnterNumberOfSimulationLine->show();
  ui->ButtonDoIt1->show();
  ui->ButtonClearShown_2->show();
  ui->ButtonExitShown_2->show();
}

void MainWindow::on_ButtonClear_clicked()
{
   Clear();
}

void MainWindow::on_ButtonExit_clicked()
{
  if(DialogMessage("Quit", "If you want to close programm press 'Yes'."))
    qApp->exit();
}

void MainWindow::on_ListEvents_itemDoubleClicked(QListWidgetItem *item)
{
  if(DialogMessage("Delete event.", "If you want to delete current event press 'Yes'. If you press 'No' nothing won't be changed."))
    {
      QString written_event = item->text();
      QStringList parts1 = written_event.split(": ");
      QString rest = parts1[1];
      QStringList parts2 = rest.split(", ");
      QString name = parts2[0];
      for (int i = 0; i < int(events.size());i++)
        {
          QString same_name = QString::fromStdString(events[i].GetName());
          if (name == same_name)
            {
              stats.erase(events[i]);
              events.erase(events.begin()+i);
              break;
            }
        }
      PrintList();
      if (number_of_simulation != 0)
        {
          if(DialogMessage("You deleted event and chanses changed.", "If you want to clear History press 'Yes'. If you press 'No' History won't be changed."))
            {
              ui->ListResult->clear();
              for (int i = 0; i < int(events.size());i++)
                stats[events[i]]=0;
              number_of_simulation = 0;
            }
        }
    }
  PrintStats();
}

void MainWindow::on_ButtonCreate_clicked()
{
  std::string name1 = "New event";
  name1+=std::to_string(number_no_named);
  number_no_named++;
  int numres = 1;
  if (ui->EnterChanceLine->text().size())
    {
      QString ch = ui->EnterChanceLine->text();
      if (EnterNumber(ch.toStdString()))
        numres = ch.toInt();
    }
  if (ui->EnterNameLine->text().size())
    {
      QString name = ui->EnterNameLine->text();
      name1 = name.toStdString();
    }
  Event e(name1,numres,numres);
  stats[e] = 0;
  events.push_back(e);
  PrintList();
  if (number_of_simulation != 0)
    if(DialogMessage("You add event and chanses changed.", "If you want to clear History press 'Yes'. If you press 'No' History won't be changed."))
      {
        ui->ListResult->clear();
        for (int i = 0; i < int(events.size());i++)
          stats[events[i]]=0;
        number_of_simulation = 0;
      }
  PrintStats();
  ui->EnterChanceLine->clear();
  ui->EnterNameLine->clear();
  ui->EnterChanceLabel->hide();
  ui->EnterChanceLine->hide();
  ui->EnterNameLabel->hide();
  ui->EnterNameLine->hide();
  ui->ButtonCreate->hide();
  ui->ButtonClear->hide();
  ui->ButtonExit->hide();
  ui->ButtonSimulate->hide();
  ui->Button_Normalise->hide();
  ui->ButtonClearShown->show();
  ui->ButtonExitShown->show();
  ui->ButtonSimulateShown->show();
  ui->ButtonNormalizeShown->show();
}

void MainWindow::on_ButtonNormalizeShown_clicked()
{
  UpdateEvents();
  PrintList();
}

void MainWindow::on_ButtonSimulateShown_clicked()
{
  ui->EnterChanceLabel->hide();
  ui->EnterChanceLine->hide();
  ui->EnterNameLabel->hide();
  ui->EnterNameLine->hide();
  ui->ButtonCreate->hide();
  ui->ButtonClear->hide();
  ui->ButtonExit->hide();
  ui->ButtonSimulate->hide();
  ui->Button_Normalise->hide();
  ui->ButtonClearShown->hide();
  ui->ButtonExitShown->hide();
  ui->ButtonSimulateShown->show();
  ui->ButtonNormalizeShown->show();
  ui->EnterNumberOfSimulationLabel->show();
  ui->EnterNumberOfSimulationLine->show();
  ui->ButtonDoIt1->show();
  ui->ButtonClearShown_2->show();
  ui->ButtonExitShown_2->show();
}

void MainWindow::on_ButtonClearShown_clicked()
{
  Clear();
}

void MainWindow::on_ButtonExitShown_clicked()
{
  if(DialogMessage("Quit", "If you want to close programm press 'Yes'."))
    qApp->exit();
}

void MainWindow::on_ButtonClearShown_2_clicked()
{
  Clear();
}

void MainWindow::on_ButtonExitShown_2_clicked()
{
  if(DialogMessage("Quit", "If you want to close programm press 'Yes'."))
    qApp->exit();
}

void MainWindow::on_ButtonDoIt1_clicked()
{        
  if(ui->EnterNumberOfSimulationLine->text().size())
    {
      if(events.size())
        {
          QString number = ui->EnterNumberOfSimulationLine->text();
          if (EnterNumber(number.toStdString()))
          {
            int numsim = number.toInt();
            for (int i = 0; i < numsim;i++)
              {
                number_of_simulation++;
                int summ = 0;
                for (int i = 0; i < int(events.size());i++)
                  summ+=events[i].GetNumberOfResults();
                int new_event_generated = rand()%summ;
                int numres= 0;
                for (int i = 0; i < int(events.size()); i++)
                  {
                    numres += events[i].GetNumberOfResults();
                    if(numres > new_event_generated)
                      {
                        AddHistory(events[i].GetName());
                        stats[events[i]]++;
                        break;
                      }
                  }
              }
          }
        }
    }
  else
    {
      if(events.size())
        {
          number_of_simulation++;
          int summ = 0;
          for (int i = 0; i < int(events.size());i++)
            summ+=events[i].GetNumberOfResults();
          int new_event_generated = rand()%summ;
          int numres= 0;
          for (int i = 0; i < int(events.size()); i++)
            {
              numres += events[i].GetNumberOfResults();
              if(numres > new_event_generated)
                {
                  AddHistory(events[i].GetName());
                  stats[events[i]]++;
                  break;
                }
            }
        }
    }
  PrintStats();
  ui->EnterNumberOfSimulationLine->clear();
  ui->EnterNumberOfSimulationLabel->hide();
  ui->EnterNumberOfSimulationLine->hide();
  ui->ButtonDoIt1->hide();
  ui->ButtonClearShown->show();
  ui->ButtonExitShown->show();
  ui->ButtonClearShown_2->hide();
  ui->ButtonExitShown_2->hide();
}


void MainWindow::on_EnterSeed_returnPressed()
{
    if(ui->EnterSeed->text().size())
      if(EnterNumber(ui->EnterSeed->text().toStdString()))
        srand(ui->EnterSeed->text().toInt());
}
