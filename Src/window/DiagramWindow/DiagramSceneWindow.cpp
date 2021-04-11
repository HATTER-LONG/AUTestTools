
#include "Arrow.h"
#include "DiagramItem.h"
#include "DiagramScene.h"
#include "DiagramSceneWindow.h"
#include "function/AnalysisMessage.h"
#include "spdlog/spdlog.h"

#include <QtWidgets>


const int InsertTextButton = 10;
DiagramSceneWindow::DiagramSceneWindow(QMainWindow* Parent)
        : QMainWindow(Parent)
{
    createActions();
    // createToolBox();
    createItemMenus();
    MyDiagramScene = new DiagramScene(ItemMenu, this);
    MyDiagramScene->setSceneRect(QRectF(0, 0, 10000, 10000));
    connect(MyDiagramScene, SIGNAL(itemInserted(DiagramItem*)), this, SLOT(itemInserted(DiagramItem*)));
    connect(MyDiagramScene, SIGNAL(textInserted(QGraphicsTextItem*)), this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(MyDiagramScene, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(itemSelected(QGraphicsItem*)));

    MyView = new QGraphicsView(MyDiagramScene);
    MyView->centerOn(0, 0);
    // createToolbars();

    auto* layout = new QHBoxLayout;
    // layout->addWidget(toolBox);

    layout->addWidget(MyView);
    setWindowFlags(Qt::FramelessWindowHint);
    auto* diagramWidget = new QWidget;
    diagramWidget->setLayout(layout);
    this->setCentralWidget(diagramWidget);
}
void DiagramSceneWindow::createItemMenus()
{
    ItemMenu = new QMenu;
    ItemMenu->addAction(SelectFuncAction);
    connect(SelectFuncAction, SIGNAL(triggered()), this, SLOT(itemSelectedToCreateSourceCode()));
    ItemMenu->addAction(ToFrontAction);
    ItemMenu->addAction(SendBackAction);
    ItemMenu->addSeparator();
    ItemMenu->addAction(DeleteAction);
}

void DiagramSceneWindow::itemSelectedToCreateSourceCode()
{
    spdlog::info("Select Item num is {}", MyDiagramScene->selectedItems().size());

    foreach (auto* item, MyDiagramScene->selectedItems())
    {
        if (item->type() == DiagramItem::Type)
        {
            spdlog::info("Function Info is {}", qgraphicsitem_cast<DiagramItem*>(item)->getItemText().toStdString().c_str());
            emit selectedFunctionInfo(qgraphicsitem_cast<DiagramItem*>(item)->getFunctionName());
        }
    }
}

void DiagramSceneWindow::backgroundButtonGroupClicked(QAbstractButton* Button)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    QList<QAbstractButton*> buttons = BackgroundButtonGroup->buttons();
    foreach (QAbstractButton* myButton, buttons)
    {
        if (myButton != Button)
            Button->setChecked(false);
    }
    QString text = Button->text();
    if (text == tr("Blue Grid"))
        MyDiagramScene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        MyDiagramScene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        MyDiagramScene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        MyDiagramScene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    MyDiagramScene->update();
    MyView->update();
}

void DiagramSceneWindow::buttonGroupClicked(int Id)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    QList<QAbstractButton*> buttons = ButtonGroup->buttons();
    foreach (QAbstractButton* button, buttons)
    {
        if (ButtonGroup->button(Id) != button)
            button->setChecked(false);
    }
    if (Id == InsertTextButton)
    {
        MyDiagramScene->setMode(DiagramScene::InsertText);
    }
    else
    {
        MyDiagramScene->setItemType(DiagramItem::DiagramType(Id));
        MyDiagramScene->setMode(DiagramScene::InsertItem);
    }
}

void DiagramSceneWindow::deleteItem()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    foreach (QGraphicsItem* item, MyDiagramScene->selectedItems())
    {
        if (item->type() == Arrow::Type)
        {
            MyDiagramScene->removeItem(item);
            auto* arrow = qgraphicsitem_cast<Arrow*>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem* item, MyDiagramScene->selectedItems())
    {
        if (item->type() == DiagramItem::Type)
        {
            qgraphicsitem_cast<DiagramItem*>(item)->removeArrows();
        }

        if (item->parentItem() == Q_NULLPTR)
        {
            MyDiagramScene->removeItem(item);
            delete item;
        }
        else
        {
            MyDiagramScene->removeItem(item->parentItem());
            delete item->parentItem();
        }
    }
}

void DiagramSceneWindow::pointerGroupClicked(int)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    MyDiagramScene->setMode(DiagramScene::Mode(PointerTypeGroup->checkedId()));
}

void DiagramSceneWindow::bringToFront()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    if (MyDiagramScene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = MyDiagramScene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems)
    {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void DiagramSceneWindow::sendToBack()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    if (MyDiagramScene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = MyDiagramScene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems)
    {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void DiagramSceneWindow::itemInserted(DiagramItem* Item)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    PointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    MyDiagramScene->setMode(DiagramScene::Mode(PointerTypeGroup->checkedId()));
    ButtonGroup->button(int(Item->diagramType()))->setChecked(false);
}

void DiagramSceneWindow::textInserted(QGraphicsTextItem* /*item*/)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    ButtonGroup->button(InsertTextButton)->setChecked(false);
    MyDiagramScene->setMode(DiagramScene::Mode(PointerTypeGroup->checkedId()));
}

void DiagramSceneWindow::currentFontChanged(const QFont&)
{
    handleFontChange();
}

void DiagramSceneWindow::fontSizeChanged(const QString&)
{
    handleFontChange();
}

void DiagramSceneWindow::sceneScaleChanged(const QString& Scale)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    double newScale = Scale.left(Scale.indexOf(tr("%"))).toDouble() / 100.0;
    QTransform oldMatrix = MyView->transform();
    MyView->resetTransform();
    MyView->translate(oldMatrix.dx(), oldMatrix.dy());
    MyView->scale(newScale, newScale);
}

void DiagramSceneWindow::textColorChanged()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    TextAction = qobject_cast<QAction*>(sender());
    FontColorToolButton->setIcon(
        createColorToolButtonIcon(":/images/textpointer.png", qvariant_cast<QColor>(TextAction->data())));
    textButtonTriggered();
}

void DiagramSceneWindow::lineColorChanged()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    LineAction = qobject_cast<QAction*>(sender());
    LineColorToolButton->setIcon(createColorToolButtonIcon(":/images/linecolor.png", qvariant_cast<QColor>(LineAction->data())));
    lineButtonTriggered();
}
void DiagramSceneWindow::textButtonTriggered()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    MyDiagramScene->setTextColor(qvariant_cast<QColor>(TextAction->data()));
}
void DiagramSceneWindow::fillButtonTriggered()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    MyDiagramScene->setItemColor(qvariant_cast<QColor>(FillAction->data()));
}
void DiagramSceneWindow::lineButtonTriggered()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    MyDiagramScene->setLineColor(qvariant_cast<QColor>(LineAction->data()));
}
void DiagramSceneWindow::handleFontChange()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    QFont font = FontCombo->currentFont();
    font.setPointSize(FontSizeCombo->currentText().toInt());
    font.setWeight(BoldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(ItalicAction->isChecked());
    font.setUnderline(UnderlineAction->isChecked());

    MyDiagramScene->setFont(font);
}

void DiagramSceneWindow::itemSelected(QGraphicsItem* Item)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    auto* textItem = qgraphicsitem_cast<DiagramTextItem*>(Item);

    QFont font = textItem->font();
    FontCombo->setCurrentFont(font);
    FontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    BoldAction->setChecked(font.weight() == QFont::Bold);
    ItalicAction->setChecked(font.italic());
    UnderlineAction->setChecked(font.underline());
}

void DiagramSceneWindow::about()
{
    QMessageBox::about(this, tr("About Diagram Scene"),
        tr("The <b>Diagram Scene</b> example shows "
           "use of the graphics framework."));
}

void DiagramSceneWindow::createToolBox()
{
    ButtonGroup = new QButtonGroup(this);
    ButtonGroup->setExclusive(false);
    connect(ButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));
    auto* layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step), 1, 0);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 2, 0);
    //! [21]

    auto* textButton = new QToolButton;
    textButton->setCheckable(true);
    ButtonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    auto* textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    auto* textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 3, 0);

    layout->setRowStretch(4, 10);   //行列方向增加弹簧 https://www.mycode.net.cn/language/cpp/539.html
    layout->setColumnStretch(1, 10);

    auto* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    BackgroundButtonGroup = new QButtonGroup(this);
    connect(BackgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this,
        SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    auto* backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"), ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"), ":/images/background2.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"), ":/images/background3.png"), 2, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"), ":/images/background4.png"), 3, 0);

    backgroundLayout->setRowStretch(4, 10);
    backgroundLayout->setColumnStretch(2, 10);

    auto* backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);

    ToolBox = new QToolBox;
    ToolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    ToolBox->setMinimumWidth(itemWidget->sizeHint().width());
    ToolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    ToolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

void DiagramSceneWindow::createActions()
{
    ToFrontAction = new QAction(QIcon(":/images/bringforward.png"), tr("Bring to &Front"), this);
    ToFrontAction->setShortcut(tr("Ctrl+F"));
    ToFrontAction->setStatusTip(tr("Bring item to front"));
    connect(ToFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    SendBackAction = new QAction(QIcon(":/images/backarrow-40px.png"), tr("Send to &Back"), this);
    SendBackAction->setShortcut(tr("Ctrl+T"));
    SendBackAction->setStatusTip(tr("Send item to back"));
    connect(SendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    DeleteAction = new QAction(QIcon(":/images/delete-40px.png"), tr("&Delete"), this);
    DeleteAction->setShortcut(tr("Delete"));
    DeleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(DeleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    BoldAction = new QAction(tr("Bold"), this);
    BoldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    BoldAction->setIcon(QIcon(pixmap));
    BoldAction->setShortcut(tr("Ctrl+B"));
    connect(BoldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    ItalicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    ItalicAction->setCheckable(true);
    ItalicAction->setShortcut(tr("Ctrl+I"));
    connect(ItalicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    UnderlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    UnderlineAction->setCheckable(true);
    UnderlineAction->setShortcut(tr("Ctrl+U"));
    connect(UnderlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    AboutAction = new QAction(QIcon(":/images/about-48px.png"), tr("About Tips"), this);
    AboutAction->setStatusTip(tr("About Tips"));
    connect(AboutAction, SIGNAL(triggered()), this, SLOT(about()));

    SelectFuncAction = new QAction(tr("Create Source Code"), this);
}

void DiagramSceneWindow::createToolbars()
{
    EditToolBar = addToolBar(tr("Edit"));
    EditToolBar->addAction(DeleteAction);
    EditToolBar->addAction(ToFrontAction);
    EditToolBar->addAction(SendBackAction);

    FontCombo = new QFontComboBox();
    connect(FontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(currentFontChanged(QFont)));

    FontSizeCombo = new QComboBox;
    FontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        FontSizeCombo->addItem(QString().setNum(i));
    auto* validator = new QIntValidator(2, 64, this);
    FontSizeCombo->setValidator(validator);
    FontSizeCombo->setCurrentText("20");
    connect(FontSizeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontSizeChanged(QString)));
    fontSizeChanged(FontSizeCombo->currentText());

    FontColorToolButton = new QToolButton;
    FontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    FontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    TextAction = FontColorToolButton->menu()->defaultAction();
    FontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    FontColorToolButton->setAutoFillBackground(true);
    connect(FontColorToolButton, SIGNAL(clicked()), this, SLOT(textButtonTriggered()));

    FillColorToolButton = new QToolButton;
    FillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    FillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    FillAction = FillColorToolButton->menu()->defaultAction();
    FillColorToolButton->setIcon(createColorToolButtonIcon(":/images/fillcolor-40px.png", Qt::white));
    connect(FillColorToolButton, &QAbstractButton::clicked, this, &DiagramSceneWindow::fillButtonTriggered);

    LineColorToolButton = new QToolButton;
    LineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    LineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    LineAction = LineColorToolButton->menu()->defaultAction();
    LineColorToolButton->setIcon(createColorToolButtonIcon(":/images/linecolor.png", Qt::black));
    connect(LineColorToolButton, SIGNAL(clicked()), this, SLOT(lineButtonTriggered()));

    TextToolBar = addToolBar(tr("Font"));
    TextToolBar->addWidget(FontCombo);
    TextToolBar->addWidget(FontSizeCombo);
    TextToolBar->addAction(BoldAction);
    TextToolBar->addAction(ItalicAction);
    TextToolBar->addAction(UnderlineAction);

    ColorToolBar = addToolBar(tr("Color"));
    ColorToolBar->addWidget(FontColorToolButton);
    ColorToolBar->addWidget(FillColorToolButton);
    ColorToolBar->addWidget(LineColorToolButton);

    auto* pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    auto* linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    PointerTypeGroup = new QButtonGroup(this);
    PointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    PointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(PointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    SceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    SceneScaleCombo->addItems(scales);
    SceneScaleCombo->setCurrentIndex(0);
    connect(SceneScaleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(sceneScaleChanged(QString)));
    sceneScaleChanged(SceneScaleCombo->currentText());

    PointerToolbar = addToolBar(tr("Pointer type"));
    PointerToolbar->addWidget(pointerButton);
    PointerToolbar->addWidget(linePointerButton);
    PointerToolbar->addWidget(SceneScaleCombo);
}

QIcon DiagramSceneWindow::createColorToolButtonIcon(const QString& ImageFile, QColor Color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(ImageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), Color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QMenu* DiagramSceneWindow::createColorMenu(const char* Slot, QColor DefaultColor)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue") << tr("yellow");
    auto* colorMenu = new QMenu(this);

    for (int i = 0; i < colors.count(); i++)
    {
        auto* action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, Slot);
        colorMenu->addAction(action);
        if (colors.at(i) == DefaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}

QIcon DiagramSceneWindow::createColorIcon(QColor Color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), Color);

    return QIcon(pixmap);
}

QWidget* DiagramSceneWindow::createCellWidget(const QString& Text, DiagramItem::DiagramType Type)
{
    spdlog::info(
        "{}:{}:{} Call text is [{}]  type is [{}]!!!", __FILE__, __FUNCTION__, __LINE__, Text.toStdString().c_str(), Type);
    DiagramItem item(Type, ItemMenu);
    QIcon icon(item.image());

    auto* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    ButtonGroup->addButton(button, int(Type));

    auto* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(Text), 1, 0, Qt::AlignCenter);

    auto* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget* DiagramSceneWindow::createBackgroundCellWidget(const QString& Text, const QString& Image)
{
    spdlog::info("{}:{}:{} Call text is [{}]  image is [{}]!!!", __FILE__, __FUNCTION__, __LINE__, Text.toStdString().c_str(),
        Image.toStdString().c_str());

    auto* button = new QToolButton;
    button->setText(Text);
    button->setIcon(QIcon(Image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    BackgroundButtonGroup->addButton(button);

    auto* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(Text), 1, 0, Qt::AlignCenter);

    auto* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

void DiagramSceneWindow::itemColorChanged()
{
    FillAction = qobject_cast<QAction*>(sender());
    FillColorToolButton->setIcon(
        createColorToolButtonIcon("://images/fillcolor-40px.png", qvariant_cast<QColor>(FillAction->data())));
    fillButtonTriggered();
}

void DiagramSceneWindow::drawResultByCodeMessage(const MyFunction::SourceCodeFunctionMessageMap& FunctionMessage,
    const MyFunction::SourceCodeErrorMessageList& /*errorMessage*/) const
{
    MyDiagramScene->clear();
    MyView->centerOn(0, 0);
    DiagramItem* tmpItem = nullptr;
    for (const auto& a : FunctionMessage)
    {
        QPointF point;
        if (tmpItem == nullptr)
        {
            point.setX(150);
            point.setY(150);
        }
        else
        {
            point = tmpItem->pos();
            point.setX(point.x() + tmpItem->boundingRect().width() + 50);
        }

        tmpItem = MyDiagramScene->createItem(DiagramItem::DiagramType::Step, point);
        const MyFunction::FunctionParamList& functionparam = a.second.getFunctionParam();
        std::string showText = functionparam[0];
        showText += " ";
        showText += a.second.getFunctionName();
        showText += "(";
        for (auto b = functionparam.begin() + 1; b != functionparam.end(); b++)
        {
            showText += *b;
            if (b != functionparam.end() - 1)
                showText += ", ";
        }
        showText += ")";

        tmpItem->setItemText(QString(showText.c_str()));
        tmpItem->setFunctionName(a.second.getFunctionName());
        const MyFunction::FunctionCallExprList& functioncallexpr = a.second.getFunctionWhichCallExpr();
        DiagramItem* tmpItemcall = nullptr;
        DiagramItem* tmpItemcallLast = nullptr;
        for (auto& b : functioncallexpr)
        {
            QPointF point_f2;
            if (tmpItemcall == nullptr)
            {
                point_f2 = tmpItem->pos();
                point_f2.setY(point_f2.y() + tmpItem->boundingRect().height() + 100);
                tmpItemcallLast = tmpItem;
            }
            else
            {
                point_f2 = tmpItemcall->pos();
                point_f2.setY(point_f2.y() + tmpItemcall->boundingRect().height() + 100);
                tmpItemcallLast = tmpItemcall;
            }

            tmpItemcall = MyDiagramScene->createItem(DiagramItem::DiagramType::Step, point_f2);
            auto iter = FunctionMessage.find(b);

            const MyFunction::FunctionParamList& functionparam = iter->second.getFunctionParam();
            std::string showText = functionparam[0];
            showText += " ";
            showText += iter->second.getFunctionName();
            showText += "(";
            for (auto b = functionparam.begin() + 1; b != functionparam.end(); b++)
            {
                showText += *b;
                if (b != functionparam.end() - 1)
                    showText += ", ";
            }
            showText += ")";
            tmpItemcall->setItemText(QString(showText.c_str()));
            tmpItemcall->setFunctionName(iter->second.getFunctionName());
            MyDiagramScene->setArrow(tmpItemcallLast, tmpItemcall);
        }
    }
}