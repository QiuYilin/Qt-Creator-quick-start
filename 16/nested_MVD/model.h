#pragma once
#include <QAbstractItemModel>
#include "common.h"

class StepModel : public QAbstractItemModel
{
    Q_OBJECT
    using BaseClass = QAbstractItemModel;

public:
    enum Columns
    {
        E_BOOL_1,
        E_INT_1,
        E_REQUIREMENT,
        _END
    };

    StepModel(Definition* definition, QObject* parent) :
        BaseClass(parent),
        definition(definition)
    {
    }

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        return createIndex(row, column, nullptr);
    }
    virtual QModelIndex parent(const QModelIndex &child) const override
    {
        return QModelIndex();
    }
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return definition->Steps.size();
    }
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _END;
    }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole) return QVariant();

        const auto& step = definition->Steps.at(index.row());
        switch (index.column())
        {
            case E_BOOL_1:
                return step->SomeBoolMember1;
            case E_INT_1:
                return step->SomeIntMember1;
            case E_REQUIREMENT:
                return QVariant();
        }
    }
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
    {
        if (role != Qt::EditRole) return false;

        auto& step = definition->Steps.at(index.row());
        switch (index.column())
        {
            case E_BOOL_1:
                step->SomeBoolMember1 = value.toBool();
                return true;
            case E_INT_1:
                step->SomeIntMember1 = value.toInt();
                return true;
            case E_REQUIREMENT:
                assert(false);
                return false;
        }
    }
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        assert(count == 1);
        beginInsertRows(parent, row, row);
        definition->Steps.push_back(std::make_shared<Step>());
        endInsertRows();
        return true;
    }
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        assert(count == 1);
        beginRemoveRows(parent, row, row);
        definition->Steps.erase(definition->Steps.begin() + row);
        endRemoveRows();
        return true;
    }

private:
    Definition* definition;
};
Q_DECLARE_METATYPE(StepModel*)


class DefinitionModel : public QAbstractItemModel
{
    Q_OBJECT
    using BaseClass = QAbstractItemModel;

public:
    enum Columns
    {
        E_NAME,
        E_BOOL_1,
        E_INT_1,
        E_STEPS,
        _END
    };

    DefinitionModel(QObject* parent) :
        BaseClass(parent)
    {
    }

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        return createIndex(row, column, nullptr);
    }
    virtual QModelIndex parent(const QModelIndex &child) const override
    {
        return QModelIndex();
    }
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return definitionData.definitions.size();
    }
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return _END;
    }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole) return QVariant();

        const auto& definition = definitionData.definitions.at(index.row());
        switch (index.column())
        {
            case E_NAME:
                return QString("Definition %1").arg(index.row() + 1);
            case E_BOOL_1:
                return definition->SomeBoolMember1;
            case E_INT_1:
                return definition->SomeIntMember1;
            case E_STEPS:
                return QVariant::fromValue(new StepModel(definition.get(), nullptr));
        }
    }
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
    {
        if (role != Qt::EditRole) return false;

        auto& definition = definitionData.definitions.at(index.row());
        switch (index.column())
        {
            case E_BOOL_1:
                definition->SomeBoolMember1 = value.toBool();
                return true;
            case E_INT_1:
                definition->SomeIntMember1 = value.toInt();
                return true;
            default:
                assert(false);
                return false;
        }
    }
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        assert(count == 1);
        beginInsertRows(parent, row, row);
        definitionData.definitions.push_back(std::make_shared<Definition>());
        endInsertRows();
        return true;
    }
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        assert(count == 1);
        beginRemoveRows(parent, row, row);
        definitionData.definitions.pop_back();
        endRemoveRows();
        return true;
    }

private:
    Data definitionData;
};