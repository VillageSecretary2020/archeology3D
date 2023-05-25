/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
// str_lang.h
// 文件功能描述：
//	界面上的多语言字符串
//
// 创建者：yzw@dappworks.cn
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/

#include "str_def.h"

#define LANG_ZH_CN
#ifndef LANG_ZH_CN
#define STRING_MENU_FILE		"file"
#define STRING_CALC_MENU		tr("calculate")
#define STRING_GRIDDING_MENU	tr("gridding")
#define STRING_MENU_GRAPHIC		tr("graphic output")
#define STRING_SLICE_MENU		tr("slice diagram")
#define STRING_ISOSURF_MENU		tr("isosurface")
#define STRING_DOCK_MODULE		"module manager"
#define STRING_DOCK_DIAGRAM		"contact diagram manager"
#define STRING_DOCK_PROPERTY	tr("property manager")
#define STRING_IMPORT_MODULE	tr("import")
#define STRING_DOCK_DESCRIP		tr("property description")
#define STRING_TAB_PROPERTY		tr("normal property")
#define STRING_TAB_GEOMETRY		tr("geometry")
#define STRING_TAB_SEARCH		tr("search")
#define STRING_START_BUTTON		tr("start gridding")
#define STRING_TITLE_WAIT		tr("A minute ...")
#define STRING_DATA_SOURCE		tr("data source")
//cpl 8.4
#define STRING_LIST_INPUT		tr("input")
#define STRING_LIST_SOURCE		tr("source")
#define STRING_LIST_ISOLINE		tr("isoline")
#define STRING_LIST_INOUTDTLI	tr("inout data limit")
#define STRING_LIST_LEVEL		tr("level")
#define STRING_LIST_LEVELCAL	tr("level calculate")
#define STRING_LIST_AUTO		tr("auto calculate")
#define STRING_LIST_MIN			tr("Min max count")
#define STRING_LIST_MAX			tr("Min max interval")
#define STRING_LIST_EXPLICIT	tr("explicit")

#else
#define STRING_DOCK_			u8""

#define STRING_MENU_FILE		tr(u8"文件&F")
#define STRING_MENU_RENDER		tr(u8"渲染图&R")
#define STRING_CALC_MENU		tr(u8"计算&C")
#define STRING_MENU_GRIDDING	tr(u8"网格化")
#define STRING_MENU_GRAPHIC		tr(u8"渲染图形")
#define STRING_MENU_GRAOUTPUT		tr(u8"图形输出")		//graphic output
#define STRING_SLICE_MENU				tr(u8"切片图")
#define STRING_ISOSURF_MENU		tr(u8"等值面图")	// isosurface
#define STRING_DOCK_MODULE		tr(u8"模块管理器")
#define STRING_DOCK_DIAGRAM		tr(u8"联络图管理器")
#define STRING_IMPORT_MODULE	tr(u8"导入数据")
#define STRING_DOCK_PROPERTY	tr(u8"属性管理器")
#define STRING_DOCK_DESCRIP		tr(u8"详情")

#define STRING_TAB_PROPERTY		tr(u8"常规设置")
#define STRING_TAB_GENERAL			tr(u8"常规设置")		//general properties
#define STRING_TAB_GEOMETRY		tr(u8"几何属性")
#define STRING_TAB_SEARCH			tr(u8"搜索")
#define STRING_TAB_CUT_PLANE	tr(u8"切割平面")		//cut plane
#define STRING_TAB_COLOR_SCALE	tr(u8"色阶")		//color scale
#define STRING_TAB_NOTATE			tr(u8"标注")		//notate

#define STRING_ITEM_COLOR_SCALE	tr(u8"色阶")		//color scale

#define STRING_START_BUTTON		tr(u8"开始网格化")
#define STRING_STOP_BUTTON		tr(u8"停止网格化")
#define STRING_TITLE_WAIT				tr(u8"请稍候...")
#define STRING_TITLE_RENAME				tr(u8"命名")
#define STRING_LABEL_RENAME				tr(u8"新名称")
#define STRING_DATA_SOURCE		tr(u8"源数据")
//cpl 8.4
//-----isoline_property_widget.cpp
#define STRING_LABEL_ISOLINE			tr(u8"等值线图")		//isoline
#define STRING_ITEM_ISOLINE			tr(u8"等值线图")		//isoline
#define STRING_ITEM_INOUTDTLI		tr(u8"数据限制")		//inout data limit
#define STRING_ITEM_LEVEL			tr(u8"水平")			//level
#define STRING_ITEM_LEVELCAL		tr(u8"水平计算")		//level calculate
#define STRING_COMBOX_AUTO			tr(u8"自动计算")		//auto calculate
#define STRING_COMBOX_MINMAXCOUN	tr(u8"最小最大计数")	//Min max count
#define STRING_COMBOX_MINMAXINTR	tr(u8"最小最大间隔")	//Min max interval
#define STRING_COMBOX_EXPLICIT		tr(u8"显式指定")			//explicit
#define STRING_ITEM_MINLEV			tr(u8"最小级别")		//min level
#define STRING_ITEM_MAXLEV			tr(u8"最大级别")		//max level
#define STRING_ITEM_LEVNUM			tr(u8"级别数")			//level number
#define STRING_ITEM_LEVINT			tr(u8"级别间隔")		//level interval
#define STRING_ITEM_CLASSIFY		tr(u8"分类")			//classify
#define STRING_ITEM_GRADCHRO		tr(u8"梯度色谱法")	//gradient chromatography
#define STRING_ITEM_SHOWBORD		tr(u8"显示边界")		//show border
#define STRING_ITEM_BORDWIDTH		tr(u8"边界宽度")		//border width
#define STRING_ITEM_BORDCOLOR		tr(u8"边界颜色")		//border color
#define STRING_ITEM_SHOWSLICE		tr(u8"显示切片")		//show slice

//cut_plane_property.cpp
#define STRING_ITEM_CUTPLANE		tr(u8"切割平面")		//cutting plane
#define STRING_ITEM_VSURF				tr(u8"法线")
#define STRING_ITEM_VPARA				tr(u8"二点")
#define STRING_ITEM_THREE_POINTS	tr(u8"三点")	
#define STRING_ITEM_AXIS_POINTS	tr(u8"轴与点")	
#define STRING_ITEM_AXISES				tr(u8"轴")
#define STRING_ITEM_NORMAL			tr(u8"法线")		//normal 
#define STRING_ITEM_PLACEDIR		tr(u8"放置方向")		//placement direction
#define STRING_ITEM_NORMALDIR		tr(u8"法线方向")		//normal direction
#define STRING_ITEM_OFFSETCENT		tr(u8"从中心偏移比例")	//offset from center
#define STRING_ITEM_SEL_MODE		tr(u8"选择方法")		//placement direction
#define STRING_ITEM_POINT_A		tr(u8"点A")		// point A
#define STRING_ITEM_POINT_B		tr(u8"点B")		// point B
#define STRING_ITEM_POINT_C		tr(u8"点C")		// point C
#define STRING_ITEM_VPARA_DESC		tr(u8"切面经过二点且与指定轴平行")
#define STRING_ITEM_THREE_POINTS_DESC	tr(u8"切面经过三点")	

//annotation_property_widget.cpp
#define STRING_LABEL_ANNOTATION		tr(u8"注释")			//comment
#define STRING_ITEM_ANNOTATION		tr(u8"注释")			//annotation
#define STRING_ITEM_TEXT			tr(u8"文本")			//text
#define STRING_ITEM_ANNO_SAMPLE		tr(u8"备注")	//hello,world
#define STRING_COMBOX_VERDANA		tr(u8"verdana")		//verdana
#define STRING_COMBOX_VIJAYA		tr(u8"vijaya")		//vijaya
#define STRING_COMBOX_VRINDA		tr(u8"vrinda")		//vrinda
#define STRING_COMBOX_WEBDINGS		tr(u8"Webdings")	//Webdings
#define STRING_ITEM_TEXTALIGN		tr(u8"文本对齐")		//text align
#define STRING_COMBOX_LEFT			tr(u8"左")			//left
#define STRING_COMBOX_RIGHT			tr(u8"右")			//right
#define STRING_COMBOX_CENTER		tr(u8"中心")			//center
#define STRING_COMBOX_LEFTDOWN		tr(u8"左下")			//left down
#define STRING_COMBOX_RIGHTUPON		tr(u8"右上")			//right upon
#define STRING_ITEM_LOCATION		tr(u8"位置")			//location
#define STRING_ITEM_ORIGIN			tr(u8"原点")			//origin

//axis_property_widget.cpp
#define STRING_LABEL_AXIS				tr(u8"轴")			//axis
#define STRING_ITEM_AXIS					tr(u8"轴")			//axis
#define STRING_ITEM_DATASIZE			tr(u8"通过数据大小")	//by data size
#define STRING_ITEM_GRIDLINE			tr(u8"显示网格线")	//display grid lines
#define STRING_ITEM_LINECOLOR		tr(u8"栅格线颜色")	//gridline color
#define STRING_ITEM_GRIDWIDTH		tr(u8"网格线宽")		//grid line width
#define STRING_ITEM_THICKOFAX		tr(u8"轴的厚度")		//thickness of axis
#define STRING_ITEM_MARKSIZE		tr(u8"标注大小")		//mark size
#define STRING_ITEM_TITSIZE			tr(u8"标题大小")		//title size
#define STRING_ITEM_XAXIS				tr(u8"X轴")			//X axis
#define STRING_ITEM_MARK				tr(u8"标注")			//mark
#define STRING_COMBOX_RED			tr(u8"红色")			//red
#define STRING_COMBOX_GREEN		tr(u8"绿色")			//green
#define STRING_COMBOX_BLUE			tr(u8"蓝色")			//BLUE
#define STRING_ITEM_SHOWAXIS		tr(u8"显示轴")		//show axis
#define STRING_ITEM_TITLE				tr(u8"标题")			//title
#define STRING_ITEM_EXT_LEFT			tr(u8"向左扩展")	// extend value on left
#define STRING_ITEM_EXT_RIGHT		tr(u8"向右扩展")	// extend value on right
#define STRING_ITEM_EXT_TOP			tr(u8"向上扩展")	// extend value on top
#define STRING_ITEM_EXT_BOTTOM	tr(u8"向下扩展")	// extend value on bottom
#define STRING_ITEM_EXT_FORE		tr(u8"向前扩展")	// extend value on fore
#define STRING_ITEM_EXT_BACK		tr(u8"向后扩展")	// extend value on back
#define STRING_ITEM_MAXAXIS			tr(u8"轴的最大值")	//maximum value of axis
#define STRING_ITEM_MIMAXIS			tr(u8"轴的最小值")	//mimimun value of axis
#define STRING_ITEM_MAXAXIS			tr(u8"轴的最大值")	//maximum value of axis
#define STRING_ITEM_SHOWMARK		tr(u8"显示读数")		//show mark
#define STRING_ITEM_MINMARK			tr(u8"最小标记值")	//minimum value of mark
#define STRING_ITEM_MAXMARK		tr(u8"最大标记值")	//maximum value of mark
#define STRING_ITEM_MARKINTER		tr(u8"标志间隔增量")	//mark interval increment
#define STRING_ITEM_MARKFORMAT	tr(u8"标志的格式")	//mark format
#define STRING_ITEM_YAXIS				tr(u8"Y轴")			//y axis
#define STRING_ITEM_ZAXIS				tr(u8"Z轴")			//z axis
#define STRING_TAB_XAXIS				tr(u8"X轴")			//X axis
#define STRING_TAB_YAXIS				tr(u8"Y轴")			//y axis
#define STRING_TAB_ZAXIS				tr(u8"Z轴")			//z axis

//base_property_widget.cpp
#define STRING_ERROR							tr(u8"错误")			//Error	
#define STRING_TIP_DELETE_NODE		tr(u8"操作提示")	
#define STRING_INFO_NODE_RUNNING	tr(u8"不能删除正在运行的节点")	
#define STRING_ERROR_CIRNOALLOW	tr(u8"不允许循环")	//Circular not allowed
#define STRING_COMBOX_WHITBLACK	tr(u8"白-黑")		//white-black
#define STRING_COMBOX_REDBLUE		tr(u8"红-蓝")		//red-blue
#define STRING_COMBOX_GREENBLUE	tr(u8"海绿-中蓝")		//seagreen-mediumblue
#define STRING_COMBOX_REDPURPLE	tr(u8"红-紫")		//red-purple
#define STRING_COMBOX_BLUERED		tr(u8"蓝-红")		//blue-red

//border_property_widget.cpp
#define STRING_LABEL_BORDER				tr(u8"边界框")
#define STRING_ITEM_NOTATE				tr(u8"标注")		//notate
#define STRING_ITEM_SHOWNOTATE		tr(u8"显示标注")		//show note
#define STRING_ITEM_NOTATEFORMAT	tr(u8"标注格式")		//notate format

//color_scale_property.cpp
#define STRING_ITEM_SHOWCOLORSCA	tr(u8"显示色阶")		//show color scale
#define STRING_ITEM_PLACEDIRTION		tr(u8"放置方向")		//place direction
#define STRING_ITEM_XPOSITION			tr(u8"x坐标")		//x position
#define STRING_ITEM_YPOSITION			tr(u8"y坐标")		//y position
#define STRING_ITEM_WIDTH					tr(u8"宽度")			//width
#define STRING_ITEM_LENGTH				tr(u8"长度")			//length
#define STRING_ITEM_NOTE					tr(u8"标注")			//note
#define STRING_ITEM_NOTENUMBER		tr(u8"标注数量")		//note number
#define STRING_ITEM_ADOPTNOTE			tr(u8"备注编号")		//adopt self note//这个好像无效果
#define STRING_ITEM_SELFNOTE			tr(u8"self note")	//self note        //这个好像无效果
#define STRING_ITEM_NOTEFORMAT		tr(u8"标注格式")		//note format
#define STRING_ITEM_PREFIX					tr(u8"前缀")			//prefix
#define STRING_ITEM_SUFFIX					tr(u8"后缀")			//suffix
#define STRING_COMBOX_HORIZON		tr(u8"水平")			//horizontal
#define STRING_COMBOX_VERTICAL		tr(u8"垂直")			//vertical
#define STRING_COMBOX_FIXVALUE		tr(u8"指定位数")		//fixed value
#define STRING_COMBOX_EXPONFOR		tr(u8"指数格式")		//exponent form
#define STRING_COMBOX_NORMALPRO	tr(u8"常规格式")		//normal property

//exclusion_filter_property_widget.cpp
#define STRING_LABEL_EXCLUFILTER		tr(u8"排除过滤器")	//exclusion filter
#define STRING_ITEM_EXCLUFILTER		tr(u8"排除过滤器")	//exclusion filter
#define STRING_ITEM_FILTERSTR		tr(u8"过滤器")		//filter str
#define STRING_ITEM_SCATDIAG		tr(u8"散点图")		//scatter diagram
#define STRING_ACTION_TRANSFORM		tr(u8"变换")			//transform
#define STRING_ITEM_GRIDDING		tr(u8"网格化")			//gridding
#define STRING_ITEM_REPEFILTER		tr(u8"重复过滤器")		//repeat filter
#define STRING_ITEM_SUBSET		tr(u8"子集")
#define STRING_ITEM_CONNUMPOINT		tr(u8"连接输出")	//connect the number of points output

//gridding_property_widget.cpp
#define STRING_LABEL_MESH			tr(u8"网格化")			//mesh
#define STRING_ITEM_INPUTCOUNT		tr(u8"输入数据点数")	//input data count
#define STRING_ITEM_FIELD_INDEX		tr(u8"数据域索引")	// field index
#define STRING_ITEM_GRIDDINGOPER	tr(u8"操作")	//gridding operator
#define STRING_ITEM_POLATMETHOD		tr(u8"极化方法")		//polation method
#define STRING_ITEM_INVERDISTAN		tr(u8"反距离法")		//inverse distance
#define STRING_ITEM_KRIGING			tr(u8"克里金法")		//kriging
#define STRING_ITEM_ANISOTROPY		tr(u8"各向异性")		//anisotropy
#define STRING_COMBOX_ISOTROPY		tr(u8"各向同性")		//isotropy
#define STRING_COMBOX_GENERPROP		tr(u8"一般属性")		//general property
#define STRING_ITEM_WEIGHINDEX		tr(u8"加权指数")		//weighted index
#define STRING_ITEM_SMOOTH			tr(u8"平滑")		//smooth
#define STRING_ITEM_FUNCMODEL		tr(u8"功能模型")		//function model
#define STRING_ITEM_NUGGET			tr(u8"部分")			//nugget
#define STRING_ITEM_ABUTMENT		tr(u8"台")			//abutment
#define STRING_ITEM_RANGE			tr(u8"范围")			//range
#define STRING_ITEM_XLIMIT			tr(u8"x限定")		//x limit
#define STRING_ITEM_XMIN			tr(u8"x最小值")		//x min
#define STRING_ITEM_XMAX			tr(u8"x最大值")		//x max
#define STRING_ITEM_YLIMIT			tr(u8"y限定")		//y limit
#define STRING_ITEM_YMIN			tr(u8"y最小值")		//y min
#define STRING_ITEM_YMAX			tr(u8"y最大值")		//y max
#define STRING_ITEM_ZLIMIT			tr(u8"z限定")		//z limit
#define STRING_ITEM_ZMIN			tr(u8"z最小值")		//z min
#define STRING_ITEM_ZMAX			tr(u8"z最大值")		//z max
#define STRING_ITEM_RESOLUTION		tr(u8"分辨率")		//resolution
#define STRING_ITEM_INTERVAL		tr(u8"间隔")			//interval
#define STRING_ITEM_XINTERVAL		tr(u8"x间隔")		//x interval
#define STRING_ITEM_YINTERVAL		tr(u8"y间隔")		//y interval
#define STRING_ITEM_ZINTERVAL		tr(u8"z间隔")		//z interval
#define STRING_ITEM_SEARCH			tr(u8"搜索")			//search
#define STRING_ITEM_SEARCHTYPE		tr(u8"搜索类型")		//search type
#define STRING_COMBOX_ALLDATA		tr(u8"所有数据")	//all data
#define STRING_COMBOX_SIMPLE		tr(u8"简单")			//simple
#define STRING_COMBOX_ANTISOPHY		tr(u8"不相容")		//antisophy
#define STRING_ITEM_SEARCHRADIUS	tr(u8"搜索半径")		//search radius
#define STRING_ITEM_SEARCHELLIPSE	tr(u8"搜索椭圆")		//search ellipse
#define STRING_ITEM_XLENGTH			tr(u8"x长度")		//x length
#define STRING_ITEM_YLENGTH			tr(u8"y长度")		//y length
#define STRING_ITEM_ZLENGTH			tr(u8"z长度")		//z length
#define STRING_ITEM_MINCOUNT		tr(u8"最小数")		//min count
#define STRING_ITEM_MAXCOUNT		tr(u8"最大数")		//max count
#define STRING_ACTION_SHAPRENDER	tr(u8"形体渲染")		//shape rendering
#define STRING_ACTION_ORTHOGRAPH	tr(u8"正交图")		//orthogonal graph
#define STRING_ACTION_SUFARENDER	tr(u8"表面渲染")		//surface render
#define STRING_ACTION_HLAREAMAP		tr(u8"高低区域图")	//high and low area map
#define STRING_ACTION_SUBSET		tr(u8"子集")			//subset
#define STRING_ACTION_CONN_IN		tr(u8"输入端连接")	//connect input
#define STRING_ACTION_CONN_OUT		tr(u8"输出端连接")	//connect output

//highlowmap_property_widget.cpp
#define STRING_LABEL_HIGHLOWMAP		tr(u8"高低图")		//high low map
#define STRING_ITEM_HIGHLOWMAP		tr(u8"高低图")		//high low map
#define STRING_ITEM_DIRECTION		tr(u8"方向")			//direction
#define STRING_ITEM_SECTIONID		tr(u8"节ID")			//section id
#define STRING_ITEM_SCALEFACTOR		tr(u8"比例因子")		//scale factor
#define STRING_ITEM_RENDERINFO		tr(u8"呈现信息")		//render info
#define STRING_ITEM_DRAWSTYLE		tr(u8"绘制样式")		//draw style
#define STRING_COMBOX_SHADOWSHAPE	tr(u8"阴影形状")		//Shadow_shape
#define STRING_COMBOX_LINESHAPE		tr(u8"线条形状")		//Line_shape
#define STRING_COMBOX_SCATERSHAPE	tr(u8"散点形状")		//Scatter_shape
#define STRING_ITEM_OPACITY			tr(u8"不透明度")		//opacity
#define STRING_ITEM_OVERLAP			tr(u8"覆盖")			//overlap
#define STRING_ITEM_COLORMODUL		tr(u8"颜色调制")		//color modulation
#define STRING_COMBOX_OVERSURFACE	tr(u8"表面上叠加颜色")		//Overlay_Color_On_Surface
#define STRING_COMBOX_OVERONLY		tr(u8"仅覆盖颜色")		//OverlayColorOnly
#define STRING_COMBOX_BLENDCLOLR	tr(u8"混合表面和叠加颜色")	//BlendSurfaceandOverlayColors
#define STRING_COMBOX_COLORMAP		tr(u8"仅色映射")			//ColorMapOnly
#define STRING_ITEM_SUITEDTYPE		tr(u8"适合型")		//suited type
#define STRING_COMBOX_STRETCHSUR	tr(u8"表面拉伸")	//Stretching_On_Surface
#define STRING_ITEM_TEXTUREFILE		tr(u8"纹理文件")		//texture File
#define STRING_OPEN_TEXTUREFILE		tr(u8"打开纹理文件")
#define STRING_COMBOX_FALSE		tr(u8"否")		//false
#define STRING_COMBOX_TRUE			tr(u8"是")		//true

#define STRING_ITEM_PICTURE		tr(u8"环境图")	
#define STRING_LABEL_DXF			tr(u8"dxf图")
#define STRING_LABEL_GEOTIFF			tr(u8"TIFF图")
#define STRING_LABEL_PICTURE		tr(u8"环境图")
#define STRING_ITEM_PIC_FILE		tr(u8"文件路径")	
#define STRING_ITEM_PIC_DRAG		tr(u8"允许移动")	
#define STRING_ITEM_PIC_SCALE		tr(u8"缩放")	
#define STRING_ITEM_PIC_ADOPT		tr(u8"匹配")	
#define STRING_ITEM_PIC_RANGE		tr(u8"范围")	
#define STRING_ITEM_PIC_XSTART		tr(u8"x起点")	
#define STRING_ITEM_PIC_XEND		tr(u8"x终点")	
#define STRING_ITEM_PIC_YSTART		tr(u8"y起点")	
#define STRING_ITEM_PIC_YEND		tr(u8"y终点")	
#define STRING_ITEM_PROBE_POS		tr(u8"坐标")	
#define STRING_ITEM_CHECK_POS		tr(u8"显示")	
#define STRING_ITEM_LEFT_TOP		tr(u8"左上XY")	
#define STRING_ITEM_RBOTTOM		tr(u8"右下XY")	
#define STRING_ITEM_XLENGTH_YWIDTH		tr(u8"X长Y宽")	
#define STRING_ITEM_PIC_XOFFSET		tr(u8"x偏移")	
#define STRING_ITEM_PIC_YOFFSET		tr(u8"y偏移")	
#define STRING_ITEM_PIC_ZOFFSET		tr(u8"z偏移")	
#define STRING_ITEM_PIC_ZROTATE		tr(u8"旋转")	
#define STRING_ITEM_PIC_OPERATE		tr(u8"操作")	
#define STRING_ITEM_PIC_APPLY			tr(u8"应用")	
#define STRING_ITEM_PIC_RESET			tr(u8"重置")	
#define STRING_ITEM_PIC_ADJUST_POS	tr(u8"调节匹配")	
#define STRING_ITEM_PIC_RECT_SEL		tr(u8"选点匹配")	
#define STRING_ITEM_PIC_TIFF_POS		tr(u8"坐标匹配")	
#define STRING_ITEM_PIC_VERTEX1		tr(u8"顶点1")	
#define STRING_ITEM_PIC_VERTEX2		tr(u8"顶点2")	
#define STRING_ITEM_PIC_VERTEX3		tr(u8"顶点3")	

//isosurface_property_widget.cpp
#define STRING_LABEL_ISOSURFACE		tr(u8"等值面")		//isosurface
#define STRING_ITEM_ISOSURFACE		tr(u8"等值面")		//isosurface
#define STRING_ITEM_INPUTCOMPON		tr(u8"输入分量")		//input component
#define STRING_ITEM_CRITICALVAL		tr(u8"临界值")		//critical value
#define STRING_ITEM_CALCULATEVOL	tr(u8"计算体积")		//calculate volume
#define STRING_ITEM_VOLTOCRITICAL	tr(u8"体积>=临界")	//volume>=critical
#define STRING_ITEM_CRITICALTOVOL	tr(u8"体积<=临界")	//volume<=critical
#define STRING_ITEM_STYLESHEET		tr(u8"样式表")		//style sheet
#define STRING_COMBOX_GRADIENT		tr(u8"渐变色")			//gradient
#define STRING_COMBOX_CUSTOM		tr(u8"自定义")		//custom
#define STRING_ITEM_MATERIAL		tr(u8"材料")			//material
#define STRING_ITEM_HIGHCOLOR		tr(u8"高光颜色")	//highlight color
#define STRING_ITEM_HIGHINITENSE	tr(u8"高光强度")		//highlight intense
#define STRING_ITEM_GLOSS			tr(u8"光泽度")			//gloss

//orthogonal_property_widget.cpp
#define STRING_LABEL_ORTHOGONAL		tr(u8"正交图")	//orthogonal diagram
#define STRING_ITEM_ORTHOGONAL		tr(u8"正交")	//orthogonal
#define STRING_ITEM_ORTHOGDIA		tr(u8"正交图")	//orthogonal diagram
#define STRING_ITEM_SLICENUM		tr(u8"序号")			//slice number
#define STRING_ITEM_COLORMETHOD		tr(u8"着色法")		//color method

//repeat_filter_property_widget.cpp
#define STRING_LABEL_REPEFILTER		tr(u8"重复过滤器")		//repeat filter
#define STRING_ITEM_DATAERROR		tr(u8"数据公差")		//data error
#define STRING_ITEM_XERROR			tr(u8"x轴公差")		//x error
#define STRING_ITEM_YERROR			tr(u8"y轴公差")		//y error
#define STRING_ITEM_ZERROR			tr(u8"z轴公差")		//z error

//scatter_property_widget.cpp
#define STRING_LABEL_SCATTER			tr(u8"散点图")			//scatter
#define STRING_ITEM_SCATTER			tr(u8"散点")			//scatter
#define STRING_ITEM_DENSITY			tr(u8"密度 %")			//density
#define STRING_ITEM_SYMBOL			tr(u8"符号")			//symbol
#define STRING_COMBOX_DIAMONDS		tr(u8"方块")			//diamonds
#define STRING_COMBOX_BALL			tr(u8"球")			//ball
#define STRING_ITEM_SIZE			tr(u8"大小")			//size
#define STRING_COMBOX_GRADIENTMAP	tr(u8"渐变图谱")		//Gradient map
#define STRING_ITEM_GRADIENTCOLOR	tr(u8"渐变色")	//gradient color
#define STRING_ITEM_COLORRANGE		tr(u8"颜色范围")		//color range
#define STRING_ITEM_REDCOMPON		tr(u8"红色分量")		//red component
#define STRING_ITEM_GREENCOMPON		tr(u8"绿色分量")	//green component
#define STRING_ITEM_BLUECOMPON		tr(u8"蓝色分量")	//blue component
#define STRING_ITEM_NOTEFONT		tr(u8"标注字体")		//note font
#define STRING_ITEM_SHOWLINE		tr(u8"显示行")		//show line

//shape_render_property_widget.cpp
#define STRING_LABEL_VOLUME			tr(u8"形体渲染")			//volume
#define STRING_ITEM_COMPOCOLOR		tr(u8"着色法")	//composition coloring method
#define STRING_ITEM_INTERMETHOD		tr(u8"插值法")		//interpolation method
#define STRING_ITEM_RENDERMODE		tr(u8"渲染模式")		//render mode
#define STRING_ITEM_SAMPDISTANCE	tr(u8"采样距离")	//sample distance
#define STRING_ITEM_SCALARANGE		tr(u8"标量范围")		//scalar range
#define STRING_ITEM_COLORTYPE		tr(u8"颜色类型")		//color type
#define STRING_COMBOX_MAXMETHOD		tr(u8"最大强度法")		//max strength method
#define STRING_COMBOX_MINMETHOD		tr(u8"最小强度法")		//min strength method
#define STRING_COMBOX_ALPHAMETHOD	tr(u8"阿尔法混合法")	//alpha mixing method
#define STRING_COMBOX_AVERGMETHOD	tr(u8"平均强度法")		//average strength method
#define STRING_COMBOX_ADDMETHOD		tr(u8"附加强度法")	//additive strength method
#define STRING_COMBOX_DEFAULT		tr(u8"默认")			//default
#define STRING_COMBOX_RAYCAST		tr(u8"raycast")			//raycast
#define STRING_COMBOX_GPU			tr(u8"gpu")				//gpu
#define STRING_COMBOX_OSPRAY		tr(u8"英特儿ospray")			//ospray
#define STRING_COMBOX_NEARMETHOD	tr(u8"最近邻方法")		//nearest neighbor method
#define STRING_COMBOX_LINERMETHOD	tr(u8"线性方法")			//linear method
#define STRING_COMBOX_THREELINEAR	tr(u8"三线性")			//three linear

//slice_properth_widget.cpp
#define STRING_LABEL_SLICE			tr(u8"切片图")			//slice
#define STRING_ITEM_BASIC			tr(u8"基本")			//basic
#define STRING_ITEM_LIGHTING		tr(u8"光照")			//lighting
#define STRING_ITEM_ORI_DAT_SLICE		tr(u8"原始数据位置切片")
//source_property_widget.cpp
#define STRING_ITEM_FILEPATH		tr(u8"文件路径")			//file path
#define STRING_ITEM_DATA_COUNT		tr(u8"数据点数")	//data count
#define STRING_ITEM_FIELDS		tr(u8"数据域个数")			//fields number
#define STRING_ACTION_SACTDIAGRAM		tr(u8"散点图")	//sactter diagram
#define STRING_ACTION_VECTDIAGRAM		tr(u8"矢量图")			//vector diagram
#define STRING_ITEM_CHANGETYPE		tr(u8"更改类型")			//change tye
#define STRING_MENU_CALC		tr(u8"计算")

//vector_property_widget.cpp
#define STRING_ITEM_VECTOR			tr(u8"向量")			//vector
#define STRING_ITEM_INPUTSCAL		tr(u8"输入标量")		//input scalar
#define STRING_ITEM_FREQUENCY		tr(u8"频率")			//frequency
#define STRING_ITEM_VECSTYLE		tr(u8"向量风格")	//vector style
#define STRING_ITEM_LINEAR			tr(u8"线性")			//linear
#define STRING_ITEM_TWOARROW		tr(u8"二翼箭头")		//Two wing arrow
#define STRING_ITEM_FOURARROW		tr(u8"四翼箭头")		//Four wing arrow
#define STRING_ITEM_CROSSARROW		tr(u8"十字箭头")		//cross arrow
#define STRING_ITEM_SQUAREROOT		tr(u8"平方根")		//square root
#define STRING_ITEM_LOGARITHM		tr(u8"对数")			//logarithm
#define STRING_ITEM_MINSCALE		tr(u8"最小比例")		//min scale
#define STRING_ITEM_MAXSCALE		tr(u8"最大比例")	//max scale
#define STRING_ITEM_ARROWTATE		tr(u8"箭头比例")		//arrow rate
#define STRING_ITEM_SYMBOLSIZE		tr(u8"符号大小")	//symbol size
#define STRING_ITEM_REVERSE			tr(u8"反向")			//reverse
#define STRING_ITEM_SCALLIST		tr(u8"标量列表")		//scalar list
#define STRING_ITEM_CONNSCAL		tr(u8"连接纯标量网格")	//connecting pure scalar mesh

//transform_property_widget.cpp
#define STRING_LABEL_TRANSFORM		tr(u8"变换")		//transform 
#define STRING_ITEM_ROTATE			tr(u8"旋转")			//rotate
#define STRING_ITEM_XCOMPONT		tr(u8"x分量")		//x component
#define STRING_ITEM_YCOMPONT		tr(u8"y分量")		//y component
#define STRING_ITEM_ZCOMPONT		tr(u8"z分量")		//z component
#define STRING_ITEM_ANGLE			tr(u8"角度")			//angle
#define STRING_ITEM_TRANSFORM		tr(u8"变换")		//transform property
#define STRING_ITEM_TRANSPRO		tr(u8"变换设置")		//transform property
#define STRING_ITEM_TRANSLATE		tr(u8"平移")			//translate
#define STRING_ITEM_ISOLINEMAP		tr(u8"等值线图")	//isoline map
#define STRING_ITEM_ISOFACEMAP		tr(u8"等值面图")		//isosurface map
#define STRING_CONN_OPTDATA			tr(u8"连接输出数据")	//connect output data

//text_property_widget.cpp
#define STRING_LABEL_TEXT		tr(u8"文本")
#define STRING_ITEM_POSITION		tr(u8"位置")			//position

//surface_render_property_widget.cpp
#define STRING_LABEL_SUFARENDER	tr(u8"表面渲染")		//surface render
#define STRING_ITEM_SUFARENDER	tr(u8"表面渲染")		//surface render
#define STRING_ITEM_BLOCKNUM		tr(u8"块数量")		//block number
#define STRING_ITEM_AVERBLOCK		tr(u8"平均块数")		//average block
#define STRING_ITEM_VOLUME			tr(u8"体积")			//volume
#define STRING_ITEM_GEOMPROTY		tr(u8"几何属性")		//geometry property
#define STRING_ITEM_XCOORLIM		tr(u8"x坐标限制")		//x coordinate limit
#define STRING_ITEM_XCOORNODE		tr(u8"X坐标首节点")	//x coordinate first node
#define STRING_ITEM_XCOORLNODE		tr(u8"X坐标末节点")	//x coordinate last node
#define STRING_ITEM_SINGLSLICE		tr(u8"单片")			//single slice
#define STRING_ITEM_YCOORLIM		tr(u8"y坐标限制")		//y coordinate limit
#define STRING_ITEM_YCOORFNODE		tr(u8"y坐标首节点")		//y coordinate first
#define STRING_ITEM_YCOORLNODE		tr(u8"Y坐标末节点")	//y coordinate last node
#define STRING_ITEM_ZCOORLIM		tr(u8"z坐标限制")		//z coordinate limit
#define STRING_ITEM_ZCOORFNODE		tr(u8"z坐标的首节点")	//z coordinate first
#define STRING_ITEM_ZCOORLNODE		tr(u8"Z坐标末节点")	//z coordinate last node

//subset_property_widget.cpp
#define STRING_LABEL_SUBSET		tr(u8"子集")			//subset
#define STRING_ITEM_INPUTRESOL		tr(u8"输入分辨率")	//input resolution
#define STRING_ITEM_OPTCOMPON		tr(u8"输出分量")		//output component
#define STRING_ITEM_DATACOMPON		tr(u8"数据分量")		//data component
#define STRING_ITEM_IPTCALCUL		tr(u8"输入计算")		//input calculate
#define STRING_ITEM_XSTARTNODE		tr(u8"x首节点")		//x start node
#define STRING_ITEM_XENDNODE		tr(u8"x坐标末节点")		//x end node
#define STRING_ITEM_XSTEP			tr(u8"x步长")		//x step
#define STRING_ITEM_YSTARTNODE		tr(u8"y坐标首节点")		//y start node
#define STRING_ITEM_YENDNODE		tr(u8"y坐标末节点")		//y end node
#define STRING_ITEM_YSTEP			tr(u8"y步长")		//y step
#define STRING_ITEM_ZSTARTNODE		tr(u8"z坐标首节点")		//z start node
#define STRING_ITEM_ZENDNODE		tr(u8"z坐标末节点")		//z end node
#define STRING_ITEM_ZSTEP			tr(u8"z步长")		//z step
#define STRING_ITEM_GRAPH_OUT		tr(u8"图形输出")	//graphic output
#define STRING_ITEM_STREAMDIAG		tr(u8"流线图")		//streamline diagram
#define STRING_ITEM_CONTOUTMAP		tr(u8"等值线图")		//contour map
#define STRING_ITEM_HIGNLOWAREA		tr(u8"高低区域图")	//hign and low area diagram
#define STRING_ITEM_SLICE			tr(u8"切片")			//slice
#define STRING_ITEM_MERGE			tr(u8"合并")			//merge
#define STRING_ITEM_EXTRACEPOINT	tr(u8"提取绘制点")		//extract drawing point
#define STRING_ITEM_MATH			tr(u8"数学")			//math
#define STRING_ITEM_FILTER			tr(u8"过滤器")		//filter
#define STRING_ITEM_RESAMPLE		tr(u8"重新取样")		//resample

//threeDdettectSystem.cpp
#define STRING_ITEM_PERMENTMESS		tr(u8"")		//permanent message
#define STRING_ITEM_PROJECT			tr(u8"项目")			//project
#define STRING_ITEM_EXAMPLE			tr(u8"例子")			//example
#define STRING_ITEM_NORMALMOD		tr(u8"常规模块")	//normal module
#define STRING_MENU_VIEW			tr(u8"视图&V")			//view
#define STRING_MENU_HELP			tr(u8"帮助&H")			//help
#define STRING_ACTION_CLOSE			tr(u8"关闭")			//close
#define STRING_ACTION_CLOSEALL		tr(u8"关闭所有")		//close all
#define STRING_MENU_NEW				tr(u8"新建")			//new
#define STRING_ACTION_PROJ				tr(u8"新建项目")			//new
#define STRING_ITEM_WORKSHEET		tr(u8"工作台")		//work_sheet
#define STRING_MENU_OPEN			tr(u8"打开")			//open
#define STRING_ACTION_OPENFILE		tr(u8"文件...")		//open_file
#define STRING_ITEM_OPEN_SOURCE		tr(u8"打开源数据文件")		//open_file
#define STRING_ACTION_OPENPROJECT		tr(u8"打开项目...")		//open_project
#define STRING_ACTION_SAVE			tr(u8"保存")			//save
#define STRING_ACTION_SAVEAS			tr(u8"另存为...")		//save as
#define STRING_ACTION_SAVEALL			tr(u8"保存所有")		//save all
#define STRING_ITEM_ADOPTWIN		tr(u8"适应窗口")		//adopt window
#define STRING_ITEM_ENLARGE			tr(u8"放大")			//enlarge
#define STRING_ITEM_NARROW			tr(u8"缩小")		//narrow
#define STRING_ITEM_TRANSMODE		tr(u8"平移模式")		//translation mode
#define STRING_ITEM_ROTATEMODE		tr(u8"旋转模式")		//rotate mode
#define STRING_ITEM_COORDINATE		tr(u8"坐标")			//coordinate
#define STRING_MENU_SIXVIEW			tr(u8"六面视图")		//six view
#define STRING_ITEM_FOREGLIGHT		tr(u8"前景光")		//foreground light
#define STRING_MENU_MANAGER			tr(u8"管理器")		//manager
#define STRING_ITEM_LIANMANAGE		tr(u8"联络图管理器")	//lianluotu manager
#define STRING_ITEM_PROMANAGE		tr(u8"属性管理器")		//property manager
#define STRING_ITEM_MODMANAGE		tr(u8"模块管理器")	//module manager
#define STRING_ITEM_BORDER			tr(u8"边")			//border
#define STRING_ITEM_SLICEDIA		tr(u8"切片图")			//slice diagram
#define STRING_ITEM_CUT				tr(u8"切")			//cut
//#define STRING_ITEM_SCATDIGR		tr(u8"散点图")			//scatter digram
#define STRING_ITEM_ISOLINEDIAG		tr(u8"等值线图")			//isoline diagram
#define STRING_ITEM_ISOSURDIAG		tr(u8"等值面图")			//isosurface diagram
#define STRING_ITEM_HIANLOREA		tr(u8"高低区域图")			//high and low rea
#define STRING_ITEM_INTRODUCT		tr(u8"介绍")			//introduction
#define STRING_ITEM_ABOUT			tr(u8"关于")			//about
#define STRING_ITEM_PROJET			tr(u8"项目")			//projet
#define STRING_ITEM_EXPORT			tr(u8"导出")			//export
#define STRING_ITEM_PRINT			tr(u8"打印")			//print
#define STRING_ITEM_PASTE			tr(u8"粘贴")			//paste
#define STRING_ITEM_REVOKE			tr(u8"撤销")			//revoke
#define STRING_ITEM_REDO			tr(u8"重做")			//redo
#define STRING_ITEM_TRACK			tr(u8"轨迹")			//track
#define STRING_ITEM_ZOOM			tr(u8"缩放")			//zoom
#define STRING_ITEM_ADAPT			tr(u8"适应")			//adapt
#define STRING_ITEM_START			tr(u8"起始")			//start
#define STRING_ITEM_PERCTIVE		tr(u8"透视")			//perspective
#define STRING_ITEM_PROJECTI		tr(u8"投影")			//projection
#define STRING_ITEM_ABOVE			tr(u8"顶视图")			//above
#define STRING_ITEM_SNAPSHOT		tr(u8"快照")			//snapshot
#define STRING_ITEM_VIDEO			tr(u8"视频")			//video
#define STRING_ITEM_RESET			tr(u8"复位")			//reset
#define STRING_ITEM_CAMERA			tr(u8"摄像")			//camera
#define STRING_ITEM_ITEMTREE		tr(u8"项目树")		//item tree
#define STRING_ITEM_VECGRAPH		tr(u8"矢量图")		//vector graph
#define STRING_ITEM_SHEET			tr(u8"表")			//sheet

#define STRING_TIPS_ERROR		tr(u8"提示")
#define STRING_TIPS_NO_PROJ		tr(u8"请使用 文件|新建 菜单创建一个新项目")

//threeDdetectSystem::create_camera_properties()
#define STRING_TITLE_CAMERA			tr(u8"摄像机位置")		//camera properties
#define STRING_LABLE_CAMERA			tr(u8"摄像机:")		//Camera
#define STRING_LABLE_TARGET			tr(u8"目标:")		//Target
#define STRING_BUTTON_OK			tr(u8"确认")			//OK
#define STRING_BUTTON_CANCEL		tr(u8"取消")			//Cancel
#define STRING_TITLE_GROUPBOX		tr(u8"摄像机和目标位置(x,y,z)")			//Camera and Target Positions(x,y,z)
#define STRING_ITEM_SELECT_FILE	tr(u8"选择文件")

//threeDdetectSystem::create_CaptureVideo()
#define STRING_TITLE_SYSTEM		tr(u8"考古地理数据三维可视化处理系统")
#define STRING_TITLE_CAPTURE		tr(u8"捕获视频")		//Capture Video
#define STRING_LABLE_PATH			tr(u8"存储路径：")			
#define STRING_LABLE_FRAMERATE		tr(u8"帧速率：")			
#define STRING_LABLE_QUALITY		tr(u8"视频质量：")			
#define STRING_LABLE_TIMES			tr(u8"估计时间(秒)：")			
#define STRING_LABLE_SIZE			tr(u8"估计文件大小(MB)：")			
#define STRING_LABLE_FRAMRATE		tr(u8"估计帧数：")			
#define STRING_BUTTON_CHOOSEFILE	tr(u8"选择文件夹")			//Cancel
#define STRING_BUTTON_STARTCAPTURE	tr(u8"开始录屏")			//Cancel
#define STRING_BUTTON_STOPCAPTURE	tr(u8"停止录屏")			//Cancel
#define STRING_BUTTON_CLOSE			tr(u8"关闭")			//Cancel
#define STRING_TIP_SNAPSHOT		tr(u8"快照已在剪贴板")
//myqwidget.cpp
#define STRING_ITEM_LOAD			tr(u8"加载")			//load
#define STRING_ITEM_COMMENT			tr(u8"注释")			//comment

//MOST USED
#define STRING_INPUT				tr(u8"输入")
#define STRING_ITEM_INPUT			tr(u8"输入")			//input
#define STRING_ITEM_SOURCE			tr(u8"源")			//source
#define STRING_ITEM_BOUNDBOX		tr(u8"边界框")		//bounding box
#define STRING_COMBOX_LEFTUPON		tr(u8"左上")			//left upon
#define STRING_ITEM_RIGHTUPON		tr(u8"右上")			//right upon
#define STRING_ITEM_LEFTDOWN		tr(u8"左下")			//left down
#define STRING_COMBOX_RIGHTDOWN		tr(u8"右下")			//right down
#define STRING_ITEM_RENDER			tr(u8"渲染")			//render
#define STRING_ITEM_LINEWIDTH		tr(u8"线宽")			//line width

#define STRING_ACTION_AXIS			tr(u8"坐标轴")			//axis
#define STRING_ACTION_BOUNDBOX		tr(u8"边界框")		//bounding box
#define STRING_ACTION_ANNOTATION	STRING_ITEM_ANNOTATION
#define STRING_ACTION_ISOLINEDIAG		STRING_ITEM_ISOLINEDIAG
#define STRING_ACTION_ISOSURDIAG		STRING_ITEM_ISOFACEMAP
#define STRING_ACTION_SCATTER		tr(u8"散点图")			//scatter digram
#define STRING_ACTION_SLICE			STRING_ITEM_SLICEDIA
#define STRING_ACTION_HIANLOREA		STRING_ITEM_HIANLOREA
#define STRING_ACTION_DXF			STRING_LABEL_DXF
#define STRING_ACTION_GEOTIFF		tr(u8"地理tiff图")
#define STRING_ACTION_PICTURE		tr(u8"环境图片")
#define STRING_ACTION_LIGHTING		STRING_ITEM_LIGHTING
#define STRING_ACTION_TABEL			tr(u8"数据表")
#define STRING_ACTION_IMPORT	tr(u8"导入数据")
#define STRING_ACTION_EXPORT		tr(u8"导出数据")		//save data
#define STRING_ACTION_COPY			tr(u8"复制")			//copy
#define STRING_ACTION_PASTE			STRING_ITEM_PASTE
#define STRING_ACTION_DELETE		tr(u8"删除")			//delete
#define STRING_ACTION_NAME 			tr(u8"命名")			//name
#define STRING_ACTION_RENAME		tr(u8"重命名")		//rename
#define STRING_ACTION_TEXT			STRING_ITEM_TEXT
#define STRING_ACTION_LIANMANAGE	STRING_ITEM_LIANMANAGE
#define STRING_ACTION_PROMANAGE		STRING_ITEM_PROMANAGE
#define STRING_ACTION_MODMANAGE		STRING_ITEM_MODMANAGE
#define STRING_ACTION_TRANSMODE		STRING_ITEM_TRANSMODE
#define STRING_ACTION_ROTATEMODE		STRING_ITEM_ROTATEMODE
#define STRING_ACTION_AXES		tr(u8"坐标轴")			//coordinate
#define STRING_ACTION_ADOPTWIN		STRING_ITEM_ADOPTWIN
#define STRING_ACTION_ENLARGE			STRING_ITEM_ENLARGE
#define STRING_ACTION_NARROW			STRING_ITEM_NARROW
#define STRING_ACTION_FOREGLIGHT		STRING_ITEM_FOREGLIGHT

#define STRING_ACTION_INTRO			tr(u8"简介...")			//introduction
#define STRING_ACTION_ABOUT			tr(u8"关于...")			//about
#define STRING_ACTION_VIEW_FRONT		tr(u8"从前看")	//Front:正面
#define STRING_ACTION_VIEW_BACK			tr(u8"从后看")	//Back:背面
#define STRING_ACTION_VIEW_LEFT			tr(u8"从左看")	//Left:左面
#define STRING_ACTION_VIEW_RIGHT			tr(u8"从右看")	//Right:右面
#define STRING_ACTION_VIEW_TOP			tr(u8"从上看")	//Top:顶面
#define STRING_ACTION_VIEW_BOTTOM		tr(u8"从下看")//Bottom:底面

#define STRING_ACTION_SNAPSHOT	tr(u8"截取渲染快照")		// snapshot
#define STRING_ACTION_VIDEO			tr(u8"截取渲染视频")		// video
//#define STRING_ACTION_RESET
//#define STRING_ACTION_START
#define STRING_ACTION_CAMERA		tr(u8"摄像机位置")		//camera

#define STRING_ACTION_GRIDDING			tr(u8"网格化")
#define STRING_ACTION_SUBSET		tr(u8"子集")
#define STRING_ACTION_MESH			tr(u8"网格化")			//mesh
#define STRING_ACTION_EXCLUFILTER		tr(u8"排除过滤器")	//exclusion filter
#define STRING_ACTION_REPEATFILT		tr(u8"重复过滤器")		//repeat filter
#define STRING_ACTION_REPEFILTER		STRING_ITEM_REPEFILTER

#define STRING_ITEM_COLOR			tr(u8"单色")			//color
#define STRING_ITEM_FONT			tr(u8"字体")			//font
#define STRING_ITEM_FONT_DEFAULT		tr(u8"仿宋体")			//font
#define STRING_ITEM_FONTSIZE		tr(u8"字体大小")		//font size
#define STRING_ITEM_TYPE			tr(u8"类型")			//type
#define STRING_COMBOX_FIXED			tr(u8"固定")			//fixed
#define STRING_COMBOX_EXPONEN		tr(u8"指数")			//Exponential
#define STRING_COMBOX_GENERAL		tr(u8"一般")			//General
#define STRING_MENU_NORMAL			tr(u8"普通")			//normal
#define STRING_ITEM_SIGNNUM			tr(u8"小数位数")		//significant number
#define STRING_ITEM_CONNECTMODULE	tr(u8"连接输出模块")	//connect output module
#define STRING_ITEM_GEOMETRY		tr(u8"普通")			//geometry
#define STRING_FALSE				tr(u8"错误")			//false
#define STRING_TRUE					tr(u8"正确")			//true
#endif

#define STRING_ACTION_CURVE_SLICE	tr(u8"曲面切片")
#define STRING_LABEL_CURVE_SLICE	STRING_ACTION_CURVE_SLICE
#define STRING_CURVE_FILE	tr(u8"曲面数据文件")
