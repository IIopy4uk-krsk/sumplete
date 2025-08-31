#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QRandomGenerator>
#include <QSettings>

class TableModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QVariantList rowSums READ rowSums NOTIFY signal_tableRecalculated)
    Q_PROPERTY(QVariantList colSums READ colSums NOTIFY signal_tableRecalculated)

public:
    enum Roles {
        IndexRole = Qt::UserRole + 1,
        ValueRole,
        ColorRole,
        ImageRole
    };

    explicit TableModel(QObject *parent = nullptr);
    virtual ~TableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariantList rowSums() const;
    QVariantList colSums() const;

    Q_INVOKABLE void toggleMark(int index);

    Q_INVOKABLE void newGame();
    Q_INVOKABLE void resetGame();
protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<int> m_table;
    QList<int> m_flags; // 1-зачеркнут 0-присутствует в сумме
    QList<int> m_currentMark; // 0 - неопред, 1 - зачеркнут, 2 - обведен
    QVariantList m_rowSums;
    QVariantList m_colSums;
    QSettings *m_settings;

    void calculateRowSums();

    void checkForVictory();

    bool loadGame();
    void saveGame();
signals:
    void signal_tableRecalculated();
    void signal_tableChanged();
    void signal_winGame();
};

