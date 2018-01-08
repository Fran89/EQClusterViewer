#include "EQClusterViewer.h"
#include "ui_EQClusterViewer.h"

EQClusterViewer::EQClusterViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EQClusterViewer)
{
    ui->setupUi(this);
    table = ui->tableView;
    graph = ui->graphView;
    ui->actionLoad_Combined_Catalog->setDisabled(true);
    ui->graph->setDisabled(true);
    ui->graphall->setDisabled(true);
    ui->class_2->setDisabled(true);
    ui->savecurpdf->setDisabled(true);
    ui->saveallpdf->setDisabled(true);
    ui->genStat->setDisabled(true);
    ui->actionSave_statistics->setDisabled(true);
    ui->actionSave_all_Classifications->setDisabled(true);
    ui->actionSave_sequences->setDisabled(true);
    ui->actionSave_swarms->setDisabled(true);
    ui->classall->setDisabled(true);
    ui->mainToolBar->hide();
}

EQClusterViewer::~EQClusterViewer()
{
    delete ui;
}

void EQClusterViewer::on_actionQuit_triggered()
{
    QApplication::quit();
}

void EQClusterViewer::toNewModel(FileT ft){

    QStandardItemModel * model2 = new QStandardItemModel;

    QList<QStandardItem*> tempL;
    QStandardItem* temp;
    QDate tempd;
    QTime tempt;
    QDateTime tempdt;
    QGeoCoordinate tempgeo;
    int tempint;
    double tempdouble;

    if(ft == csv_regsw){
        model2 = new QStandardItemModel(0,2);
        model2->setHorizontalHeaderItem(0,new QStandardItem("Date Time"));
        model2->setHorizontalHeaderItem(1,new QStandardItem("Coordinate"));
        model2->setHorizontalHeaderItem(2,new QStandardItem("Magnitude"));
        model2->setHorizontalHeaderItem(3,new QStandardItem("Cluster ID"));
        for (int i=0; i < model->rowCount(); i++) {
            eevent etemp;
            tempL.clear();
            for(int j=0; j < model->columnCount(); j++){
                temp = new QStandardItem;
                temp = model->takeItem(i,j);
                switch(j){
                case 3:
                    tempd = QDate::fromString(temp->text().simplified(), "yyyy/MM/dd");
                    break;
                case 4:
                    tempt = QTime::fromString(temp->text().simplified(),"hh:mm:ss:zzz");
                    break;
                case 0:
                    tempgeo.setLatitude(temp->text().toDouble());
                    break;
                case 1:
                    tempgeo.setLongitude(temp->text().toDouble());
                    break;
                case 2:
                    tempgeo.setAltitude(temp->text().toDouble());
                    break;
                case 5:
                    tempdouble = temp->text().toDouble();
                    break;
                case 9:
                    tempint = temp->text().toInt();
                    break;
                }
                delete(temp);
            }
            tempdt.setDate(tempd);
            tempdt.setTime(tempt);
            etemp.time = tempdt;
            etemp.time_in_usec = tempdt.toTime_t();
            etemp.position = tempgeo;
            etemp.latitude = tempgeo.latitude();
            etemp.longitude = tempgeo.longitude();
            etemp.depth = tempgeo.altitude()/1000;
            etemp.magnitude = tempdouble;
            Cluster tempS;
            tempS.setClusterID(tempint);
            if(Clusters.contains(tempS)){
                int index = Clusters.indexOf(tempS);
                Clusters[index].addEvent(etemp);
            }else{
                tempS << etemp;
                Clusters.append(tempS);
            }

            tempL.append(new QStandardItem(tempdt.toString()));
            tempL.append(new QStandardItem(tempgeo.toString()));
            tempL.append(new QStandardItem(QString::number(tempdouble)));
            tempL.append(new QStandardItem(QString::number(tempint)));
            model2->appendRow(tempL);
        }
    }
    if(ft == csv_comcat){
        model2 = new QStandardItemModel(0,2);
        model2->setHorizontalHeaderItem(0,new QStandardItem("Date Time"));
        model2->setHorizontalHeaderItem(1,new QStandardItem("Coordinate"));
        model2->setHorizontalHeaderItem(2,new QStandardItem("Magnitude"));
        double year,month,day,hour,min,sec,msec;
        for (int i=0; i < model->rowCount(); i++) {
            eevent etemp;
            tempL.clear();
            for(int j=0; j < model->columnCount(); j++){
                temp = new QStandardItem;
                temp = model->takeItem(i,j);
                switch(j){
                case 0:
                    tempgeo.setLongitude(temp->text().toDouble());
                    break;
                case 1:
                    tempgeo.setLatitude(temp->text().toDouble());
                    break;
                case 2:
                    year = floor(temp->text().toDouble());
                    break;
                case 3:
                    month = temp->text().toInt();
                    break;
                case 4:
                    day = temp->text().toInt();
                    break;
                case 5:
                    tempdouble = temp->text().toDouble();
                    break;
                case 6:
                    tempgeo.setAltitude(temp->text().toDouble()*-1000);
                    break;
                case 7:
                    hour = temp->text().toInt();
                    break;
                case 8:
                    min = temp->text().toInt();
                    break;
                case 9:
                    msec = modf(temp->text().toDouble(),&sec);
                    break;
                }
                delete temp;
            }
            tempd.setDate(year,month,day);
            tempt.setHMS(hour,min,sec,msec);
            tempdt.setDate(tempd);
            tempdt.setTime(tempt);
            etemp.time = tempdt;
            etemp.time_in_usec = tempdt.currentMSecsSinceEpoch();
            etemp.position = tempgeo;
            etemp.latitude = tempgeo.latitude();
            etemp.longitude = tempgeo.longitude();
            etemp.depth = tempgeo.altitude()/1000;
            etemp.magnitude = tempdouble;
            myCatalog.addEvent(etemp);

            tempL.append(new QStandardItem(tempdt.toString()));
            tempL.append(new QStandardItem(tempgeo.toString()));
            tempL.append(new QStandardItem(QString::number(tempdouble)));
            model2->appendRow(tempL);
        }
    }

    delete(model);

    if(ft == csv_regsw){
        swmodel = model2;
        table->setModel(swmodel);
        table->resizeColumnsToContents();
        for(int i=0; i<Clusters.size(); i++)
            ui->comboBox->addItem(QString::number(Clusters.at(i).getID()),i);
        ui->comboBox->addItem("All",-1);
    }
    if(ft == csv_comcat){
        ccmodel = model2;
        table->setModel(ccmodel);
        table->resizeColumnsToContents();
    }
}

void EQClusterViewer::checkString(QString &temp, QChar character, bool First)
{
    if(temp.count("\"")%2 == 0) {
        //if (temp.size() == 0 && character != ',') //problem with line endings
        //    return;
        if (temp.startsWith( QChar('\"')) && temp.endsWith( QChar('\"') ) ) {
             temp.remove( QRegExp("^\"") );
             temp.remove( QRegExp("\"$") );
        }
        //FIXME: will possibly fail if there are 4 or more reapeating double quotes
        temp.replace("\"\"", "\"");
        QStandardItem *item = new QStandardItem(temp);
        standardItemList.append(item);
        if (character != QChar(',') && First) {
            model->appendRow(standardItemList);
            standardItemList.clear();
        }
        if (character != QChar(',') && !First) {
            model->appendRow(standardItemList);
            standardItemList.clear();
        }
        temp.clear();
    } else {
        temp.append(character);
    }
}

void EQClusterViewer::on_actionLoad_Combined_Catalog_triggered()
{
    model = new QStandardItemModel(this);
    QString filename;
    QFileDialog getfilename(this);
    getfilename.setFileMode(QFileDialog::AnyFile);
    getfilename.setAcceptMode(QFileDialog::AcceptSave);
    getfilename.setWindowTitle("Open CSV file");
    getfilename.setLabelText(QFileDialog::Accept,"Open CSV");
    getfilename.setNameFilter("CSV Files (*.csv)");
    getfilename.setDirectory(QDir::currentPath());
    getfilename.setOption(QFileDialog::DontConfirmOverwrite);

    ui->graphall->setEnabled(true);
    ui->graphall->setChecked(true);

    if(getfilename.exec()){
        filename = getfilename.selectedFiles().first();
        QFileInfo check_file(filename);
        if (!check_file.exists())
            return;
    } else
        return;

    bool First = true;
    QFile file (filename);
    if (file.open(QIODevice::ReadOnly)) {
        QString data = file.readAll();
        data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
        QString temp;
        QChar character;
        QTextStream textStream(&data);
        while (!textStream.atEnd()) {
            textStream >> character;
            if (character == ',') {
                checkString(temp, character);
            } else if (character == '\n') {
                checkString(temp, character, First);
                First = false;
            } else if (textStream.atEnd()) {
                temp.append(character);
                checkString(temp);
            } else {
                temp.append(character);
            }
        }
    }
    toNewModel(csv_comcat);
}

void EQClusterViewer::on_actionLoad_File_triggered()
{
    bool ok = false;
    RegionName = QInputDialog::getText(this, "Set Region Name",
                                       "Region:", QLineEdit::Normal,
                                       "PR", &ok);
    if (!ok || RegionName.isEmpty())
        return ;

    model = new QStandardItemModel(this);
    QString filename;
    QFileDialog getfilename(this);
    getfilename.setFileMode(QFileDialog::AnyFile);
    getfilename.setAcceptMode(QFileDialog::AcceptSave);
    getfilename.setWindowTitle("Open CSV file");
    getfilename.setLabelText(QFileDialog::Accept,"Open CSV");
    getfilename.setNameFilter("CSV Files (*.csv)");
    getfilename.setDirectory(QDir::currentPath());
    getfilename.setOption(QFileDialog::DontConfirmOverwrite);

    if(getfilename.exec()){
        filename = getfilename.selectedFiles().first();
        QFileInfo check_file(filename);
        if (!check_file.exists())
            return;
    }else
        return;

    ui->actionLoad_Combined_Catalog->setEnabled(true);
    ui->graph->setEnabled(true);
    ui->genStat->setEnabled(true);
    ui->class_2->setEnabled(true);
    ui->actionSave_statistics->setEnabled(true);
    ui->actionSave_all_Classifications->setEnabled(true);
    ui->actionSave_sequences->setEnabled(true);
    ui->actionSave_swarms->setEnabled(true);
    ui->classall->setEnabled(true);

    bool First = true;
    QFile file (filename);
    if (file.open(QIODevice::ReadOnly)) {
        QString data = file.readAll();
        data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
        QString temp;
        QChar character;
        QTextStream textStream(&data);
        while (!textStream.atEnd()) {
            textStream >> character;
            if (character == ',') {
                checkString(temp, character);
            } else if (character == '\n') {
                checkString(temp, character, First);
                First = false;
            } else if (textStream.atEnd()) {
                temp.append(character);
                checkString(temp);
            } else {
                temp.append(character);
            }
        }
    }
    toNewModel(csv_regsw);
}

void EQClusterViewer::on_graph_clicked()
{
    ui->saveallpdf->setEnabled(true);
    ui->savecurpdf->setEnabled(true);
    plotCluster(ui->comboBox->currentData().toInt());
}

void EQClusterViewer::on_savecurpdf_clicked()
{
    QString filename;
    QFileDialog savefiledia(this);
    savefiledia.setFileMode(QFileDialog::AnyFile);
    savefiledia.setAcceptMode(QFileDialog::AcceptSave);
    savefiledia.setWindowTitle("Save PDF File");
    savefiledia.setLabelText(QFileDialog::Accept,"Save PDF");
    savefiledia.setNameFilter("PDF Files (*.pdf)");
    savefiledia.setDirectory(QDir::currentPath());

    if(savefiledia.exec()){
        filename = savefiledia.selectedFiles().first();
    }else
        return;

    graph->savePdf(filename,595,842,QCP::epAllowCosmetic,"EQClusterViewer","EQCluster");
}

void EQClusterViewer::on_saveallpdf_clicked()
{
    QFileDialog savefiledia(this);
    QString filedir;
    savefiledia.setDirectory(QDir::currentPath());
    savefiledia.setOption(QFileDialog::ShowDirsOnly);
    savefiledia.setFileMode(QFileDialog::Directory);
    savefiledia.setWindowTitle("Select PDF save path");
    savefiledia.show();
    if(savefiledia.exec()){
        filedir = savefiledia.selectedFiles().at(0);
        filedir.append('/');
    }else{
        return;
    }
    for(int i=0; i < Clusters.size(); i++){
        QString filenamepdf = filedir;
        filenamepdf.append(Clusters[i].getMinTime().toString("yyyy.MM.dd.hh.mm.s"));
        QString filenamepng = filenamepdf;
        filenamepdf.append("_cluster.pdf");
        filenamepng.append("_cluster.png");
        plotCluster(i);
        graph->savePng(filenamepng,700,842,3.0,-1,300);
        graph->savePdf(filenamepdf,595,842,QCP::epAllowCosmetic,"EQClusterViewer","EQCluster");
    }

    genStats(filedir, true);
    genStats(filedir, false);
    genSortFile(filedir, swarms);
    genSortFile(filedir, sequences);
    genSortFile(filedir, all);

}

void EQClusterViewer::on_genStat_clicked()
{
    QString filedir;

    QFileDialog savefilestat(this);
    savefilestat.setDirectory(QDir::currentPath());
    savefilestat.setOption(QFileDialog::ShowDirsOnly);
    savefilestat.setFileMode(QFileDialog::Directory);
    savefilestat.setWindowTitle("Select PDF save path");
    savefilestat.show();
    if(savefilestat.exec()){
        filedir = savefilestat.selectedFiles().at(0);
        filedir.append('/');
    }else{
        return;
    }
    genStats(filedir, true);
}

void EQClusterViewer::plotCluster(int SwID)
{
    // Clear Graphs and Items
    graph->clearGraphs();
    graph->plotLayout()->clear();
    graph->clearItems();

    // Point CurrentCluster to the currently selected swarm
    Cluster *CurrentCluster = &Clusters[SwID];

    // Get swarm extents
    QGeoCoordinate TL(CurrentCluster->getMax(Lat),CurrentCluster->getMin(Lon));
    QGeoCoordinate BR(CurrentCluster->getMin(Lat),CurrentCluster->getMax(Lon));
    QDateTime Begin = CurrentCluster->getMinTime().addSecs(-28800);
    QDateTime End = CurrentCluster->getMaxTime().addSecs(28800);

    Catalog SubCat;
    // Get SubCatalog for this Extents
    if (ui->graphall->isChecked()){
        SubCat = myCatalog.getSubCat(TL,BR, Begin, End);
    }
    // Make Sublayouts
    QCPLayoutGrid *subLayout1 = new QCPLayoutGrid;
    QCPLayoutGrid *subLayout2 = new QCPLayoutGrid;
    QCPLayoutGrid *subLayout3 = new QCPLayoutGrid;

    // Make Title, decide color:
    QCPTextElement* Title;
    QPen Pen;
    if (CurrentCluster->isclassified()){
        Title = new QCPTextElement(graph,
                                   CurrentCluster->getMinTime().toString("yyyy/MM/dd hh:mm:s")
                                   + QString(" Cluster in the %1 Region (ID = %2, %3, %4)")
                                   .arg(RegionName)
                                   .arg(CurrentCluster->getID())
                                   .arg(CurrentCluster->getClass(false))
                                   .arg(CurrentCluster->getTrend(false)));
        Pen = QPen(QColor(Qt::blue));
    } else {
        Title = new QCPTextElement(graph,
                                   CurrentCluster->getMinTime().toString("yyyy/MM/dd hh:mm:s")
                                   + QString(" Cluster in the %1 Region (ID = %2)")
                                   .arg(RegionName)
                                   .arg(CurrentCluster->getID()));
        Pen = QPen(QColor(Qt::green));
    }
    // Add to the Graph
    graph->plotLayout()->insertRow(0);
    graph->plotLayout()->addElement(0,0, Title);
    graph->plotLayout()->addElement(1,0,subLayout1);
    graph->plotLayout()->addElement(2,0,subLayout2);
    graph->plotLayout()->addElement(3,0,subLayout3);

    // Make Graphs
    QCPAxisRect *MagTime = new QCPAxisRect(graph, false); // false means to not setup default axes
    QCPAxisRect *DepTime = new QCPAxisRect(graph, false);
    QCPAxisRect *LatTime = new QCPAxisRect(graph, false);
    QCPAxisRect *LonTime = new QCPAxisRect(graph, false);
    QCPAxisRect *LatDep  = new QCPAxisRect(graph);
    QCPAxisRect *LonDep  = new QCPAxisRect(graph);
    QCPAxisRect *MagHist  = new QCPAxisRect(graph, false);
    QCPAxisRect *DepHist  = new QCPAxisRect(graph, false);

    // Add to sublayouts
    subLayout1->addElement(0,0,new QCPTextElement(graph,"Time vs Magitude"));
    subLayout1->addElement(0,1,new QCPTextElement(graph,"Time vs Depth"));
    subLayout1->addElement(1, 0, MagTime);
    subLayout1->addElement(1, 1, DepTime);
    //subLayout1->addElement(1, 0, LatTime);
    //subLayout1->addElement(1, 1, LonTime);
    subLayout2->addElement(0,0,new QCPTextElement(graph,"Latitude Profile"));
    subLayout2->addElement(0,1,new QCPTextElement(graph,"Longitude Profile"));
    subLayout2->addElement(1, 0, LatDep);
    subLayout2->addElement(1, 1, LonDep);
    subLayout3->addElement(0,0,new QCPTextElement(graph,"Magnitude Histogram"));
    subLayout3->addElement(0,1,new QCPTextElement(graph,"Depth Histogram"));
    subLayout3->addElement(1, 0, MagHist);
    subLayout3->addElement(1, 1, DepHist);

    // Add Axis
    MagTime->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    DepTime->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    LatTime->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    LonTime->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    MagHist->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    DepHist->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);

    // Range for DateTime
    QCPRange DateRange;
    DateRange.upper = CurrentCluster->getMax(TimeS) + 28800;
    DateRange.lower = CurrentCluster->getMin(TimeS) - 28800;

    // Range for Magnitude
    QCPRange MagRange;
    MagRange.upper = CurrentCluster->getMax(Mag) + 0.3;
    MagRange.lower = CurrentCluster->getMin(Mag) - 0.3;
    if (ui->graphall->isChecked()){
        MagRange.upper = SubCat.getMax(Mag) + 0.3;
        MagRange.lower = SubCat.getMin(Mag) - 0.3;
    }

    // Range for Depth
    QCPRange DepRange;
    DepRange.upper = CurrentCluster->getMin(Depth) - 5;
    DepRange.lower = CurrentCluster->getMax(Depth) + 5;
    if (ui->graphall->isChecked()){
        DepRange.upper = SubCat.getMin(Depth) - 5;
        DepRange.lower = SubCat.getMax(Depth) + 5;
    }

    // Range for Latitude
    QCPRange LatRange;
    LatRange.upper = CurrentCluster->getMax(Lat) + 0.05;
    LatRange.lower = CurrentCluster->getMin(Lat) - 0.05;
    if (ui->graphall->isChecked()){
        LatRange.upper = SubCat.getMax(Lat) + 0.05;
        LatRange.lower = SubCat.getMin(Lat) - 0.05;
    }

    // Range for Longitude
    QCPRange LonRange;
    LonRange.upper = CurrentCluster->getMax(Lon) + 0.05;
    LonRange.lower = CurrentCluster->getMin(Lon) - 0.05;
    if (ui->graphall->isChecked()){
        LonRange.upper = SubCat.getMax(Lon) + 0.05;
        LonRange.lower = SubCat.getMin(Lon) - 0.05;
    }

    // Histogram intervals
    double magint = 0.2;
    double depint = 0.5;
    double deprangesize;
    deprangesize = ((-1*DepRange.upper) - (-1*DepRange.lower));
    if( deprangesize < 5)
        depint = 0.5;
    if (5 <= deprangesize && deprangesize < 10)
        depint = 1;
    if (10 <= deprangesize && deprangesize < 25)
        depint = 2;
    if (25 <= deprangesize && deprangesize < 50)
        depint = 5;
    if (50 <= deprangesize && deprangesize < 100)
        depint = 10;
    if (deprangesize >=100)
        depint = 15;

    // Create DateTime Ticker
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d/M/yy\nhh:mm");
    dateTicker->setTickCount(3);

    // setup a ticker for subBars key axis that only gives 0.2 ticks:
    QSharedPointer<QCPAxisTickerFixed> magTicker(new QCPAxisTickerFixed);
    magTicker->setTickStep(magint);
    magTicker->setTickOrigin(0.1);

    // setup a ticker for subBars key axis that only gives 0.5 ticks:
    QSharedPointer<QCPAxisTickerFixed> depTicker(new QCPAxisTickerFixed);
    depTicker->setTickStep(depint);

    // Set ranges for sublayout1
    MagTime->axis(QCPAxis::atBottom)->setRange(DateRange);
    MagTime->axis(QCPAxis::atLeft)->setRange(MagRange);
    DepTime->axis(QCPAxis::atBottom)->setRange(DateRange);
    DepTime->axis(QCPAxis::atLeft)->setRange(DepRange);
    LatTime->axis(QCPAxis::atBottom)->setRange(DateRange);
    LatTime->axis(QCPAxis::atLeft)->setRange(LatRange);
    LonTime->axis(QCPAxis::atBottom)->setRange(DateRange);
    LonTime->axis(QCPAxis::atLeft)->setRange(LonRange);
    MagTime->axis(QCPAxis::atBottom)->setTicker(dateTicker);
    DepTime->axis(QCPAxis::atBottom)->setTicker(dateTicker);
    LatTime->axis(QCPAxis::atBottom)->setTicker(dateTicker);
    LonTime->axis(QCPAxis::atBottom)->setTicker(dateTicker);

    // Set Labels for sublayout1
    MagTime->axis(QCPAxis::atBottom)->setLabel("Date and Time (d/m/yy)");
    MagTime->axis(QCPAxis::atBottom)->setSubTicks(false);
    MagTime->axis(QCPAxis::atLeft)->setLabel("Magnitude (Coda)");
    DepTime->axis(QCPAxis::atBottom)->setLabel("Date and Time (d/m/yy)");
    DepTime->axis(QCPAxis::atLeft)->setLabel("Depth in Km");
    //LatTime->axis(QCPAxis::atBottom)->setLabel("Date and Time");
    LatTime->axis(QCPAxis::atLeft)->setLabel("Latitude in Dec Deg");
    //LonTime->axis(QCPAxis::atBottom)->setLabel("Date and Time");
    LonTime->axis(QCPAxis::atLeft)->setLabel("Longitude in Dec Deg");

    // Set Ranges for sublayout2
    LonDep->axis(QCPAxis::atLeft)->setRange(DepRange);
    LonDep->axis(QCPAxis::atBottom)->setRange(LonRange);
    LatDep->axis(QCPAxis::atLeft)->setRange(DepRange);
    LatDep->axis(QCPAxis::atBottom)->setRange(LatRange);

    // Set Lables for sublayout2
    LonDep->axis(QCPAxis::atLeft)->setLabel("Depth in Km");
    LonDep->axis(QCPAxis::atBottom)->setLabel("Longitude in Dec Deg");
    LatDep->axis(QCPAxis::atLeft)->setLabel("Depth in Km");
    LatDep->axis(QCPAxis::atBottom)->setLabel("Latitude in Dec Deg");

    // Set Lables for sublayout3
    MagHist->axis(QCPAxis::atLeft)->setLabel("Number of events");
    MagHist->axis(QCPAxis::atBottom)->setLabel(QString("Magnitude (%1M bins)").arg(magint));
    DepHist->axis(QCPAxis::atLeft)->setLabel("Number of events");
    DepHist->axis(QCPAxis::atBottom)->setLabel(QString("Depth in Km (%1Km bins)").arg(depint));

    // synchronize the left and right margins of the sublayout1 (DateTime):
    QCPMarginGroup *marginGroup1 = new QCPMarginGroup(graph);
    QCPMarginGroup *marginGroup2 = new QCPMarginGroup(graph);
    MagTime->setMarginGroup(QCP::msLeft, marginGroup1);
    LatTime->setMarginGroup(QCP::msLeft, marginGroup1);
    DepTime->setMarginGroup(QCP::msLeft, marginGroup2);
    LonTime->setMarginGroup(QCP::msLeft, marginGroup2);
    MagHist->setMarginGroup(QCP::msLeft, marginGroup1);
    DepHist->setMarginGroup(QCP::msLeft, marginGroup2);

    // Are we graphing +- 8 Hrs?
    if (ui->graphall->isChecked()){
        QPen allPen = QPen(QColor(Qt::red));
        // Let's start Plotting sublayout1
        // MagPlot
        QCPGraph *MagTimega = graph->addGraph(MagTime->axis(QCPAxis::atBottom),MagTime->axis(QCPAxis::atLeft));
        MagTimega->setData(SubCat.getSeries(TimeS),
                      SubCat.getSeries(Mag));
        MagTimega->setLineStyle(QCPGraph::lsNone);
        MagTimega->setScatterStyle(QCPScatterStyle::ssDisc);
        MagTimega->setPen(allPen);
        // DepPlot
        QCPGraph *DepTimega = graph->addGraph(DepTime->axis(QCPAxis::atBottom),DepTime->axis(QCPAxis::atLeft));
        DepTimega->setData(SubCat.getSeries(TimeS),
                      SubCat.getSeries(Depth));
        DepTimega->setLineStyle(QCPGraph::lsNone);
        DepTimega->setScatterStyle(QCPScatterStyle::ssDisc);
        DepTimega->setPen(allPen);
        // LatPlot
        QCPGraph *LatTimega = graph->addGraph(LatTime->axis(QCPAxis::atBottom),LatTime->axis(QCPAxis::atLeft));
        LatTimega->setData(SubCat.getSeries(TimeS),
                      SubCat.getSeries(Lat));
        LatTimega->setLineStyle(QCPGraph::lsNone);
        LatTimega->setScatterStyle(QCPScatterStyle::ssDisc);
        LatTimega->setPen(allPen);
        // LonPlot
        QCPGraph *LonTimega = graph->addGraph(LonTime->axis(QCPAxis::atBottom),LonTime->axis(QCPAxis::atLeft));
        LonTimega->setData(SubCat.getSeries(TimeS),
                      SubCat.getSeries(Lon));
        LonTimega->setLineStyle(QCPGraph::lsNone);
        LonTimega->setScatterStyle(QCPScatterStyle::ssDisc);
        LonTimega->setPen(allPen);

        // Let's start plotting sublayout2
        // LonDepPlot
        QCPGraph *LonDepga = graph->addGraph(LonDep->axis(QCPAxis::atBottom), LonDep->axis(QCPAxis::atLeft));
        LonDepga->setData(SubCat.getSeries(Lon),
                      SubCat.getSeries(Depth));
        LonDepga->setLineStyle(QCPGraph::lsNone);
        LonDepga->setScatterStyle(QCPScatterStyle::ssDisc);
        LonDepga->setPen(allPen);
        // LatDepPlot
        QCPGraph *LatDepga = graph->addGraph(LatDep->axis(QCPAxis::atBottom), LatDep->axis(QCPAxis::atLeft));
        LatDepga->setData(SubCat.getSeries(Lat),
                      SubCat.getSeries(Depth));
        LatDepga->setLineStyle(QCPGraph::lsNone);
        LatDepga->setScatterStyle(QCPScatterStyle::ssDisc);
        LatDepga->setPen(allPen);
    }

//    QCPItemText *Min = new QCPItemText(graph);
//    Min->setText("Min: "+
//                 QString::number(CurrentCluster->getMin(Mag)));
//    QCPItemText *Max = new QCPItemText(graph);
//    Max->setText("Max: "+
//                 QString::number(CurrentCluster->getMax(Mag)));
//    QCPItemText *Avg = new QCPItemText(graph);
//    Avg->setText("Avg: "+
//                 QString::number(CurrentCluster->getAvg(Mag)));
//    QCPItemText *Med = new QCPItemText(graph);
//    Med->setText("Med: "+
//                 QString::number(CurrentCluster->getMedian(Mag)));
//    QCPItemText *Sdv = new QCPItemText(graph);
//    Sdv->setText("StDv: "+
//                 QString::number(CurrentCluster->getStdDev(Mag)));
//    QCPItemText *Nev = new QCPItemText(graph);
//    Nev->setText("#Ev: "+
//                 QString::number(CurrentCluster->numEvents()));


    if(ui->comboBox->currentData() == -1)
        return;

    // Let's start Plotting sublayout1
    // MagPlot
    QCPGraph *MagTimeg = graph->addGraph(MagTime->axis(QCPAxis::atBottom),MagTime->axis(QCPAxis::atLeft));
    MagTimeg->setData(CurrentCluster->getSeries(TimeS),
                  CurrentCluster->getSeries(Mag));
    MagTimeg->setLineStyle(QCPGraph::lsNone);
    MagTimeg->setScatterStyle(QCPScatterStyle::ssDisc);
    MagTimeg->setPen(Pen);
    // DepPlot
    QCPGraph *DepTimeg = graph->addGraph(DepTime->axis(QCPAxis::atBottom),DepTime->axis(QCPAxis::atLeft));
    DepTimeg->setData(CurrentCluster->getSeries(TimeS),
                  CurrentCluster->getSeries(Depth));
    DepTimeg->setLineStyle(QCPGraph::lsNone);
    DepTimeg->setScatterStyle(QCPScatterStyle::ssDisc);
    DepTimeg->setPen(Pen);

    // LatPlot
    QCPGraph *LatTimeg = graph->addGraph(LatTime->axis(QCPAxis::atBottom),LatTime->axis(QCPAxis::atLeft));
    LatTimeg->setData(CurrentCluster->getSeries(TimeS),
                  CurrentCluster->getSeries(Lat));
    LatTimeg->setLineStyle(QCPGraph::lsNone);
    LatTimeg->setScatterStyle(QCPScatterStyle::ssDisc);
    LatTimeg->setPen(Pen);
    // LonPlot
    QCPGraph *LonTimeg = graph->addGraph(LonTime->axis(QCPAxis::atBottom),LonTime->axis(QCPAxis::atLeft));
    LonTimeg->setData(CurrentCluster->getSeries(TimeS),
                  CurrentCluster->getSeries(Lon));
    LonTimeg->setLineStyle(QCPGraph::lsNone);
    LonTimeg->setScatterStyle(QCPScatterStyle::ssDisc);
    LonTimeg->setPen(Pen);

    // Let's start plotting sublayout2
    // LonDepPlot
    QCPGraph *LonDepg = graph->addGraph(LonDep->axis(QCPAxis::atBottom), LonDep->axis(QCPAxis::atLeft));
    LonDepg->setData(CurrentCluster->getSeries(Lon),
                  CurrentCluster->getSeries(Depth));
    LonDepg->setLineStyle(QCPGraph::lsNone);
    LonDepg->setScatterStyle(QCPScatterStyle::ssDisc);
    LonDepg->setPen(Pen);
    // LatDepPlot
    QCPGraph *LatDepg = graph->addGraph(LatDep->axis(QCPAxis::atBottom), LatDep->axis(QCPAxis::atLeft));
    LatDepg->setData(CurrentCluster->getSeries(Lat),
                  CurrentCluster->getSeries(Depth));
    LatDepg->setLineStyle(QCPGraph::lsNone);
    LatDepg->setScatterStyle(QCPScatterStyle::ssDisc);
    LatDepg->setPen(Pen);

    // MagHist
    QCPBars *MagHisg = new QCPBars(MagHist->axis(QCPAxis::atBottom), MagHist->axis(QCPAxis::atLeft));
    QVector<double> key, val;
    CurrentCluster->getHistSeries( &key, &val, Mag, magint);
    MagHisg->setData(key,val);
    MagHisg->keyAxis()->setSubTicks(false);
    MagHisg->setWidth(magint);
    MagHisg->rescaleAxes();
    MagHisg->keyAxis()->setTicker(magTicker);

    // DepthHist
    QCPBars *DepHisg = new QCPBars(DepHist->axis(QCPAxis::atBottom), DepHist->axis(QCPAxis::atLeft));
    key.clear();
    val.clear();
    CurrentCluster->getHistSeries( &key, &val, Depth, depint);
    DepHisg->setData(key,val);
    DepHisg->keyAxis()->setSubTicks(false);
    DepHisg->setWidth(depint);
    DepHisg->keyAxis()->setRangeReversed(true);
    DepHisg->rescaleAxes();
    DepHisg->keyAxis()->setTicker(depTicker);


//    temp->rescaleAxes();
//    Min->position->setType(QCPItemPosition::ptViewportRatio);
//    Min->position->setCoords(0.1,0.32);
//    Max->position->setParentAnchor(Min->position);
//    Max->position->setCoords(0,-15);
//    Med->position->setParentAnchor(Max->position);
//    Med->position->setCoords(0,-15);
//    Avg->position->setParentAnchor(Med->position);
//    Avg->position->setCoords(0,-15);
//    Sdv->position->setParentAnchor(Avg->position);
//    Sdv->position->setCoords(0,-15);
//    Nev->position->setParentAnchor(Sdv->position);
//    Nev->position->setCoords(0,-15);
    graph->replot();
}

void EQClusterViewer::genStats(QString filedir, bool pdf)
{
    if(pdf){
        QString table;
        table = "<style type=\"text/css\"> \
                .tg  {border-collapse:collapse;border-spacing:0;border: 1px solid black;}\
                .tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#ccc;color:#333;background-color:#fff;} \
                .tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#ccc;color:#333;background-color:#f0f0f0;} \
                .tg .tg-erlg{font-weight:bold;background-color:#efefef;vertical-align:top} \
                .tg .tg-2sfh{font-weight:bold;background-color:#efefef} \
                .tg .tg-yw4l{vertical-align:top} \
                </style> \
                <h1>Basic statistics for " + RegionName + " Region (Total Clusters: "+ QString::number(Clusters.size()) + ")</h1> \
                <table class=\"tg\" width=\"1000\"> \
                <tr> \
                <th class=\"tg-2sfh\" rowspan=\"2\">Cluster Id</th> \
                <th class=\"tg-2sfh\" rowspan=\"2\"># Event</th> \
                <th class=\"tg-erlg\" colspan=\"2\">Time YMD</th> \
                <th class=\"tg-erlg\" colspan=\"4\">Magnitude</th> \
                <th class=\"tg-erlg\" colspan=\"4\">Depth</th> \
                <th class=\"tg-erlg\" colspan=\"4\">Extents</th> \
                <th class=\"tg-erlg\" rowspan=\"2\">Class</th> \
                <th class=\"tg-erlg\" rowspan=\"2\">Trend</th> \
                </tr> \
                <tr> \
                <td class=\"tg-erlg\">Start</td> \
                <td class=\"tg-erlg\">End</td> \
                <td class=\"tg-erlg\">Min</td> \
                <td class=\"tg-erlg\">Mean</td> \
                <td class=\"tg-erlg\">Median</td> \
                <td class=\"tg-erlg\">Max</td> \
                <td class=\"tg-erlg\">Min</td> \
                <td class=\"tg-erlg\">Mean</td> \
                <td class=\"tg-erlg\">Median</td> \
                <td class=\"tg-erlg\">Max</td> \
                <td class=\"tg-erlg\">West</td> \
                <td class=\"tg-erlg\">North</td> \
                <td class=\"tg-erlg\">East</td> \
                <td class=\"tg-erlg\">South</td> \
                </tr>\n";
                for (int i = 0; i < Clusters.size(); i++){
                    table.append(QString("<tr> \
                    <td class=\"tg-yw4l\"> %1 </td> \
                    <td class=\"tg-yw4l\"> %2 </td> \
                    <td class=\"tg-yw4l\"> %3 </td> \
                    <td class=\"tg-yw4l\"> %4 </td> \
                    <td class=\"tg-yw4l\"> %5 </td> \
                    <td class=\"tg-yw4l\"> %6 </td> \
                    <td class=\"tg-yw4l\"> %7 </td> \
                    <td class=\"tg-yw4l\"> %8 </td> \
                    <td class=\"tg-yw4l\"> %9 </td> \
                    <td class=\"tg-yw4l\"> %10 </td> \
                    <td class=\"tg-yw4l\"> %11 </td> \
                    <td class=\"tg-yw4l\"> %12 </td> \
                    <td class=\"tg-yw4l\"> %13 </td> \
                    <td class=\"tg-yw4l\"> %14 </td> \
                    <td class=\"tg-yw4l\"> %15 </td> \
                    <td class=\"tg-yw4l\"> %16 </td> \
                    <td class=\"tg-yw4l\"> %17 </td> \
                    <td class=\"tg-yw4l\"> %18 </td> \
                    </tr>\n")
                    .arg(Clusters[i].getID())
                    .arg(Clusters[i].numEvents())
                    .arg(Clusters[i].getMinTime().toString("yyyy/MM/dd hh:mm:ss"))
                    .arg(Clusters[i].getMaxTime().toString("yyyy/MM/dd hh:mm:ss"))
                    .arg(Clusters[i].getMin(Mag),0,'f',2)
                    .arg(Clusters[i].getAvg(Mag),0,'f',2)
                    .arg(Clusters[i].getMedian(Mag),0,'f',2)
                    .arg(Clusters[i].getMax(Mag),0,'f',2)
                    .arg(Clusters[i].getMin(Depth),0,'f',2)
                    .arg(Clusters[i].getAvg(Depth),0,'f',2)
                    .arg(Clusters[i].getMedian(Depth),0,'f',2)
                    .arg(Clusters[i].getMax(Depth),0,'f',2)
                    .arg(Clusters[i].getMin(Lat))
                    .arg(Clusters[i].getMax(Lon))
                    .arg(Clusters[i].getMax(Lat))
                    .arg(Clusters[i].getMin(Lon))
                    .arg(Clusters[i].getClass(true))
                    .arg(Clusters[i].getTrend(true)));
        }
        table.append("</table>");
        QTextDocument Stats;
        Stats.setHtml(table);

        QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
        printer->setFullPage(true);
        printer->setResolution(90);
        printer->setPaperSize(QPrinter::A4);
        printer->setOrientation(QPrinter::Landscape);
        printer->setPageMargins(5, 5, 5, 5, QPrinter::Millimeter);
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName(filedir + RegionName + "_Stats.pdf");

        Stats.print(printer);
    } else {
        QFile file(filedir + RegionName + "_Stats.csv");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
        QTextStream out(&file);

        QString table;
        table = "Basic statistics for " + RegionName + " Region (Total Clusters: "+ QString::number(Clusters.size()) + ")\n"
                + "Cluster Id, # Event, Time Start YMD HH:MM:SS,Time End YMD HH:MM:SS, Min Mag, Avg Mag, Med Avg, Max Mag, "
                + "Min Dep, Avg Dep, Med Dep, Max Dep, Ext West, Ext North, Ext East, Ext South, Class, Trend \n";
                for (int i = 0; i < Clusters.size(); i++){
                    table.append(QString(
                     "%1,  %2, %3,  %4,  %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18 \n")
                    .arg(Clusters[i].getID())
                    .arg(Clusters[i].numEvents())
                    .arg(Clusters[i].getMinTime().toString("yyyy/MM/dd hh:mm:ss"))
                    .arg(Clusters[i].getMaxTime().toString("yyyy/MM/dd hh:mm:ss"))
                    .arg(Clusters[i].getMin(Mag),0,'f',2)
                    .arg(Clusters[i].getAvg(Mag),0,'f',2)
                    .arg(Clusters[i].getMedian(Mag),0,'f',2)
                    .arg(Clusters[i].getMax(Mag),0,'f',2)
                    .arg(Clusters[i].getMin(Depth),0,'f',2)
                    .arg(Clusters[i].getAvg(Depth),0,'f',2)
                    .arg(Clusters[i].getMedian(Depth),0,'f',2)
                    .arg(Clusters[i].getMax(Depth),0,'f',2)
                    .arg(Clusters[i].getMin(Lat))
                    .arg(Clusters[i].getMax(Lon))
                    .arg(Clusters[i].getMax(Lat))
                    .arg(Clusters[i].getMin(Lon))
                    .arg(Clusters[i].getClass(true))
                    .arg(Clusters[i].getTrend(true)));
        }
        out << table;
        file.close();
    }
}

void EQClusterViewer::genSortFile(QString filedir, clustype type)
{
    QMetaEnum fromt = QMetaEnum::fromType<EQClusterViewer::clustype>();
    QFile file(filedir + RegionName + "_"+ fromt.valueToKey(type) +".csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    QTextStream out(&file);

    QString table;

    table = RegionName + " Region "+fromt.valueToKey(type)+" (Total Clusters: "+ QString::number(Clusters.size()) + ")\n"
            + "Cluster Id, Lat, Lon, Date, N, Unix Ep (s), DT (in sec), Mag, Depth, Class, Trend \n";

    for(int i = 0; i < Clusters.size(); i++){
        Cluster *current = &Clusters[i] ;
        switch (type) {
        case swarms:
            if (current->getClusterType() == swm)
                for(int j = 0; j < current->numEvents(); j++){
                    table.append(QString("%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11\n")
                                 .arg(current->getID())
                                 .arg(current->getEvent(j).latitude)
                                 .arg(current->getEvent(j).longitude)
                                 .arg(current->getEvent(j).time.toString()).arg(j).arg(current->getEvent(j).time_in_usec)
                                 .arg(current->getEvent(j).time_in_usec - current->getMin(TimeS))
                                 .arg(current->getEvent(j).magnitude)
                                 .arg(current->getEvent(j).depth)
                                 .arg(current->getClass(true))
                                 .arg(current->getTrend(true)));
                }
            break;
        case sequences:
            if (current->getClusterType() == seq)
                for(int j = 0; j < current->numEvents(); j++){
                    table.append(QString(" %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11\n")
                                 .arg(current->getID())
                                 .arg(current->getEvent(j).latitude)
                                 .arg(current->getEvent(j).longitude)
                                 .arg(current->getEvent(j).time.toString()).arg(j).arg(current->getEvent(j).time_in_usec)
                                 .arg(current->getEvent(j).time_in_usec - current->getMin(TimeS))
                                 .arg(current->getEvent(j).magnitude)
                                 .arg(current->getEvent(j).depth)
                                 .arg(current->getClass(true))
                                 .arg(current->getTrend(true)));
                }
            break;
        case all:
            for(int j = 0; j < current->numEvents(); j++){
                table.append(QString("%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11\n")
                             .arg(current->getID())
                             .arg(current->getEvent(j).latitude)
                             .arg(current->getEvent(j).longitude)
                             .arg(current->getEvent(j).time.toString()).arg(j).arg(current->getEvent(j).time_in_usec)
                             .arg(current->getEvent(j).time_in_usec - current->getMin(TimeS))
                             .arg(current->getEvent(j).magnitude)
                             .arg(current->getEvent(j).depth)
                             .arg(current->getClass(true))
                             .arg(current->getTrend(true)));
            }
            break;
        default:
            return;
            break;
        }
    }
    out << table;
    file.close();
}

void EQClusterViewer::on_class_2_clicked()
{
    QPair<clusT,trendT> info;
    QDialog *classify = new Classify(this, &info);
    classify->move(0,0);
    if(classify->exec() == QDialog::Accepted){
        // Point CurrentCluster to the currently selected swarm
        Cluster *CurrentCluster = &Clusters[ui->comboBox->currentData().toInt()];
        CurrentCluster->setClusterTypeTrend(info.first, info.second);
        plotCluster(ui->comboBox->currentData().toInt());
    }
}

void EQClusterViewer::on_comboBox_currentIndexChanged(int index)
{
    if(index < ui->comboBox->count()-1){
        plotCluster(index);
    }
}

void EQClusterViewer::on_actionSave_sequences_triggered()
{
    QString filedir;

    QFileDialog savefilestat(this);
    savefilestat.setDirectory(QDir::currentPath());
    savefilestat.setOption(QFileDialog::ShowDirsOnly);
    savefilestat.setFileMode(QFileDialog::Directory);
    savefilestat.setWindowTitle("Select CSV save path");
    savefilestat.show();
    if(savefilestat.exec()){
        filedir = savefilestat.selectedFiles().at(0);
        filedir.append('/');
    }else{
        return;
    }
    genSortFile(filedir, sequences);
}

void EQClusterViewer::on_actionSave_statistics_triggered()
{
    QString filedir;

    QFileDialog savefilestat(this);
    savefilestat.setDirectory(QDir::currentPath());
    savefilestat.setOption(QFileDialog::ShowDirsOnly);
    savefilestat.setFileMode(QFileDialog::Directory);
    savefilestat.setWindowTitle("Select CSV save path");
    savefilestat.show();
    if(savefilestat.exec()){
        filedir = savefilestat.selectedFiles().at(0);
        filedir.append('/');
    }else{
        return;
    }
    genStats(filedir, false);
}

void EQClusterViewer::on_actionSave_all_Classifications_triggered()
{
    QString filedir;

    QFileDialog savefilestat(this);
    savefilestat.setDirectory(QDir::currentPath());
    savefilestat.setOption(QFileDialog::ShowDirsOnly);
    savefilestat.setFileMode(QFileDialog::Directory);
    savefilestat.setWindowTitle("Select CSV save path");
    savefilestat.show();
    if(savefilestat.exec()){
        filedir = savefilestat.selectedFiles().at(0);
        filedir.append('/');
    }else{
        return;
    }
    genSortFile(filedir, all);
}

void EQClusterViewer::on_actionSave_swarms_triggered()
{
    QString filedir;

    QFileDialog savefilestat(this);
    savefilestat.setDirectory(QDir::currentPath());
    savefilestat.setOption(QFileDialog::ShowDirsOnly);
    savefilestat.setFileMode(QFileDialog::Directory);
    savefilestat.setWindowTitle("Select CSV save path");
    savefilestat.show();
    if(savefilestat.exec()){
        filedir = savefilestat.selectedFiles().at(0);
        filedir.append('/');
    }else{
        return;
    }
    genSortFile(filedir, swarms);
}

void EQClusterViewer::on_classall_clicked()
{
    for(int i = 0; i < ui->comboBox->count()-1; i++){
        ui->comboBox->setCurrentIndex(i);
        on_class_2_clicked();
    }
}
