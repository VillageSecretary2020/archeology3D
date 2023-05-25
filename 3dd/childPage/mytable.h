#pragma once
#include <QTableWidget>
#include <QAbstractItemModel>
#include <QKeyEvent>
#include <QScrollBar>
#include<QDebug>
#include<QWheelEvent>
#include<QStringList>
#include<QHeaderView>
#include <QVector>
#include <QMessageBox>
class Mytable:public QTableWidget
{
	Q_OBJECT
public:
	 Mytable(int,QWidget *parent = nullptr);
	QVector<QVector<QString>> vector1;
	QVector<QVector<QString>> vector2;
	static int m_number_of_this_table;
	inline void set_open_file_name(QString path_name)
	{
		m_path_name = path_name;
	}
	inline QString get_open_file_name()
	{
		return m_path_name;
	}
private:
	int row_count;
	int m_table_edit_flag = 0;  //表格是否被编辑的标志，默认为0表示未编辑修改，为后面保存做判断
	void save_new_table(Mytable*);
	void save_exist_table(Mytable*,QString);
	QString m_path_name;
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *);
private slots:
	void slot_close_current_new_table(Mytable*);
	void slot_close_exist_table(Mytable*,QString);
	inline void table_is_modify()
	{
		m_table_edit_flag = 1;
	};

};
