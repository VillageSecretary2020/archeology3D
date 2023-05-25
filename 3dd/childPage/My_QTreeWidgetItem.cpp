#include "My_QTreeWidgetItem.h"
My_QTreeWidgetItem::My_QTreeWidgetItem(QTreeWidgetItem  *parent,const QStringList  & strings):QTreeWidgetItem(parent,strings)
{
	this->setBackground(0, QBrush(QColor("#FFFFFF")));
	this->setBackground(1, QBrush(QColor("#FFFFFF")));
}

My_QTreeWidgetItem::~My_QTreeWidgetItem()
{

}