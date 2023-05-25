#pragma once
#include <QTreeWidgetItem>
#include <QObject>
class My_QTreeWidgetItem:public QObject,public QTreeWidgetItem
{
	Q_OBJECT
public:
	My_QTreeWidgetItem(QTreeWidgetItem  *parent,const QStringList &);
	~My_QTreeWidgetItem();
};