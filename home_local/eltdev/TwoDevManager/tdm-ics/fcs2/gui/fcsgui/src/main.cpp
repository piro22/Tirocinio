/**
 * @file
 * @ingroup fcfgui
 * @copyright ESO - European Southern Observatory
 *
 * @brief FCFGUI main file.
 */

#include <iostream>
#include <fcs/gui/fcsgui/mainwindow.h>
#include "fcf/devmgr/common/config.hpp"

#include <QApplication>
#include <QFile>
#include <QCommandLineParser>

#include <utils/bat/logger.hpp>
#include <rad/helper.hpp>
#include <rad/logger.hpp>

#include <rad/dbAdapterRedis.hpp>
#include <QDebug>

#include <azmq/socket.hpp>
#include <azmq/message.hpp>
#include <boost/asio.hpp>

#include <boost/exception/diagnostic_information.hpp>

int main(int argc, char *argv[]) {
  log4cplus::initialize ();

  rad::LogInitializer();
  RAD_LOG_TO_CONSOLE(true);

  QApplication a(argc, argv);
  QCoreApplication::setApplicationName( QString("FCS GUI") );

  QCommandLineOption u_opt(QStringList() << "u" << "uri", \
                           QCoreApplication::translate("main", "Server URI") \
                           ,QCoreApplication::translate("main", "uri"), "1");

  QCommandLineOption l_opt({"l","loglevel"}, "Specify Loglevel", "ERROR|INFO|DEBUG|TRACE");
  QCommandLineOption s_opt({"s","stylesheet-name"}, "Specify built-in stylesheet", "Default||Combinear|Diffnes|Takezo");
  QCommandLineOption f_opt({"f","stylesheet-file"}, "Specify stylesheet file", "stylesheet");
  QCommandLineOption p_opt({"p","pollinterval"}, "Specify DB polling interval (default 500ms)", "500");
  QCommandLineParser parser;

  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(u_opt);
  parser.addOption(l_opt);
  parser.addOption(s_opt);
  parser.addOption(f_opt);
  parser.addOption(p_opt);

  parser.process(QCoreApplication::arguments());

  const QStringList args = parser.optionNames();
  if (args.size() < 1) {
      fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Must specify an argument.")));
      parser.showHelp();
  }

  if (argc <= 1) {
      std::cout << "ERROR !" << std::endl;
      std::cout << "Please provide the URI for the server -uri <uri>"
                << std::endl;
      return EXIT_FAILURE;
    }

  QString uri = parser.value(u_opt);
  QString loglevel = parser.value(l_opt);
  QString stylesheet = parser.value(s_opt);
  QString stylesheet_file = parser.value(f_opt);
  int polling = parser.value(p_opt).toInt();


  if (!parser.isSet(u_opt)) {
      qDebug("\nURI is a required option.\n");
      parser.showHelp();
  }

  if (parser.isSet(s_opt) && parser.isSet(f_opt)) {
      qDebug("\nOptions stylesheet-name and stylesheet-file cannot be both specified.\n");
      parser.showHelp();
  }

  if (!parser.isSet(p_opt)) {
      polling = 500;
  }

  if (uri.isEmpty()) {
      qDebug("\nThe URI value is required.\n");
      parser.showHelp();
  }

  if (loglevel.isEmpty()) {
     loglevel = "ERROR";
  }

  if (stylesheet.isEmpty()) {
      stylesheet = "Default";
  }

   RAD_LOG_SETLEVEL(loglevel.toStdString());
   QString style;
   QString fileName;
   if (parser.isSet(f_opt)) {
       fileName = stylesheet_file;
     } else {
       fileName = QString(":/styles/%1/%2.qss").arg(stylesheet, stylesheet);
     }

   QFile file(fileName);
   if(!file.open(QFile::ReadOnly)) {
       parser.showHelp();
   } else {
       if (parser.isSet(f_opt)) {
           style = QLatin1String(file.readAll());
         } else {
            style = file.readAll();
         }
   }
   file.close();

  try {
    MainWindow w(nullptr, uri, polling);
    w.setWindowTitle( QCoreApplication::applicationName() );
    a.setStyleSheet( style );
    w.show();
    return a.exec();
  }
  catch (std::exception& e) {
    std::cout << "Exception occurred:"  << e.what();
    return EXIT_FAILURE;
  }
  catch (...) {
    std::cout << "Exception occurred" << boost::current_exception_diagnostic_information();
    return EXIT_FAILURE;
  }

return EXIT_SUCCESS;

}
