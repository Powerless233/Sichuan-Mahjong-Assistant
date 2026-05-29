#pragma once

#include <QMainWindow>
#include <QVector>

#include "core/DiscardAnalyzer.h"
#include "core/Hand.h"

class QButtonGroup;
class QLabel;
class QPushButton;
class QRadioButton;
class QTableWidget;
class QVBoxLayout;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onTilePoolClicked();
    void onHandTileClicked();
    void onClearClicked();
    void onQueSuitChanged();
    void runAnalysis();

private:
    void buildUi();
    void rebuildHandView();
    void updateStatusLabel();
    QString shantenText(int shanten) const;
    Suit currentQueSuit() const;

    Hand hand_;
    DiscardAnalyzer discardAnalyzer_;

    QWidget* centralWidget_ = nullptr;
    QButtonGroup* queGroup_ = nullptr;
    QRadioButton* queTong_ = nullptr;
    QRadioButton* queTiao_ = nullptr;
    QRadioButton* queWan_ = nullptr;
    QWidget* tilePool_ = nullptr;
    QWidget* handView_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QTableWidget* resultTable_ = nullptr;
    QPushButton* clearButton_ = nullptr;

    QVector<QPushButton*> poolButtons_;
    QVector<QPushButton*> handButtons_;
};
