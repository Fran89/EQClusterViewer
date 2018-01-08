#ifndef SWARMVIEWER_H
#define SWARMVIEWER_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStandardItemModel>
#include <QTableView>
#include <QGeoCoordinate>
#include <QDateTime>
#include <QMap>
#include <QSplitter>
#include <QFileInfo>
#include <QTextDocument>
#include <qcustomplot/qcustomplot.h>
#include <cluster.h>
#include "classify.h"
/*!
 * \mainpage
 * \section Welcome Welcome to the Swarm Viewer documentation
 * \list
 * \li Catalog <- Class handles the main catalog.
 * \li Swarm <- Class handles individual swarms.
 * \li \ref eevent <- Struct handles individual events.
 * \li SwarmViewer <- Class handles the main GUI, catalog, and swarm list.
 * \endlist
 */

/*!4.8
 * \namespace Ui The default namespace.
 */
namespace Ui {
class EQClusterViewer;
}

/*!
 * \brief The SwarmViewer class is the main class of the swarm viewer. It contains the GUI and most interactions
 * \class SwarmViewer
 */
class EQClusterViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit EQClusterViewer(QWidget *parent = 0);
    ~EQClusterViewer();
    enum clustype { swarms, sequences, all };
    Q_ENUM(clustype)

private slots:
    void on_actionQuit_triggered();

    void on_actionLoad_Combined_Catalog_triggered();

    void on_actionLoad_File_triggered();

    void on_graph_clicked();

    void on_savecurpdf_clicked();

    void on_saveallpdf_clicked();

    void on_genStat_clicked();

    void on_class_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_actionSave_sequences_triggered();

    void on_actionSave_statistics_triggered();

    void on_actionSave_all_Classifications_triggered();

    void on_actionSave_swarms_triggered();

    void on_classall_clicked();

private:
    Ui::EQClusterViewer *ui;
    enum FileT { csv_regsw, csv_comcat };
    QString RegionName;
    void checkString(QString &temp, QChar character = 0, bool First = false);
    void toNewModel(FileT ft);
    void plotCluster(int SwID);
    void genStats(QString filedir, bool pdf);
    void genSortFile(QString filedir, clustype type);
    QList<QStringList> csv;
    QStandardItemModel *model;
    QStandardItemModel *swmodel;
    QStandardItemModel *ccmodel;
    QList<QStandardItem*> standardItemList;
    QTableView *table;
    QList<Cluster> Clusters;
    Catalog      myCatalog;
    QCustomPlot *graph;
};

#endif // SWARMVIEWER_H
