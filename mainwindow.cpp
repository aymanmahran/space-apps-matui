#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>

QString symbol[116] = {"Li", "Be", "Na", "Mg", "Al", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Ti", "Pb", "Bi", "Po", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->elements->setColumnCount(2);
    ui->elements->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->elements->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->elements->hideColumn(0);
    ui->elements->setColumnWidth(1, ui->elements->size().width());

    ui->composition->setColumnCount(5);
    ui->composition->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->composition->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->composition->hideColumn(0);
    ui->composition->setColumnWidth(1, ui->elements->size().width()/7);
    ui->composition->setColumnWidth(2, ui->elements->size().width()*2/7);
    ui->composition->setColumnWidth(3, ui->elements->size().width()*2/7);
    ui->composition->setColumnWidth(4, ui->elements->size().width()*2/7);


    //ui->elements->hideRow(1);
    for(int i = 0; i<116; i++){
         ui->elements->insertRow(i);
         ui->elements->setItem(i,1, new QTableWidgetItem(symbol[i]));

    }

    connect(ui->add, &QPushButton::released, this, &MainWindow::addElement);
    connect(ui->remove, &QPushButton::released, this, &MainWindow::removeElement);
    connect(ui->start, &QPushButton::released, this, &MainWindow::startSimulation);

}

void MainWindow::addElement()
{
  if(ui->elements->selectedItems()[0]){
     ui->composition->insertRow(ui->composition->rowCount());
     ui->composition->setItem(ui->composition->rowCount()-1, 1, new QTableWidgetItem(ui->elements->selectedItems()[0]->text()));
     ui->composition->setItem(ui->composition->rowCount()-1, 2, new QTableWidgetItem(ui->x->toPlainText()));
     ui->composition->setItem(ui->composition->rowCount()-1, 3, new QTableWidgetItem(ui->y->toPlainText()));
     ui->composition->setItem(ui->composition->rowCount()-1, 4, new QTableWidgetItem(ui->z->toPlainText()));
  }
}

void MainWindow::removeElement()
{
  if(ui->composition->selectedItems()[0]){
     qDebug() << ui->composition->selectedItems()[0]->row();
     ui->composition->removeRow(ui->composition->selectedItems()[0]->row());
  }
}

void MainWindow::startSimulation()
{
    QFile file("lat.in");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         QTextStream out(&file);
         out << ui->pa->toPlainText()+ " " + ui->pb->toPlainText() + " " + ui->pc->toPlainText() + " ";
         out << ui->aa->toPlainText()+ " " + ui->ab->toPlainText() + " " + ui->ac->toPlainText() + "\n";
         out << ui->ua->toPlainText()+ " " + ui->ub->toPlainText() + " " + ui->uc->toPlainText() + "\n";
         out << ui->va->toPlainText()+ " " + ui->vb->toPlainText() + " " + ui->vc->toPlainText() + "\n";
         out << ui->wa->toPlainText()+ " " + ui->wb->toPlainText() + " " + ui->wc->toPlainText() + "\n";

         QList<QList<QString>> compose;
         compose.empty();
         for(int i=0; i<ui->composition->rowCount();i++){
             QString e = ui->composition->model()->index(i, 1).data().toString();
             QString a = ui->composition->model()->index(i, 2).data().toString();
             QString b = ui->composition->model()->index(i, 3).data().toString();
             QString c = ui->composition->model()->index(i, 4).data().toString();
             QString all = a + " " + b + " " + c;
             QList<QString> some;
             some.push_back(all);
             some.push_back(e);
             compose.push_back(some);
         }
         qDebug() << compose;
         while(compose.size()!=0){
             QString key = compose.at(0).at(0);
             QString lis = compose.at(0).at(1);
             for(int j=1; j<compose.size(); j++){
                 if(compose.at(j).at(0) == key){
                     lis += ", ";
                     lis += compose.at(j).at(1);
                     compose.removeAt(j);
                     j--;
                     qDebug() << compose;
                    qDebug()<<j;
                 }
             }
             out << key + " " + lis + "\n";
             compose.removeAt(0);
         }
    }
    file.close();

    QProcess process;
    process.start("maps", QStringList() << "-c" << "-d &");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    process.waitForFinished();
    qDebug() << output;
    QString err = process.readAllStandardError();
    qDebug() << err;
}

MainWindow::~MainWindow()
{
    delete ui;
}

