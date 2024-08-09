#include "modelmenu.h"

#include <QItemSelectionRange>

ModelMenu::ModelMenu(const QString &title, QWidget *parent) : QMenu(title, parent) {}

void ModelMenu::setModel(QAbstractItemModel *model)
{
    if(m_model)
    {
        disconnect(m_model.get(), &QAbstractItemModel::dataChanged, this, &ModelMenu::onModelDataChanged);
        disconnect(m_model.get(), &QAbstractItemModel::modelReset, this, &ModelMenu::onModelReset);
        disconnect(m_model.get(), &QAbstractItemModel::rowsInserted, this, &ModelMenu::onModelRowsInserted);
        disconnect(m_model.get(), &QAbstractItemModel::rowsRemoved, this, &ModelMenu::onModelRowsRemoved);
    }
    m_model = model;
    clear();
    if(model)
    {
        connect(model, &QAbstractItemModel::dataChanged, this, &ModelMenu::onModelDataChanged);
        connect(model, &QAbstractItemModel::modelReset, this, &ModelMenu::onModelReset);
        connect(model, &QAbstractItemModel::rowsInserted, this, &ModelMenu::onModelRowsInserted);
        connect(model, &QAbstractItemModel::rowsRemoved, this, &ModelMenu::onModelRowsRemoved);
        const auto rowCount = model->rowCount();
        if(rowCount > 0)
            onModelRowsInserted({}, 0, rowCount - 1);
    }
}

void ModelMenu::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    Q_UNUSED(roles)

    const auto actions = this->actions();
    QItemSelectionRange range = {topLeft, bottomRight};
    const auto indexes = range.indexes();
    for (const auto &index : indexes)
    {
        const auto row = index.row();
        if (row >= actions.size())
            continue;

        auto action = actions.at(row);
        action->setText(index.data(Qt::DisplayRole).toString());
    }
}

void ModelMenu::onModelReset()
{
    clear();
    if(m_model)
        onModelRowsInserted({}, 0, m_model->rowCount() - 1);
}

void ModelMenu::onModelRowsInserted(const QModelIndex &parent, int first, int last)
{
    if(!m_model)
        return;

    const QItemSelectionRange range = {m_model->index(first, 0, parent), m_model->index(last, 0, parent)};
    const auto indexes = range.indexes();
    for (const auto &index : indexes)
    {
        addAction(index.data(Qt::DisplayRole).toString());
    }
}

void ModelMenu::onModelRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    const auto actions = this->actions();
    for (auto i = first; i <= last; i++)
    {
        if (i < actions.size())
            removeAction(actions.at(i));
    }
}
