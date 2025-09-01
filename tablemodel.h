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

    enum Marks
    {
        MarkUndefined = 0,
        MarkCrossed,
        MarkValidated,
        MarkCrossedHint,
        MarkValidatedHint
    };

    explicit TableModel(QObject *parent = nullptr);
    virtual ~TableModel();

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariantList rowSums() const;
    QVariantList colSums() const;

    Q_INVOKABLE void toggleMark(int index);

    Q_INVOKABLE void newGame();
    Q_INVOKABLE void resetGame();

    Q_INVOKABLE void makeHint();

protected:
    QHash<int, QByteArray> roleNames() const override;
private:

    const QMap<int, QString> m_images = {{MarkUndefined, ""}, {MarkCrossed, "images/cross.png"}, {MarkValidated, "images/circle.png"}, {MarkCrossedHint, "images/cross_hint.png"}, {MarkValidatedHint, "images/circle_hint.png"}};
    QList<int> m_table;
    QList<int> m_flags; // enum Marks
    QList<int> m_currentMark; // enum Marks
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

