#include "gui/MainWindow.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QTableWidget>
#include <QVBoxLayout>

#include "analyzer/ShantenAnalyzer.h"
#include "core/ShantenCalculator.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("川麻牌效率向听分析助手"));
    resize(900, 700);
    buildUi();
    runAnalysis();
}

void MainWindow::buildUi() {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);

    auto* mainLayout = new QVBoxLayout(centralWidget_);

    auto* queBox = new QGroupBox(QStringLiteral("定缺选择"), centralWidget_);
    auto* queLayout = new QHBoxLayout(queBox);
    queGroup_ = new QButtonGroup(this);
    queTong_ = new QRadioButton(QStringLiteral("定缺筒"), queBox);
    queTiao_ = new QRadioButton(QStringLiteral("定缺条"), queBox);
    queWan_ = new QRadioButton(QStringLiteral("定缺万"), queBox);
    queWan_->setChecked(true);
    queGroup_->addButton(queTong_, static_cast<int>(Suit::Tong));
    queGroup_->addButton(queTiao_, static_cast<int>(Suit::Tiao));
    queGroup_->addButton(queWan_, static_cast<int>(Suit::Wan));
    queLayout->addWidget(queTong_);
    queLayout->addWidget(queTiao_);
    queLayout->addWidget(queWan_);
    queLayout->addStretch();
    mainLayout->addWidget(queBox);

    connect(queGroup_, &QButtonGroup::idClicked, this, &MainWindow::onQueSuitChanged);

    auto* poolBox = new QGroupBox(QStringLiteral("牌池（点击添加）"), centralWidget_);
    auto* poolLayout = new QGridLayout(poolBox);
    poolLayout->setSpacing(4);
    tilePool_ = poolBox;

    const QString suitLabels[] = {QStringLiteral("筒"), QStringLiteral("条"), QStringLiteral("万")};
    int buttonIndex = 0;
    for (int suit = 0; suit < 3; ++suit) {
        for (int rank = 1; rank <= 9; ++rank) {
            auto* btn = new QPushButton(QString::number(rank) + suitLabels[suit], poolBox);
            btn->setMinimumSize(48, 36);
            btn->setProperty("tileIndex", suit * 9 + (rank - 1));
            poolLayout->addWidget(btn, suit, rank - 1);
            poolButtons_.append(btn);
            connect(btn, &QPushButton::clicked, this, &MainWindow::onTilePoolClicked);
            ++buttonIndex;
            Q_UNUSED(buttonIndex);
        }
    }
    mainLayout->addWidget(poolBox);

    auto* handBox = new QGroupBox(QStringLiteral("当前手牌（点击移除）"), centralWidget_);
    auto* handOuterLayout = new QVBoxLayout(handBox);
    auto* handScroll = new QScrollArea(handBox);
    handScroll->setWidgetResizable(true);
    handView_ = new QWidget(handScroll);
    handScroll->setWidget(handView_);
    handOuterLayout->addWidget(handScroll);

    auto* handBar = new QHBoxLayout();
    statusLabel_ = new QLabel(handBox);
    clearButton_ = new QPushButton(QStringLiteral("清空"), handBox);
    handBar->addWidget(statusLabel_, 1);
    handBar->addWidget(clearButton_);
    handOuterLayout->addLayout(handBar);
    mainLayout->addWidget(handBox);

    connect(clearButton_, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    auto* resultBox = new QGroupBox(QStringLiteral("切牌分析结果"), centralWidget_);
    auto* resultLayout = new QVBoxLayout(resultBox);
    resultTable_ = new QTableWidget(0, 4, resultBox);
    resultTable_->setHorizontalHeaderLabels({
        QStringLiteral("切出牌"),
        QStringLiteral("向听数"),
        QStringLiteral("有效进张"),
        QStringLiteral("进张种类")
    });
    resultTable_->horizontalHeader()->setStretchLastSection(true);
    resultTable_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    resultTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultTable_->setAlternatingRowColors(true);
    resultLayout->addWidget(resultTable_);
    mainLayout->addWidget(resultBox, 1);
}

Suit MainWindow::currentQueSuit() const {
    return static_cast<Suit>(queGroup_->checkedId());
}

void MainWindow::onTilePoolClicked() {
    auto* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) {
        return;
    }
    if (hand_.totalCount() >= kMaxHandSize) {
        statusLabel_->setText(QStringLiteral("手牌已满 14 张，请先移除一张再添加"));
        return;
    }

    const int tileIndex = btn->property("tileIndex").toInt();
    const Tile tile = Tile::fromIndex(tileIndex);
    if (!hand_.addTile(tile)) {
        statusLabel_->setText(QStringLiteral("无法添加该牌（已达每种 4 张上限）"));
        return;
    }

    rebuildHandView();
    runAnalysis();
}

void MainWindow::onHandTileClicked() {
    auto* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) {
        return;
    }

    const int tileIndex = btn->property("tileIndex").toInt();
    hand_.removeTile(Tile::fromIndex(tileIndex));
    rebuildHandView();
    runAnalysis();
}

void MainWindow::onClearClicked() {
    hand_.clear();
    rebuildHandView();
    runAnalysis();
}

void MainWindow::onQueSuitChanged() {
    runAnalysis();
}

void MainWindow::rebuildHandView() {
    handButtons_.clear();

    if (handView_->layout()) {
        QLayoutItem* item = nullptr;
        while ((item = handView_->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete handView_->layout();
    }

    auto* layout = new QHBoxLayout(handView_);
    layout->setAlignment(Qt::AlignLeft);
    layout->setSpacing(4);

    const auto tiles = hand_.tiles();
    for (const Tile& tile : tiles) {
        auto* btn = new QPushButton(QString::fromStdString(tile.toString()), handView_);
        btn->setMinimumSize(48, 36);
        btn->setProperty("tileIndex", tile.index());
        layout->addWidget(btn);
        handButtons_.append(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onHandTileClicked);
    }
    layout->addStretch();
    updateStatusLabel();
}

void MainWindow::updateStatusLabel() {
    statusLabel_->setText(
        QStringLiteral("当前手牌：%1 / 14 张").arg(hand_.totalCount()));
}

QString MainWindow::shantenText(int shanten) const {
    return QString::fromStdString(ShantenCalculator::formatLabel(shanten));
}

void MainWindow::runAnalysis() {
    resultTable_->setRowCount(0);
    const Suit queSuit = currentQueSuit();
    const int count = hand_.totalCount();

    if (count == 0) {
        statusLabel_->setText(QStringLiteral("当前手牌：0 / 14 张 — 请从牌池选择手牌"));
        return;
    }

    if (count < kMaxHandSize) {
        ShantenAnalyzer shantenAnalyzer;
        const int shanten = shantenAnalyzer.analyze(hand_, queSuit);
        statusLabel_->setText(
            QStringLiteral("当前手牌：%1 / 14 张 — 当前向听：%2（请选满 14 张进行切牌分析）")
                .arg(count)
                .arg(shantenText(shanten)));
        return;
    }

    if (count > kMaxHandSize) {
        statusLabel_->setText(QStringLiteral("手牌数量异常"));
        return;
    }

    const std::vector<DiscardOption> options =
        discardAnalyzer_.analyzeDiscards(hand_, queSuit);

    if (options.empty()) {
        statusLabel_->setText(QStringLiteral("无法分析当前手牌"));
        return;
    }

    statusLabel_->setText(
        QStringLiteral("当前手牌：14 / 14 张 — 切牌分析如下"));

    resultTable_->setRowCount(static_cast<int>(options.size()));
    for (int row = 0; row < static_cast<int>(options.size()); ++row) {
        const DiscardOption& opt = options[static_cast<size_t>(row)];

        auto* discardItem = new QTableWidgetItem(QString::fromStdString(opt.tile.toString()));
        auto* shantenItem = new QTableWidgetItem(shantenText(opt.shanten));
        auto* tilesItem = new QTableWidgetItem(QString::number(opt.acceptingTiles));
        auto* typesItem = new QTableWidgetItem(QString::number(opt.acceptingTypes));

        tilesItem->setToolTip(QString::fromStdString(opt.acceptingDetail));

        resultTable_->setItem(row, 0, discardItem);
        resultTable_->setItem(row, 1, shantenItem);
        resultTable_->setItem(row, 2, tilesItem);
        resultTable_->setItem(row, 3, typesItem);
    }
}
