#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QMap>
#include <QString>
#include "qcustomplot/qcustomplot.h"
#include <QRegExp>


namespace Ui {
class App;
}

struct Instruction
{
	int _timestampSection;
	int _valueSection;
	double _offset;
	double _scale;
	QRegExp _rx;
	QString _name;
};

class App : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit App(QWidget *parent = 0);
    ~App();
    
private slots:
    void on_bReload_clicked();

	//void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);
	void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
	void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
	void selectionChanged();
	void mousePress();
	void mouseWheel();
	void removeSelectedGraph();
	void removeAllGraphs();
	void contextMenuRequest(QPoint pos);
	void moveLegend();
	void graphClicked(QCPAbstractPlottable *plottable);

	void zoomXIn();
	void zoomXOut();
	void zoomYIn();
	void zoomYOut();

	bool addPlot(QString);
	void processRangeChanged(const QCPRange &newRange);

	QCustomPlot* getPlot(QString);

	void processXRangeChange(const QCPRange &newRange);

	bool plotData(QString, QVector<double> x, QVector<double> y);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

private:
    Ui::App *ui;
	QList<Instruction> _instructions;
	QMap<QString, QCustomPlot*> _plots; //TODO not map. RX may be same for many graphs
	QCustomPlot* _currentPlot;

};

#endif // APP_H
