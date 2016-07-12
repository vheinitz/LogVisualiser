#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QMap>
#include <QString>
#include "qcustomplot/qcustomplot.h"


namespace Ui {
class App;
}

class App : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit App(QWidget *parent = 0);
    ~App();
    
private slots:
    void on_bReload_clicked();

	void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);
	void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
	void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
	void selectionChanged();
	void mousePress();
	void mouseWheel();
	void addRandomGraph();
	void removeSelectedGraph();
	void removeAllGraphs();
	void contextMenuRequest(QPoint pos);
	void moveLegend();
	void graphClicked(QCPAbstractPlottable *plottable);

private:
    Ui::App *ui;
	QMap<QString, QString> _parsInstructions;
};

#endif // APP_H
