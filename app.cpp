#include "app.h"
#include "ui_app.h"
#include <os/fstools.h>
#include <os/persistence.h>
#include <QInputDialog>
#include <QDateTime>
#include <QShortcut>

App::App(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::App),
	_currentPlot(0)
{
    ui->setupUi(this);
	PERSISTENCE_INIT( "heinitz-it.de", "LogVisualiser" )
	PERSISTENT("RXText", ui->tParser, "plainText" )
    PERSISTENT("LogFile", ui->eLogFile, "text" )

	 
  QShortcut* xIn = new QShortcut(Qt::CTRL + Qt::Key_1, this);
  connect( xIn, SIGNAL(activated()), this, SLOT(zoomXIn()));

  QShortcut* xOut = new QShortcut(Qt::CTRL + Qt::Key_2, this);
  connect( xOut, SIGNAL(activated()), this, SLOT(zoomXOut()));

  QShortcut* yIn = new QShortcut(Qt::CTRL + Qt::Key_3, this);
  connect( yIn, SIGNAL(activated()), this, SLOT(zoomYIn()));

  QShortcut* yOut = new QShortcut(Qt::CTRL + Qt::Key_4, this);
  connect( yOut, SIGNAL(activated()), this, SLOT(zoomYOut()));
}

bool App::addPlot(QString plotName)
{
	if ( _plots.contains( plotName ) )
		return false;

	_plots[plotName] = new QCustomPlot;
	ui->ltPlots->addWidget( _plots[plotName] );

	_plots[plotName]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
 
  
	QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	timeTicker->setTimeFormat( "%h:%m:%s.%z" );
	//_plots[plotName]->xAxis->setTicker(timeTicker);
	

	_plots[plotName]->xAxis->setLabel("time");
	_plots[plotName]->yAxis->setLabel(plotName);


	// connect slot that ties some axis selections together (especially opposite axes):
	connect(_plots[plotName], SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
	// connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
	connect(_plots[plotName], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
	connect(_plots[plotName], SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

	// make bottom and left axes transfer their ranges to top and right axes:
	connect(_plots[plotName]->xAxis, SIGNAL(rangeChanged(QCPRange)), _plots[plotName]->xAxis2, SLOT(setRange(QCPRange)));
	connect(_plots[plotName]->yAxis, SIGNAL(rangeChanged(QCPRange)), _plots[plotName]->yAxis2, SLOT(setRange(QCPRange)));

	connect(_plots[plotName]->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(processXRangeChange(QCPRange)));
	

	// connect some interaction slots:
	connect(_plots[plotName], SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
	connect(_plots[plotName], SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
	connect(_plots[plotName], SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

	// connect slot that shows a message in the status bar when a graph is clicked:
	connect(_plots[plotName], SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

	// setup policy and connect slot for context menu popup:
	_plots[plotName]->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(_plots[plotName], SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

	_plots[plotName]->setMinimumHeight(150);
}

QCustomPlot* App::getPlot(QString plotName)
{
	if ( !_plots.contains( plotName ) )
		return 0;

	return 0;
}

bool App::plotData(QString plotName, QVector<double> x, QVector<double> y)
{
	if ( !_plots.contains( plotName ) )
		return false;

	return true;
}





void App::zoomXIn()
{
	if (!_currentPlot) return;
	_currentPlot->xAxis->scaleRange( .5 );
	_currentPlot->replot();
}

void App::zoomXOut()
{
	if (!_currentPlot) return;
	_currentPlot->xAxis->scaleRange( 2 );
	_currentPlot->replot();
} 

void App::zoomYIn()
{
	if (!_currentPlot) return;
	_currentPlot->yAxis->scaleRange( .5 );
	_currentPlot->replot();
}

void App::zoomYOut()
{
	if (!_currentPlot) return;
	_currentPlot->yAxis->scaleRange( 2 );
	_currentPlot->replot();
}

App::~App()
{
    delete ui;
}

void App::on_bReload_clicked()
{

	foreach(QString plotName, _plots.keys() )
	{
		delete _plots[plotName];
	}
	_plots.clear();

	_parsInstructions = FSTools::mapFromText( ui->tParser->toPlainText(), QRegExp("\t") );

	QMap<QString, QPair<QVector<double>, QVector<double> > > plotData;

	QStringList logdata = FSTools::fromFile(ui->eLogFile->text() );
	int i = 0;
	double xOffset=-1;
	foreach(QString l, logdata )
	{
		QStringList items = l.split("\t");
		if (items.size() >= 3 )
		{
			foreach(QString rxstr, _parsInstructions.keys() )
			{
				QRegExp rx(rxstr);
				if ( l.contains(rx) )
				{
					QString tsrxstr = items.at(0);	// Extracting timestemp
					QRegExp tsrx(tsrxstr);			// The rx should contain one single block ()
					tsrx.indexIn(l);				// after parsing ...
					QString dt = tsrxstr;//tsrx.cap(1);       // the captured string is got by cap(1)
					QString tsstr = l.section(" ",-3,-3);
					QString dname = l.section(" ",-2,-2);
					double ts = tsstr.toDouble(); //TODO parse properly QDateTime::fromString( dt, "yyyy-MM-dd HH:mm:ss" ).toMSecsSinceEpoch();
					if ( xOffset < 0 )
						xOffset = ts;
					ts -= xOffset;
					ts /=1000; // sec/10
					double y = l.section(" ",-1,-1).toDouble();
					if ( dname == "MicroZaxis.position" )
					{
						y -= 17000000;
						y /= 840;
					}
					plotData[dname].first.append(ts);
					plotData[dname].second.append(y);
					ui->tOutput->append(l);
				}
			}
		}
	}

	
	//for( int i=0; i<x.size(); i++ )x[i] = x[i]/offs;

	foreach(QString plotName, plotData.keys() )
	{
		addPlot(plotName);
	}

	foreach(QString plotName, plotData.keys() )
	{
		QVector<double> &x = plotData[plotName].first;
		if (x.isEmpty())
			continue;
		double offs = x[0];

		QVector<double> &y = plotData[plotName].second;
		QCPRange xr = QCPRange( x.first(), x.last() );
		_plots[plotName]->xAxis->setRange(xr);
		_plots[plotName]->addGraph();
		//_plots[plotName]->graph()->setName(QString("New graph %1").arg(_plots[plotName]->graphCount()-1));
		_plots[plotName]->graph()->setData(x, y);
		_plots[plotName]->graph()->setLineStyle(QCPGraph::lsImpulse);
		_plots[plotName]->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
		_plots[plotName]->yAxis->setRange(0, *std::max_element(y.begin(), y.end()));
		_plots[plotName]->axisRect()->setupFullAxesBox();
		//QPen graphPen;
		//graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
		//graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
		//ui->customPlot->graph()->setPen(graphPen);
		_plots[plotName]->replot();
	}	
}

void App::processXRangeChange(const QCPRange &newRange)
{
	foreach(QString plotName, _plots.keys() )
	{
		if ( _plots[plotName]->xAxis->range()!= newRange  )
		{
			_plots[plotName]->xAxis->setRange( newRange );
			_plots[plotName]->replot();
		}
	}
}



void App::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      plot->replot();
    }
  }
}

void App::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      plot->replot();
    }
  }
}

void App::selectionChanged()
{
   QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  _currentPlot = plot;
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      plot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    plot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    plot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      plot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    plot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    plot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<plot->graphCount(); ++i)
  {
    QCPGraph *graph = plot->graph(i);
    QCPPlottableLegendItem *item = plot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      //graph->setSelected(true);
    }
  }
}

void App::mousePress()
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeDrag(plot->xAxis->orientation());
  else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeDrag(plot->yAxis->orientation());
  else
    plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void App::mouseWheel()
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
  else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeZoom(plot->yAxis->orientation());
  else
    plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}



void App::removeSelectedGraph()
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  if (plot->selectedGraphs().size() > 0)
  {
    plot->removeGraph(plot->selectedGraphs().first());
    plot->replot();
  }
}

void App::removeAllGraphs()
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  plot->clearGraphs();
  plot->replot();
}

void App::contextMenuRequest(QPoint pos)
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  if (plot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (plot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (plot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }
  
  menu->popup(plot->mapToGlobal(pos));
}

void App::moveLegend()
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      plot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      plot->replot();
    }
  }
}

void App::graphClicked(QCPAbstractPlottable *plottable)
{
	QCustomPlot * plot = qobject_cast<QCustomPlot *>( sender() );
  if (!plot) return;
  ui->statusBar->showMessage(QString("Clicked on graph '%1'.").arg(plottable->name()), 1000);
}
