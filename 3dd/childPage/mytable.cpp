#include "mytable.h"
#include<QFileDialog.h>
int Mytable::m_number_of_this_table = 0;
Mytable::Mytable(int row_count,QWidget *parent) : QTableWidget(parent)
{
	this->setColumnCount(24);
	this->setRowCount(row_count);
	m_number_of_this_table++;
	connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(table_is_modify()));
}

void Mytable::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Down)
	{
		int a = this->currentRow();
		int b = this->currentColumn();
		if (a < this->rowCount() - 1)
		{
			this->setCurrentCell(a + 1, b);
			this->update();
			return;
		}
		int row = this->rowCount();
		this->insertRow(row);
		row_count = this->rowCount();
		this->setCurrentCell(a + 1, b);
		this->update();
	}
	if (event->key() == Qt::Key_Up)
	{
		int a = this->currentRow();
		int b = this->currentColumn();
		if (a > 0)
			this->setCurrentCell(a - 1, b);
		else
			return;
	}

	if (event->key() == Qt::Key_Right)
	{
		int a = this->currentRow();
		int b = this->currentColumn();
		if (b < this->columnCount() - 1)
		{
			this->setCurrentCell(a, b + 1);
			this->update();
			return;
		}
		int row = this->columnCount();
		this->insertColumn(row);
		this->setCurrentCell(a, b + 1);
		this->update();
	}

	if (event->key() == Qt::Key_Left)
	{
		int a = this->currentRow();
		int b = this->currentColumn();
		if (b > 0)
			this->setCurrentCell(a, b - 1);
		else
			return;
	}
}

void Mytable::wheelEvent(QWheelEvent *event) 
{
	if (event->delta() < 0)
	{
		int a = this->verticalScrollBar()->value();
		int b = this->rowCount();
		if (this->verticalScrollBar()->sliderPosition() + 29 < row_count)
		{
			this->verticalScrollBar()->setValue(a + 1);
			update();
			return;
		}
		int row = this->rowCount();
		this->insertRow(row);
		this->verticalScrollBar()->setValue(a + 1);
		update();
	}

	else if (event->delta() > 0)
	{
		int a = this->verticalScrollBar()->value();
		this->verticalScrollBar()->setValue(a - 1);

		if (row_count < this->rowCount())
		{
			this->removeRow(this->rowCount() - 1);
		}
		update();
	}
}

void Mytable::save_new_table(Mytable *current_table)
{
	QString path = QFileDialog::getSaveFileName(this, "save_table", ".", "dat(*.dat)");
	if (path.isEmpty() == false)
	{
		QFile file(path);
		if (file.open(QIODevice::WriteOnly) == true)
		{
			QTextStream text(&file);
			text.setCodec("UTF-8");
			//QString table_text = "";
			for (int row = 0; row < current_table->rowCount(); row++)
			{
				for (int col = 0; col < current_table->columnCount(); col++)
				{
					if (current_table->item(row, col) != 0)
					{
						text << QString(current_table->item(row, col)->text());
						text << QString("\t");
					}
					else
						text << QString("\t");
				}
				text << QString("\r\n");
			}
			file.close();
		}
	}
}

void Mytable::save_exist_table(Mytable*current_table,QString file_name)
{
	QFile file(file_name);
	if (file.open(QIODevice::WriteOnly) == true)
	{
		QTextStream text(&file);
		text.setCodec("UTF-8");
		for (int row = 0; row < current_table->rowCount(); row++)
		{
			for (int col = 0; col < current_table->columnCount(); col++)
			{
				if (current_table->item(row, col) != 0)
				{
					text << QString(current_table->item(row, col)->text());
					text << QString("\t");
				}
			}
			text << QString("\r\n");
		}
		file.close();
	}
}

void Mytable::slot_close_current_new_table(Mytable *current_table) 
{
	if (current_table->m_table_edit_flag==1)     //文件被修改，是否保存?
	{
		QString title = "file message";
		QString str = "The file have not been saved,do you save?";
		QMessageBox::StandardButton result;
		result = QMessageBox::question(this, title, str, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (result == QMessageBox::Yes)
		{
			save_new_table(current_table);
		}
	}
}

void Mytable::slot_close_exist_table(Mytable *current_table, QString path_name)
{
	if (current_table->m_table_edit_flag == 1)
	{
		QString title = "file name";
		QString str = "The file have been modified,do you save the modify?";
		QMessageBox::StandardButton result;
		result = QMessageBox::question(this, title, str, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (result == QMessageBox::Yes)
		{
			save_exist_table(current_table, path_name);
		}
	}
}
