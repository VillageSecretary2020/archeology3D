#pragma once
#include "base_property_widget.h"

class vector_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	vector_property_widget(QString a = "");
	virtual void delete_self();
private:
	QTreeWidget *m_NormalProperty;
	QTreeWidgetItem *m_vector_treeitem;
	QTreeWidgetItem *m_input_data_treeitem;
	QTreeWidgetItem *m_input_scalar_treeitem;
	QTreeWidgetItem *m_place_direction_treeitem;
	QTreeWidgetItem *m_frequent_treeitem;
	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_vector_style_treeitem;
	QTreeWidgetItem *m_scaling_method_treeitem;
	QTreeWidgetItem *m_min_rate_treeitem;
	QTreeWidgetItem *m_max_rate_treeitem;
	QTreeWidgetItem *m_arrow_rate_treeitem;
	QTreeWidgetItem *m_symbol_size_treeitem;
	QTreeWidgetItem *m_line_width_treeitem;
	QTreeWidgetItem *m_reverse_treeitem;
	QTreeWidgetItem *m_color_method_treeitem;
	QSpinBox *m_Frequency;
	QComboBox *m_VectorStyle;
	QComboBox *m_ScalingMethod;  //缩放法
	QDoubleSpinBox *m_MinRate_spin;
	QSlider *m_MinRate_slider;
	QDoubleSpinBox *m_MaxRate_spin;
	QSlider *m_MaxRate_slider;
	QDoubleSpinBox *m_ArrowRate_spin;
	QSlider *m_ArrowRate_slider;
	QDoubleSpinBox *m_SymbolSize_spin;
	QSlider *m_SymbolSize_slider;
	QDoubleSpinBox *m_LineWidth_spin;
	QSlider *m_LineWidth_slider;
	QCheckBox *m_Reverse;
	QComboBox *m_ColorMethod;
	void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
signals:
	void sig_vector_to_axis();
	void sig_vector_to_border();
	void sig_delete_vector();
private slots:
	inline void m_MinRate_slider_setValue(double a)
	{
		int value = (int)(a * 10000);
		m_MinRate_slider->setValue(value);
	}
	inline void m_MinRate_spin_setValue(int a)
	{
		double value = a / 10000.0;
		m_MinRate_spin->setValue(value);
	}
	inline void m_MaxRate_slider_setValue(double a)
	{
		int value = (int)(a * 10000);
		m_MaxRate_slider->setValue(value);
	}
	inline void m_MaxRate_spin_setValue(int a)
	{
		double value = a / 10000.0;
		m_MaxRate_spin->setValue(value);
	}
	inline void m_ArrowRate_slider_setValue(double a)
	{
		int value = (int)(a * 10000);
		m_ArrowRate_slider->setValue(value);
	}
	inline void m_ArrowRate_spin_setValue(int a)
	{
		double value = a / 10000.0;
		m_ArrowRate_spin->setValue(value);
	}
	inline void m_SymbolSize_slider_setValue(double a)
	{
		int value = (int)(a * 10000);
		m_SymbolSize_slider->setValue(value);
	}
	inline void m_SymbolSize_spin_setValue(int a)
	{
		double value = a / 10000.0;
		m_SymbolSize_spin->setValue(value);
	}
	inline void m_LineWidth_slider_setValue(double a)
	{
		int value = (int)(a * 10000);
		m_LineWidth_slider->setValue(value);
	}
	inline void m_LineWidth_spin_setValue(int a)
	{
		double value = a / 10000.0;
		m_LineWidth_spin->setValue(value);
	}

	void slot_vector_to_axis();
	void slot_vector_to_border();

};

