#ifndef FOO_H
#define FOO_H

#endif // FOO_H
#include <QProgressDialog>
#include <QtCore>

class Foo : public QObject {
   Q_OBJECT
   QProcess process;
   QString output;
   QProgressDialog progress;

public:
   Foo() : QObject() {
       progress.setWindowTitle("Installing libraries...");
       progress.setWindowModality(Qt::WindowModal);
       progress.setMaximum(0);
       progress.setMinimum(0);
       progress.setValue(0);
       progress.exec();
   };
   void stopIt(){
       progress.close();
   }

private slots:
   // here we check what was received (called everytime new data is readable)
   void readSubProcess(void) {
      output.append(process.readAllStandardOutput());
      qDebug() << "complete output: " << output;
      output.append(process.readAllStandardError());
      qDebug() << "complete output: " << output;

   };
};
