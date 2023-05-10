#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QAbstractItemModel>
#include <QSpinBox>
#include <QPushButton>
#include "model.h"
#include "ui_mainwindow.h"



namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0)  :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        auto definitionModel = new DefinitionModel(this);
        ui->definitionView->setModel(definitionModel);

        connect(ui->definitionView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, [=]
        {
            update();
        }, Qt::QueuedConnection);

        connect(ui->addDefinition, &QPushButton::pressed, this, [this]
        {
            auto* model = ui->definitionView->model();
            model->insertRow(model->rowCount());

            ui->definitionView->update();
            ui->definitionView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
        });
        connect(ui->removeDefinition, &QPushButton::pressed, this, [this]
        {
            auto* model = ui->definitionView->model();
            if (model->rowCount() > 0)
            {
                const int row = model->rowCount() - 1;
                for (const auto& widget : steps[row])
                {
                    unmap(widget);
                }
                steps.erase(steps.find(row));
                model->removeRow(row);
            }
        });

        auto getCurrentDefinition = [this] { return ui->definitionView->currentIndex().row(); };
        map(ui->definitionInt, definitionModel, DefinitionModel::E_INT_1, getCurrentDefinition);
        map(ui->definitionBool, definitionModel, DefinitionModel::E_BOOL_1, getCurrentDefinition);

        connect(ui->addStep, &QPushButton::pressed, this, [=]
        {
            if (getCurrentDefinition() == -1) return;
            auto widget = new QWidget(this);
            auto layout = new QHBoxLayout(widget);
            auto checkBox = new QCheckBox(widget);
            layout->addWidget(checkBox);
            auto spinBox = new QSpinBox(widget);
            layout->addWidget(spinBox);
            auto removeButton = new QPushButton(widget);
            removeButton->setText("remove");
            layout->addWidget(removeButton);
            ui->rightLayout->addWidget(widget);

            const int currentDefinition = getCurrentDefinition();
            steps[currentDefinition].push_back(widget);

            auto model = definitionModel->data(definitionModel->index(currentDefinition, DefinitionModel::E_STEPS), Qt::DisplayRole).value<QAbstractItemModel*>();
            model->setParent(widget);
            const int rowCount = model->rowCount();
            model->insertRow(rowCount);

            auto getRow = [=]
            {
                auto it = std::find(steps[currentDefinition].begin(), steps[currentDefinition].end(), widget);
                return std::distance(steps[currentDefinition].begin(), it);
            };
            map(checkBox, model, StepModel::E_BOOL_1, getRow);
            map(spinBox, model, StepModel::E_INT_1, getRow);

            connect(ui->definitionView->selectionModel(), &QItemSelectionModel::currentRowChanged, widget, [=] (const QModelIndex& current)
            {
                widget->setVisible(current.row() == currentDefinition);
            });

            connect(removeButton, &QPushButton::pressed, widget, [=]
            {
                model->removeRow(rowCount);
                unmap(checkBox);
                unmap(spinBox);
                ui->rightLayout->removeWidget(widget);
                auto it = std::find(steps[getCurrentDefinition()].begin(), steps[getCurrentDefinition()].end(), widget);
                steps[currentDefinition].erase(it);
                delete widget;
            });

            update();
        });
    }

    ~MainWindow()
    {
        delete ui;
    }

private:

    void map(QCheckBox* checkBox, QAbstractItemModel* model, int column, std::function<int()> getRow)
    {
        connect(checkBox, &QCheckBox::toggled, this, [=] (bool value)
        {
            model->setData(model->index(getRow(), column), value, Qt::EditRole);
        });

        auto update = [=]
        {
            checkBox->setChecked(model->data(model->index(getRow(), column), Qt::DisplayRole).toBool());
        };

        mapping.emplace(checkBox, update);
    }

    void map(QSpinBox* spinBox, QAbstractItemModel* model, int column, std::function<int()> getRow)
    {
        connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=] (int value)
        {
            model->setData(model->index(getRow(), column), value, Qt::EditRole);
        });

        auto update = [=]
        {
            spinBox->setValue(model->data(model->index(getRow(), column), Qt::DisplayRole).toInt());
        };

        mapping.emplace(spinBox, update);
    }

    void unmap(QWidget* widget)
    {
        mapping.erase(mapping.find(widget));
    }

    void update() const
    {
        for (const auto& pair : mapping)
        {
            pair.second();
        }
    }

    Ui::MainWindow *ui;
    std::map<QWidget*, std::function<void()>> mapping;
    std::map<int, std::vector<QWidget*>> steps;
};