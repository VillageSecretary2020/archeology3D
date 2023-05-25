#pragma once
#include "base_property_widget.h"

class source_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	source_property_widget(QString str, SuperNodeStruct* param=nullptr);
	~source_property_widget();
	bool set_progress(int r, int p, const char* m);
   D_DataInfo* return_data_source_property_struct();
   virtual void delete_self();
   virtual void end_progress();
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
private:
   QTreeWidget *m_normal_property;
   QTreeWidgetItem *m_path_treeitem;
   QTreeWidgetItem *m_file_path_treeitem;
signals:
	void sig_delete_source();
	void sig_set_progress(int range, int pos, const char*);
	/*
	void sig_property_manager_dock_null();
	void sig_data_to_scatter();
	void sig_data_to_vector();
	void sig_data_to_axis();
	void sig_data_to_border();
	void sig_data_to_gridding();
private slots:
	void slot_data_to_vector();
	void slot_data_to_scatter();
	void slot_data_to_axis();
	void slot_data_to_border();
	//void slot_edit_table();
	void slot_data_to_gridding();
	*/
private	slots:
	void slot_set_progress(int r, int p, const char* m);
};
