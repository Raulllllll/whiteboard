#include "whiteboard.h"

#include <QScreen>
#include <QGuiApplication>

WhiteBoard::WhiteBoard(QWidget *parent, WhiteboardClient *client) :
    QMainWindow(parent)
{
    //初始化
//    myTcpSocket = _myTcpSocket;
    //100ms一次
//    mySendTimer = new SendTimer(0,myTcpSocket,100);
    //通告白板创建
//    mySendTimer->sendCreateWhiteboard(userID);

    //test
    // mySendTimer->sendJsonObject.insert("draw","12");
    // mySendTimer->sendJsonObject.insert("drawline",false);

    _lpress = false;//初始鼠标左键未按下
    _drawType = 0;//初始为什么都不画
    _drag = 0;//默认非拖拽模式
    _begin = pos();//拖拽的参考坐标，方便计算位移
    _openflag = 0;//初始不打开图片
    _tEdit = new QTextEdit(this);//初始化文本输入框
    _tEdit->hide();//隐藏
    _color=0;//当前画笔默认黑色(_color代表_colors中的颜色序号)
    _size=1;//当前画笔默认粗细(_size代表_sizes中的粗细序号)
    this->setFocusPolicy(Qt::StrongFocus);

    //获取设备分辨率
    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect deskRect=screen->availableGeometry() ;
    this->setGeometry(deskRect);//设置窗体大小、位置
    this->setFixedSize(this->width(),this->height());
    setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听

    //设置背景颜色
    this->setStyleSheet("background-color:white;");

    //创建工具栏
    QToolBar *tbar = addToolBar(tr("工具栏"));
    tbar->setMovable(false);//工具栏不可移动
    tbar->setIconSize(QSize(16, 16));//设置动作图标的尺寸
    tbar->setStyleSheet("background-color:rgb(199,237,204)");//背景色

//    _Rmenu = new QMenu(this);//创建右键菜单
//    _Rmenu->addAction(tr("保存  \tCtrl+S"), this, SLOT(SavePic()));//添加菜单动作
//    _Rmenu->addAction(tr("退出  \tALT+F4"), this, SLOT(close()));//添加菜单动作
//    _Rmenu->setStyleSheet("background-color:rgb(199,237,204)");//设置背景色

    QAction *openAction = new QAction(tr("&打开"), this);//打开动作
    openAction->setIcon(QIcon(":/png/images/open.png"));//图标
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));//热键
    tbar->addAction(openAction);//添加到工具栏

    QAction *saveAction = new QAction(tr("&保存"), this);//保存动作
    saveAction->setIcon(QIcon(":/png/images/save.png"));//图标
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S")));//热键
    tbar->addAction(saveAction);//添加到工具栏

    QAction *saveasAction = new QAction(tr("&另存为"), this);//保存动作
    saveasAction->setIcon(QIcon(":/png/images/saveas.png"));//图标
    saveasAction->setShortcut(QKeySequence(tr("Ctrl+ALT+S")));//热键
    tbar->addAction(saveasAction);//添加到工具栏

    QAction *lineAction = new QAction(tr("&直线"), this);//直线动作
    lineAction->setIcon(QIcon(":/png/images/line.png"));//图标
    lineAction->setShortcut(QKeySequence(tr("Ctrl+L")));//热键
    tbar->addAction(lineAction);//添加到工具栏

    QAction *linesAction = new QAction(tr("&铅笔"), this);//铅笔动作
    linesAction->setIcon(QIcon(":/png/images/lines.png"));//图标
    linesAction->setShortcut(QKeySequence(tr("Ctrl+P")));//热键
    tbar->addAction(linesAction);//添加到工具栏

    QAction *rectAction = new QAction(tr("&矩形"), this);//矩形动作
    rectAction->setIcon(QIcon(":/png/images/rect.png"));//图标
    rectAction->setShortcut(QKeySequence(tr("Ctrl+R")));//热键
    tbar->addAction(rectAction);

    QAction *ellipseAction = new QAction(tr("&椭圆"), this);//椭圆动作
    ellipseAction->setIcon(QIcon(":/png/images/ellipse.png"));//图标
    ellipseAction->setShortcut(QKeySequence(tr("Ctrl+E")));//热键
    tbar->addAction(ellipseAction);

    QAction *textAction = new QAction(tr("&文字"), this);//文字动作
    textAction->setIcon(QIcon(":/png/images/texts.png"));//图标
    textAction->setShortcut(QKeySequence(tr("Ctrl+T")));//热键
    tbar->addAction(textAction);

    //橡皮擦功能
    QAction *eraseAction = new QAction(tr("&橡皮"), this);//橡皮擦动作
    eraseAction->setIcon(QIcon(":/png/images/eraser.png"));//图标
    eraseAction->setShortcut(QKeySequence(tr("Ctrl+Y")));//热键
    tbar->addAction(eraseAction);

    //创建颜色选择工具栏
    QToolBar *tbar_color = addToolBar(tr("颜色工具栏"));
    tbar_color->setMovable(false);//工具栏不可移动
    tbar_color->setIconSize(QSize(16, 16));//设置动作图标的尺寸
    tbar_color->setStyleSheet("background-color:rgb(199,237,204)");//背景色

    QAction *color0Action = new QAction(tr("&黑色"), this);//黑色
    color0Action->setIcon(QIcon(":/png/images/color0.png"));//图标
    tbar_color->addAction(color0Action);

    QAction *color1Action = new QAction(tr("&红色"), this);//红色
    color1Action->setIcon(QIcon(":/png/images/color1.png"));//图标
    tbar_color->addAction(color1Action);

    QAction *color2Action = new QAction(tr("&绿色"), this);//绿色
    color2Action->setIcon(QIcon(":/png/images/color2.png"));//图标
    tbar_color->addAction(color2Action);

    QAction *color3Action = new QAction(tr("&蓝色"), this);//蓝色
    color3Action->setIcon(QIcon(":/png/images/color3.png"));//图标
    tbar_color->addAction(color3Action);

    QAction *color4Action = new QAction(tr("&黄色"), this);//黄色
    color4Action->setIcon(QIcon(":/png/images/color4.png"));//图标
    tbar_color->addAction(color4Action);

    QAction *color5Action = new QAction(tr("&洋红色"), this);//洋红色
    color5Action->setIcon(QIcon(":/png/images/color5.png"));//图标
    tbar_color->addAction(color5Action);

    QAction *color6Action = new QAction(tr("&青色"), this);//青色
    color6Action->setIcon(QIcon(":/png/images/color6.png"));//图标
    tbar_color->addAction(color6Action);

    QAction *color7Action = new QAction(tr("&白色"), this);//白色
    color7Action->setIcon(QIcon(":/png/images/color7.png"));//图标
    tbar_color->addAction(color7Action);

    //创建线条粗细选择工具栏
    QToolBar *tbar_size = addToolBar(tr("线条粗细工具栏"));
    tbar_size->setMovable(false);//工具栏不可移动
    tbar_size->setIconSize(QSize(16, 16));//设置动作图标的尺寸
    tbar_size->setStyleSheet("background-color:rgb(199,237,204)");//背景色

    QAction *size0Action = new QAction(tr("&粗细0"), this);//粗细0
    size0Action->setIcon(QIcon(":/png/images/size0.png"));//图标
    tbar_size->addAction(size0Action);

    QAction *size1Action = new QAction(tr("&粗细1"), this);//粗细1
    size1Action->setIcon(QIcon(":/png/images/size1.png"));//图标
    tbar_size->addAction(size1Action);

    QAction *size2Action = new QAction(tr("&粗细2"), this);//粗细2
    size2Action->setIcon(QIcon(":/png/images/size2.png"));//图标
    tbar_size->addAction(size2Action);

    QAction *size3Action = new QAction(tr("&粗细3"), this);//粗细3
    size3Action->setIcon(QIcon(":/png/images/size3.png"));//图标
    tbar_size->addAction(size3Action);

    QAction *size4Action = new QAction(tr("&粗细4"), this);//粗细4
    size4Action->setIcon(QIcon(":/png/images/size4.png"));//图标
    tbar_size->addAction(size4Action);

    //连接信号与槽函数
    connect(linesAction, SIGNAL(triggered()), this, SLOT(Lines()));
    connect(rectAction, SIGNAL(triggered()), this, SLOT(Rects()));
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(Ellipses()));
    connect(lineAction, SIGNAL(triggered()), this, SLOT(Line()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    connect(saveasAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPic()));
    connect(textAction, SIGNAL(triggered()), this, SLOT(Texts()));
//    connect(_tEdit, SIGNAL(textChanged()), this, SLOT(AddTexts()));
    connect(eraseAction, SIGNAL(triggered()), this, SLOT(Erase()));

    connect(color0Action, SIGNAL(triggered()), this, SLOT(Color0()));
    connect(color1Action, SIGNAL(triggered()), this, SLOT(Color1()));
    connect(color2Action, SIGNAL(triggered()), this, SLOT(Color2()));
    connect(color3Action, SIGNAL(triggered()), this, SLOT(Color3()));
    connect(color4Action, SIGNAL(triggered()), this, SLOT(Color4()));
    connect(color5Action, SIGNAL(triggered()), this, SLOT(Color5()));
    connect(color6Action, SIGNAL(triggered()), this, SLOT(Color6()));
    connect(color7Action, SIGNAL(triggered()), this, SLOT(Color7()));

    connect(size0Action, SIGNAL(triggered()), this, SLOT(Size0()));
    connect(size1Action, SIGNAL(triggered()), this, SLOT(Size1()));
    connect(size2Action, SIGNAL(triggered()), this, SLOT(Size2()));
    connect(size3Action, SIGNAL(triggered()), this, SLOT(Size3()));
    connect(size4Action, SIGNAL(triggered()), this, SLOT(Size4()));

}

void WhiteBoard::paintEvent(QPaintEvent *)
{
    if(_openflag == 0)//不是打开图片的，每一次新建一个空白的画布
    {
        _pixmap = QPixmap(size());//新建pixmap
        _pixmap.fill(Qt::white);//背景色填充为白色A
    }
    QPixmap pix = _pixmap;//以_pixmap作为画布
    QPainter p(&pix);//将_pixmap作为画布
    unsigned int i1=0,i2=0,i3=0,i4=0,i5=0;//各种图形的索引

    for(int c = 0;c<_shape.size();++c)//控制用户当前所绘图形总数
    {
        if(_shape.at(c) == 6){
            p.setPen(QPen(Qt::white,_sizes.at(_shape_size.at(c))));
        }
        else p.setPen(QPen(_colors.at(_shape_colors.at(c)),_sizes.at(_shape_size.at(c))));
        if(_shape.at(c) == 1 || _shape.at(c) == 6 )//线条或者橡皮擦
        {
              const QVector<QPoint>& line = _lines.at(i1++);//取出一条线条
              for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
              {
                  p.drawLine(line.at(j), line.at(j+1));
              }
        }
        else if(_shape.at(c) == 2)//矩形
        {
             p.drawRect(_rects.at(i2++));
        }
        else if(_shape.at(c) == 3)//椭圆
        {
            p.drawEllipse(_ellipse.at(i3++));
        }
        else if(_shape.at(c) == 4)//直线
        {
            p.drawLine(_line.at(i4).topLeft(),_line.at(i4).bottomRight());
            i4++;
        }
        else if(_shape.at(c) == 5)//文本
        {
            p.drawText(_tpoint.at(i5),_text.at(i5));
            i5++;
        }
    }
    unsigned int i1_rt=0,i2_rt=0,i3_rt=0,i4_rt=0,i5_rt=0;//各种图形的索引

    for(int c_rt = 0;c_rt<_shape_remote.size();++c_rt)//控制远程用户当前所绘图形总数
    {
        if(_shape_remote.at(c_rt) == 6){
            p.setPen(QPen(Qt::white,_sizes.at(_shape_size_remote.at(c_rt))));
        }
        else p.setPen(QPen(_colors.at(_shape_colors_remote.at(c_rt)),_sizes.at(_shape_size_remote.at(c_rt))));
        if(_shape_remote.at(c_rt) == 1 || _shape_remote.at(c_rt) == 6)//线条
        {
              const QVector<QPoint>& line = _lines_remote.at(i1_rt++);//取出一条线条
              for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
              {
                  p.drawLine(line.at(j), line.at(j+1));
              }
        }
        else if(_shape_remote.at(c_rt) == 2)//矩形
        {
             p.drawRect(_rects_remote.at(i2_rt++));
        }
        else if(_shape_remote.at(c_rt) == 3)//椭圆
        {
            p.drawEllipse(_ellipse_remote.at(i3_rt++));
        }
        else if(_shape_remote.at(c_rt) == 4)//直线
        {
            p.drawLine(_line_remote.at(i4_rt).topLeft(),_line_remote.at(i4_rt).bottomRight());
            i4_rt++;
        }
        else if(_shape_remote.at(c_rt) == 5)//文本
        {
            p.drawText(_tpoint_remote.at(i5_rt),_text_remote.at(i5_rt));
            i5_rt++;
        }
    }
    p.end();
    p.begin(this);//将当前窗体作为画布
    p.drawPixmap(0,0, pix);//将pixmap画到窗体
}

void WhiteBoard::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        if(_drawType == 1 || _drawType == 6)//线条(铅笔)
        {
            _lpress = true;//左键按下标志
            QVector<QPoint> line;//鼠标按下，新的线条开始
            _lines.append(line);//将新线条添加到线条集合
            QVector<QPoint>& lastLine = _lines.last();//拿到新线条
            lastLine.append(e->pos());//记录鼠标的坐标(新线条的开始坐标)
            if(_drawType == 1){
                _shape.append(1);
                _shape_size.append(_size);
                mySendTimer->sendLines(false,true,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height(),_color,_size);
            }
            else{
                _shape.append(6);
                _shape_size.append(5);
                mySendTimer->sendLines(true,true,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height(),_color,5);
            }
            _shape_colors.append(_color);

        }
        else if(_drawType == 2)//矩形
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，矩形开始
                _rects.append(rect);//将新矩形添加到矩形集合
                QRect& lastRect = _rects.last();//拿到新矩形
                lastRect.setTopLeft(e->pos());//记录鼠标的坐标(新矩形的左上角坐标)
                mySendTimer->sendRects(true,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height(), _color, _size);
                _shape.append(2);
                _shape_colors.append(_color);
                _shape_size.append(_size);
            }
            else if(_rects.last().contains(e->pos()))//拖拽模式、如果在矩形内按下
            {
                _begin = e->pos();//记录拖拽开始的坐标位置,方便计算位移

            }

        }
        else if(_drawType == 3)//椭圆
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，椭圆开始
                _ellipse.append(rect);//将新椭圆添加到椭圆集合
                QRect& lastEllipse = _ellipse.last();//拿到新椭圆
                lastEllipse.setTopLeft(e->pos());//记录鼠标的坐标(新椭圆的左上角坐标)
                mySendTimer->sendEllipse(true,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height(), _color, _size);
                _shape.append(3);
                _shape_colors.append(_color);
                _shape_size.append(_size);
            }
            else if(_ellipse.last().contains(e->pos()))//如果在椭圆内按下
            {
                _begin = e->pos();//记录拖拽开始的坐标位置

            }
        }
        else if(_drawType == 4)//直线
        {
            _lpress = true;//左键按下标志
            QRect rect;//鼠标按下，直线一端开始
            _line.append(rect);//将新直线添加到直线集合
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setTopLeft(e->pos());//记录鼠标的坐标(新直线开始一端坐标)
            mySendTimer->sendLine(true,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height(), _color, _size);
            _shape.append(4);
            _shape_colors.append(_color);
            _shape_size.append(_size);
        }
        else if(_drawType == 5)//文字
        {
            update();//触发窗体重绘
            QPoint p;//鼠标按下，文字开始
            _tpoint.append(p);//将文字坐标添加到文字位置集合
            QPoint& lastp = _tpoint.last();//拿到新文字
            lastp = e->pos();//记录鼠标的坐标
            _tEdit->setGeometry(lastp.x()-6,lastp.y()-17,90,90);//设置文本框的位置、大小

            _tEdit->show();//显示文本输入框
            _text.append("");//添加文本到文本集合
            mySendTimer->sendText(true,lastp.x()/(double)this->height(),lastp.y()/(double)this->height(),"", _color, _size);
            _tEdit->clear();//因为全局只有一个，所以使用之前要清空
            _shape.append(5);
            _shape_colors.append(_color);
            _shape_size.append(_size);
        }

    }
}

void WhiteBoard::AddTexts()//当输入框文本改变时调用
{
    QString& last = _text.last();//拿到最后一个文本
    last = _tEdit->toPlainText();//将输入框文本作为文本
    mySendTimer->sendText(false,-1,-1,last);
}



void WhiteBoard::mouseMoveEvent(QMouseEvent *e)
{
    if(_drag && ((_drawType == 2 && _rects.last().contains(e->pos()))
            || (_drawType == 3 && _ellipse.last().contains(e->pos()) )))
    {
        setCursor(Qt::SizeAllCursor);//拖拽模式下，并且在拖拽图形中，将光标形状改为十字
    }
    else
    {
        setCursor(Qt::ArrowCursor);//恢复原始光标形状
        _drag = 0;
    }

    if(_lpress)
    {
        if(_drawType == 1 || _drawType == 6)//铅笔画线或者橡皮擦
        {
            if(_lines.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = _lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录鼠标的坐标(线条的轨迹)
            mySendTimer->sendLines(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
            update();//触发窗体重绘
        }
        else if(_drawType == 2)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastRect = _rects.last();//拿到新矩形
                lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标
                mySendTimer->sendRects(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
            }
            else//拖拽模式
            {
                QRect& lastRect = _rects.last();//拿到最后添加的矩形
                if(lastRect.contains(e->pos()))//在矩形的内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastRect = lastRect.adjusted(dx,dy,dx,dy);//更新矩形的位置
                    mySendTimer->sendMove(2,dx/(double)this->height(),dy/(double)this->height());
                    _begin = e->pos();//刷新拖拽点起始坐标
                }

            }
            update();//触发窗体重绘

        }
        else if(_drawType == 3)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastEllipse = _ellipse.last();//拿到新椭圆
                lastEllipse.setBottomRight(e->pos());//更新椭圆的右下角坐标)
                mySendTimer->sendEllipse(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());

            }
            else//拖拽
            {
                QRect& lastEllipse = _ellipse.last();//拿到最后添加的矩形
                if(lastEllipse.contains(e->pos()))//在椭圆内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastEllipse = lastEllipse.adjusted(dx,dy,dx,dy);
                    mySendTimer->sendMove(3,dx/(double)this->height(),dy/(double)this->height());
                    _begin = e->pos();//刷新拖拽点起始坐标
                }

            }
            update();//触发窗体重绘
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setBottomRight(e->pos());//更新直线另一端)
            mySendTimer->sendLine(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
            update();//触发窗体重绘
        }
    }


}

void WhiteBoard::mouseReleaseEvent(QMouseEvent *e)
{
    if(_lpress)
    {
        if(_drawType == 1 || _drawType == 6)
        {
            QVector<QPoint>& lastLine = _lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录线条的结束坐标
            mySendTimer->sendLines(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
            _lpress = false;//标志左键释放
        }
        else if(_drawType == 2)
        {
            QRect& lastRect = _rects.last();//拿到新矩形
            if(!_drag)
            {
                lastRect.setBottomRight(e->pos());//不是拖拽时，更新矩形的右下角坐标)
                mySendTimer->sendRects(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
                //刚画完矩形，将光标设置到新矩形的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastRect.width()/2,this->cursor().pos().y()-lastRect.height()/2);
                _drag = 1;

            }
            _lpress = false;

        }
        else if(_drawType == 3)
        {
            QRect& lastEllipse = _ellipse.last();//拿到新椭圆
            if(!_drag)
            {
                lastEllipse.setBottomRight(e->pos());//不是拖拽时，更新椭圆的右下角坐标)
                mySendTimer->sendEllipse(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
                //刚画完椭圆，将光标设置到新椭圆的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastEllipse.width()/2,this->cursor().pos().y()-lastEllipse.height()/2);
                _drag = 1;

            }
            _lpress = false;
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//拿到新矩形
            lastLine.setBottomRight(e->pos());//更新矩形的右下角坐标)
            mySendTimer->sendLine(false,e->pos().x()/(double)this->height(),e->pos().y()/(double)this->height());
            _lpress = false;

        }
    }
}

void WhiteBoard::Lines()
{
    _drawType = 1;//铅笔
    _tEdit->hide();//文本框隐藏
}

void WhiteBoard::Line()
{
    _drawType = 4;//直线
    _tEdit->hide();
}

void WhiteBoard::Rects()
{
    _drawType = 2;//矩形
    _tEdit->hide();

}

void WhiteBoard::Ellipses()
{
    _drawType = 3;//椭圆
    _tEdit->hide();
}

void WhiteBoard::Texts()
{
    _drawType = 5;//文字
}

void WhiteBoard::Erase()
{
    _drawType = 6;//橡皮擦
    _tEdit->hide();
}

//颜色设置
void WhiteBoard::Color0()
{
    _color=0;//黑色
}

void WhiteBoard::Color1()
{
    _color=1;//红色
}

void WhiteBoard::Color2()
{
    _color=2;//绿色
}

void WhiteBoard::Color3()
{
    _color=3;//蓝色
}

void WhiteBoard::Color4()
{
    _color=4;//黄色
}

void WhiteBoard::Color5()
{
    _color=5;//洋红色
}

void WhiteBoard::Color6()
{
    _color=6;//青色
}

void WhiteBoard::Color7()
{
    _color=7;//白色
}

//线条粗细设置
void WhiteBoard::Size0()
{
    _size=0;
}

void WhiteBoard::Size1()
{
    _size=1;
}

void WhiteBoard::Size2()
{
    _size=2;
}

void WhiteBoard::Size3()
{
    _size=3;
}

void WhiteBoard::Size4()
{
    _size=4;
}

void WhiteBoard::SavePic()
{
    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.jpg", "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(0,30,size().width(),size().height()-30)).save(fileName);//不包含工具栏
    }
}

void WhiteBoard::OpenPic()
{
    //弹出文件打开对话框
    QString picPath = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.jpg *.png)");
    if(!picPath.isEmpty())//用户选择了文件
    {
        QPixmap pix;
        pix.load(picPath);//加载图片
        QPainter p(&_pixmap);
        p.drawPixmap(0,30,pix);//添加工具栏的空间
        _openflag = 1;//设置文件打开标志

        //传递给远程打开
        mySendTimer->sendPic(pixmapTostring(pix));
        update();//触发窗体重绘，将图片画到窗体
    }
}

void WhiteBoard::remoteOpenpic(QString pic){
    qDebug()<<"收到远程图片";
    QPixmap pix = stringTopixmap(pic);
    QPainter p(&_pixmap);
    p.drawPixmap(0,30,pix);//添加工具栏的空间
    _openflag = 1;//设置文件打开标志
    update();
}

QString WhiteBoard::pixmapTostring( const QPixmap& pixmap )
{
  QByteArray byte_array;
  QDataStream data_stream(&byte_array,QIODevice::WriteOnly);
  data_stream<<pixmap;
  QString res = QString::fromLocal8Bit(byte_array.toBase64());
  return res;
}

QPixmap  WhiteBoard::stringTopixmap( const QString& pic )
{
  QByteArray byte_array = QByteArray::fromBase64(pic.toLocal8Bit());
  QPixmap pix;
  QDataStream data_stream(&byte_array,QIODevice::ReadOnly);
  data_stream>>pix;
  return pix;
}

//void WhiteBoard::contextMenuEvent(QContextMenuEvent *)  //右键菜单事件
//{
//    _Rmenu->exec(cursor().pos());//在光标位置弹出菜单
//}

//void WhiteBoard::keyPressEvent(QKeyEvent *e) //按键事件
//{
//     //Ctrl+Z撤销
//     if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier)
//     {
//         qDebug()<<"撤销操作";
//         if(_shape.size()>0)
//         {
//             switch(_shape.last())
//             {
//                 case 1: _lines.pop_back();
//                         break;
//                 case 6: _lines.pop_back();
//                         break;
//                 case 2: _rects.pop_back();
//                         break;
//                 case 3: _ellipse.pop_back();
//                         break;
//                 case 4: _line.pop_back();
//                         break;
//                 case 5:{
//                    _text.pop_back();
//                    _tpoint.pop_back();
//                    break;
//             }
//             default:
//                 break;
//             }
//             _shape.pop_back();
//             _drag = 0;//设置为非拖拽模式
//             mySendTimer->sendUndo();
//             update();
//         }
//     }
//     else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)//保存
//     {
//        SavePic();//Ctrl+S保存
//     }
//}

void WhiteBoard::remoteUndo(){
    qDebug()<<"远程撤销操作";
    if(_shape_remote.size()>0)
    {
        switch(_shape_remote.last())
        {
            case 1: _lines_remote.pop_back();
                    break;
            case 6: _lines.pop_back();
                    break;
            case 2: _rects_remote.pop_back();
                    break;
            case 3: _ellipse_remote.pop_back();
                    break;
            case 4: _line_remote.pop_back();
                    break;
            case 5:{
                    _text_remote.pop_back();
                    _tpoint_remote.pop_back();
                    break;
                }
        default:
            break;
        }
        _shape_remote.pop_back();
        update();
    }
}

void WhiteBoard::remoteLinesAppend(bool isEraser, bool isNewLine, double x, double y, int color ,int size){
    if(isNewLine){
        QVector<QPoint> line;//接受到新的线条
        _lines_remote.append(line);//将新线条添加到线条集合
        QVector<QPoint>& lastLine = _lines_remote.last();//拿到新线条
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastLine.append(p);//记录鼠标的坐标(新线条的开始坐标)
        if (!isEraser) _shape_remote.append(1);
        else _shape_remote.append(6);
        _shape_colors_remote.append(color);
        _shape_size_remote.append(size);

    }
    else{
        if(_lines_remote.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = _lines_remote.last();//最后添加的线条，就是最新画的
            QPoint p;
            p.setX(x*this->height());
            p.setY(y*this->height());
            lastLine.append(p);//记录鼠标的坐标(线条的轨迹)
            update();
    }
}

void WhiteBoard::remoteRectsAppend(bool isNewRect, double x, double y, int color, int size){
    if(isNewRect){
        QRect rect;//鼠标按下，矩形开始
        _rects_remote.append(rect);//将新矩形添加到矩形集合
        QRect& lastRect = _rects_remote.last();//拿到新矩形
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastRect.setTopLeft(p);//记录鼠标的坐标(新矩形的左上角坐标)
        _shape_remote.append(2);
        _shape_colors_remote.append(color);
        _shape_size_remote.append(size);
    }
    else{

        QRect& lastRect = _rects_remote.last();//拿到新矩形
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastRect.setBottomRight(p);//更新矩形的右下角坐标
        update();

    }

}

void WhiteBoard::remoteEllipseAppend(bool isNewEllipse, double x, double y, int color, int size){
    if(isNewEllipse){
        QRect rect;//鼠标按下，椭圆开始
        _ellipse_remote.append(rect);//将新椭圆添加到椭圆集合
        QRect& lastEllipse = _ellipse_remote.last();//拿到新椭圆
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastEllipse.setTopLeft(p);//记录鼠标的坐标(新椭圆的左上角坐标)
        _shape_remote.append(3);
        _shape_colors_remote.append(color);
        _shape_size_remote.append(size);
    }
    else{
        QRect& lastEllipse = _ellipse_remote.last();//拿到新椭圆
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastEllipse.setBottomRight(p);//更新椭圆的右下角坐标)
        update();
    }
}

void WhiteBoard::remoteLineAppend(bool isNewLine, double x, double y, int color, int size){
    if(isNewLine){
        QRect rect;//鼠标按下，直线一端开始
        _line_remote.append(rect);//将新直线添加到直线集合
        QRect& lastLine = _line_remote.last();//拿到新直线
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastLine.setTopLeft(p);//记录鼠标的坐标(新直线开始一端坐标)
        _shape_remote.append(4);
        _shape_colors_remote.append(color);
        _shape_size_remote.append(size);
    }
    else{
        QRect& lastLine = _line_remote.last();//拿到新直线
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        lastLine.setBottomRight(p);//更新直线另一端)
        update();//触发窗体重绘
    }
}

void WhiteBoard::remoteTextAppend(bool isNewText, double x, double y, QString text, int color, int size){
    if(isNewText){
        QPoint p;
        p.setX(x*this->height());
        p.setY(y*this->height());
        _tpoint_remote.append(p);
        _text_remote.append(text);
        _shape_remote.append(5);
        _shape_colors_remote.append(color);
        _shape_size_remote.append(size);
    }
    else{
        QString& last = _text_remote.last();//拿到最后一个文本
        last = text;//将输入框文本作为文本
        update();
    }
}

void WhiteBoard::remoteMove(int type, double dx, double dy){
    qDebug()<<"收到移动操作";
    if(type == 2){
        QRect& lastRect = _rects_remote.last();//拿到最后添加的矩形
        lastRect = lastRect.adjusted(dx*this->height(),dy*this->height(),dx*this->height(),dy*this->height());//更新矩形的位置
        update();
    }
    else if(type ==3){
        QRect& lastEllipse = _ellipse_remote.last();//拿到最后添加的矩形
        lastEllipse = lastEllipse.adjusted(dx*this->height(),dy*this->height(),dx*this->height(),dy*this->height());
        update();
    }
    else{
        qDebug()<<"收到不正确type的move";
    }
}

//void WhiteBoard::createWhiteboard(int user_id) {
//    client->send_create_whiteboard_request(user_id);
//}

WhiteBoard::~WhiteBoard()
{
}
