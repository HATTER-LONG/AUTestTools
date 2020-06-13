
#include "Arrow.h"
#include "DiagramItem.h"
#include "DiagramScene.h"
#include "DiagramSceneWindow.h"
#include "function/FunctionDeclAnalysis.h"
#include "spdlog/spdlog.h"
#include <QtWidgets>


const int InsertTextButton = 10;
DiagramSceneWindow::DiagramSceneWindow(QMainWindow* parent) : QMainWindow(parent)
{
    createActions();
    createToolBox();

    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 10000, 10000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)), this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)), this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(itemSelected(QGraphicsItem*)));

    view = new QGraphicsView(scene);
    createToolbars();

    auto* layout = new QHBoxLayout;
    layout->addWidget(toolBox);

    layout->addWidget(view);
    setWindowFlags(Qt::FramelessWindowHint);
    auto* diagramWidget = new QWidget;
    diagramWidget->setLayout(layout);
    this->setCentralWidget(diagramWidget);
}
void DiagramSceneWindow::initDefconf() {}
void DiagramSceneWindow::backgroundButtonGroupClicked(QAbstractButton* button)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    QList<QAbstractButton*> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton* myButton, buttons)
    {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}

void DiagramSceneWindow::buttonGroupClicked(int id)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    foreach (QAbstractButton* button, buttons)
    {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton)
    {
        scene->setMode(DiagramScene::InsertText);
    }
    else
    {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}

void DiagramSceneWindow::deleteItem()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    foreach (QGraphicsItem* item, scene->selectedItems())
    {
        if (item->type() == Arrow::Type)
        {
            scene->removeItem(item);
            auto* arrow = qgraphicsitem_cast<Arrow*>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem* item, scene->selectedItems())
    {
        if (item->type() == DiagramItem::Type)
        {
            qgraphicsitem_cast<DiagramItem*>(item)->removeArrows();
        }

        if (item->parentItem() == Q_NULLPTR)
        {
            scene->removeItem(item);
            delete item;
        }
        else
        {
            scene->removeItem(item->parentItem());
            delete item->parentItem();
        }
    }
}

void DiagramSceneWindow::pointerGroupClicked(int)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void DiagramSceneWindow::bringToFront()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = scene->selectedItems().first();
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

    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems)
    {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void DiagramSceneWindow::itemInserted(DiagramItem* item)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void DiagramSceneWindow::textInserted(QGraphicsTextItem* /*item*/)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void DiagramSceneWindow::currentFontChanged(const QFont&) { handleFontChange(); }

void DiagramSceneWindow::fontSizeChanged(const QString&) { handleFontChange(); }

void DiagramSceneWindow::sceneScaleChanged(const QString& scale)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void DiagramSceneWindow::textColorChanged()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    textAction = qobject_cast<QAction*>(sender());
    fontColorToolButton->setIcon(
        createColorToolButtonIcon(":/images/textpointer.png", qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}

void DiagramSceneWindow::lineColorChanged()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);

    lineAction = qobject_cast<QAction*>(sender());
    lineColorToolButton->setIcon(
        createColorToolButtonIcon(":/images/linecolor.png", qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
void DiagramSceneWindow::textButtonTriggered()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
void DiagramSceneWindow::fillButtonTriggered()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
void DiagramSceneWindow::lineButtonTriggered()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
void DiagramSceneWindow::handleFontChange()
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}

void DiagramSceneWindow::itemSelected(QGraphicsItem* item)
{
    spdlog::info("{}:{}:{} Call!!!", __FILE__, __FUNCTION__, __LINE__);
    auto* textItem = qgraphicsitem_cast<DiagramTextItem*>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void DiagramSceneWindow::about()
{
    QMessageBox::about(this, tr("About Diagram Scene"),
                       tr("The <b>Diagram Scene</b> example shows "
                          "use of the graphics framework."));
}

void DiagramSceneWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));
    auto* layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step), 1, 0);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 2, 0);
    //! [21]

    auto* textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    auto* textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    auto* textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 3, 0);

    layout->setRowStretch(4, 10); //行列方向增加弹簧 https://www.mycode.net.cn/language/cpp/539.html
    layout->setColumnStretch(1, 10);

    auto* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this,
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

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

void DiagramSceneWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringforward.png"), tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/images/backarrow-40px.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete-40px.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(QIcon(":/images/about-48px.png"), tr("About Tips"), this);
    aboutAction->setStatusTip(tr("About Tips"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void DiagramSceneWindow::createToolbars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    auto* validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    fontSizeCombo->setCurrentText("20");
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontSizeChanged(QString)));
    fontSizeChanged(fontSizeCombo->currentText());

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(textButtonTriggered()));

    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(":/images/fillcolor-40px.png", Qt::white));
    connect(fillColorToolButton, &QAbstractButton::clicked, this, &DiagramSceneWindow::fillButtonTriggered);

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()), this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    auto* pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    auto* linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(0);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(sceneScaleChanged(QString)));
    sceneScaleChanged(sceneScaleCombo->currentText());

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
}

QIcon DiagramSceneWindow::createColorToolButtonIcon(const QString& imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QMenu* DiagramSceneWindow::createColorMenu(const char* slot, QColor defaultColor)
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
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}

QIcon DiagramSceneWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}

QWidget* DiagramSceneWindow::createCellWidget(const QString& text, DiagramItem::DiagramType type)
{
    spdlog::info("{}:{}:{} Call text is [{}]  type is [{}]!!!", __FILE__, __FUNCTION__, __LINE__,
                 text.toStdString().c_str(), type);
    DiagramItem item(type);
    QIcon icon(item.image());

    auto* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    auto* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    auto* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
QWidget* DiagramSceneWindow::createBackgroundCellWidget(const QString& text, const QString& image)
{
    spdlog::info("{}:{}:{} Call text is [{}]  image is [{}]!!!", __FILE__, __FUNCTION__, __LINE__,
                 text.toStdString().c_str(), image.toStdString().c_str());

    auto* button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    auto* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    auto* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

void DiagramSceneWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction*>(sender());
    fillColorToolButton->setIcon(
        createColorToolButtonIcon("://images/fillcolor-40px.png", qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}

void DiagramSceneWindow::fileopen(QString filename)
{
    auto* fundeclanalysisptr = new FunctionDeclAnalysis("./test/test.cxx", "./build/compile_commands.json");


    int result = fundeclanalysisptr->StartToAnalysis();
    spdlog::info("StartToAnalysis Result is {}\n", result);

    SourceCodeErrorMessageList tmpErrorMessagevector = fundeclanalysisptr->GetErrorMessage();
    for (auto a : tmpErrorMessagevector)
    {
        spdlog::info("errorLevel[{}] && message[{}] && filepos[{}]", a.GetErrorLevel(), a.GetErrorMessage(),
                     a.GetErrorPos());
    }

    SourceCodeFunctionMessageMap tmpFunctionMessageMap = fundeclanalysisptr->GetFunctionMessage();

    DiagramItem* tmpItem = nullptr;
    for (auto a : tmpFunctionMessageMap)
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

        tmpItem = scene->createItem(DiagramItem::DiagramType::Step, point);
        std::vector<std::string> functionparam = a.second.GetFunctionParam();
        std::string showText = functionparam[0];
        showText += " ";
        showText += a.second.GetFunctionName();
        showText += "(";
        for (auto b = functionparam.begin() + 1; b != functionparam.end(); b++)
        {
            showText += *b;
            if (b != functionparam.end() - 1)
                showText += ", ";
        }
        showText += ")";

        tmpItem->setItemText(QString(showText.c_str()));

        std::vector<std::string> functioncallexpr = a.second.GetFunctionWhichCallExpr();
        DiagramItem* tmpItemcall = nullptr;
        DiagramItem* tmpItemcallLast = nullptr;
        for (auto& b : functioncallexpr)
        {
            QPointF pointf2;
            if (tmpItemcall == nullptr)
            {
                pointf2 = tmpItem->pos();
                pointf2.setY(pointf2.y() + tmpItem->boundingRect().height() + 100);
                tmpItemcallLast = tmpItem;
            }
            else
            {
                pointf2 = tmpItemcall->pos();
                pointf2.setY(pointf2.y() + tmpItemcall->boundingRect().height() + 100);
                tmpItemcallLast = tmpItemcall;
            }

            tmpItemcall = scene->createItem(DiagramItem::DiagramType::Step, pointf2);
            auto iter = tmpFunctionMessageMap.find(b);

            std::vector<std::string> functionparam = iter->second.GetFunctionParam();
            std::string showText = functionparam[0];
            showText += " ";
            showText += iter->second.GetFunctionName();
            showText += "(";
            for (auto b = functionparam.begin() + 1; b != functionparam.end(); b++)
            {
                showText += *b;
                if (b != functionparam.end() - 1)
                    showText += ", ";
            }
            showText += ")";
            tmpItemcall->setItemText(QString(showText.c_str()));

            scene->setArrow(tmpItemcallLast, tmpItemcall);
        }
    }
}