#ifndef MODELMENU_H
#define MODELMENU_H

#include <QAbstractItemModel>
#include <QMenu>
#include <QPointer>

class ModelMenu : public QMenu
{
    Q_OBJECT
public:
    ModelMenu(const QString &title, QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model);

private:
    void onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);
    void onModelReset();
    void onModelRowsInserted(const QModelIndex &parent, int first, int last);
    void onModelRowsRemoved(const QModelIndex &parent, int first, int last);

    QPointer<QAbstractItemModel> m_model;
};

#endif // MODELMENU_H
