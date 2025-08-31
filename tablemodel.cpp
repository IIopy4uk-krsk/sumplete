#include "tablemodel.h"

#include <QRandomGenerator>
#include <QDebug>

#define COLUMNS (9)
#define ROWS (9)
#define COUNT (COLUMNS * ROWS)
#define VALUERANGE 0,9

TableModel::TableModel(QObject *parent) : QAbstractListModel(parent)
{
    m_settings = new QSettings(QSettings::UserScope);

    if (!loadGame())
        newGame();
}

TableModel::~TableModel()
{
    m_settings->sync();
    delete m_settings;
}


int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ROWS;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COLUMNS;
}

QVariant TableModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    int ndx = index.column() + COLUMNS * index.row();

    switch (role) {
    case IndexRole:
        return QVariant::fromValue(ndx);
    case ValueRole:
    case Qt::DisplayRole:
        return QVariant::fromValue(m_table.value(ndx, -1));
    case ColorRole:
        if (m_flags.value(ndx, 0) > 0)
            return QVariant::fromValue(QString("#faa"));
        else
            return QVariant::fromValue(QString("#fff"));
    case ImageRole:
    {
        int aa = m_currentMark.value(ndx, 0);
        if (aa == 1)
            return QVariant::fromValue(QString("images/cross.png"));
        else if (aa == 2)
            return QVariant::fromValue(QString("images/circle.png"));
        return QString("");
    }

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[ValueRole] = "value";
    roles[ColorRole] = "color";
    roles[ImageRole] = "image";

    return roles;
}

QVariantList TableModel::rowSums() const
{
    return m_rowSums;
}

QVariantList TableModel::colSums() const
{
    return m_colSums;
}

void TableModel::toggleMark(int index)
{
    int val = m_currentMark.value(index, 0) + 1;

    if (val > 2)
        val = 0;

    m_currentMark.replace(index, val);

    saveGame();

    auto ndx = this->index(index / COLUMNS, index % COLUMNS);
    emit dataChanged(ndx, ndx);

    checkForVictory();

}

void TableModel::calculateRowSums()
{
    m_rowSums.clear();
    m_colSums.clear();

    for (int i = 0; i < ROWS; i++)
    {
        int rsum = 0;
        for (int j = 0; j < COLUMNS; j++)
        {
            if (m_flags.value(i * COLUMNS + j, 0) == 0)
                rsum += m_table.value(i * COLUMNS + j, 0);
        }
        m_rowSums.append(rsum);
    }

    for (int i = 0; i < COLUMNS; i++)
    {
        int csum = 0;
        for (int j = 0; j < ROWS; j++)
        {
            if (m_flags.value(j * ROWS + i, 0) == 0)
                csum += m_table.value(j * ROWS + i, 0);
        }
        m_colSums.append(csum);
    }

    emit signal_tableRecalculated();
}

void TableModel::checkForVictory()
{
    int correctRows = 0;
    for (int i = 0; i < ROWS; i++)
    {
        int rsum = 0;
        for (int j = 0; j < COLUMNS; j++)
        {
            if (m_currentMark.value(i * COLUMNS + j, 0) != 1)
                rsum += m_table.value(i * COLUMNS + j, 0);
        }
        if (rsum == m_rowSums.value(i, 0))
            correctRows++;
    }

    int correctColumns = 0;
    for (int i = 0; i < COLUMNS; i++)
    {
        int csum = 0;
        for (int j = 0; j < ROWS; j++)
        {
            if (m_currentMark.value(j * ROWS + i, 0) != 1)
                csum += m_table.value(j * ROWS + i, 0);
        }
        if (csum == m_colSums.value(i, 0))
            correctColumns++;
    }

    if ((correctRows == ROWS) && (correctColumns == COLUMNS))
        emit signal_winGame();
}

QString toString(QList<int> list)
{
    QString s = "";
    for (auto item : list)
        s += QString("%1,").arg(item);
    return s;
}

QList<int> fromString(QString s)
{
    QList<int> list;

    for (auto item : s.split(",", Qt::SkipEmptyParts))
        list.append(item.toInt());

    return list;
}

bool TableModel::loadGame()
{
    if (!m_settings)
        return false;

    if (!m_settings->contains("field") || !m_settings->contains("flags") || !m_settings->contains("marks"))
        return false;

    m_table = fromString(m_settings->value("field").toString());
    m_flags = fromString(m_settings->value("flags").toString());
    m_currentMark = fromString(m_settings->value("marks").toString());

    emit dataChanged(index(0,0), index(ROWS - 1,COLUMNS - 1));

    calculateRowSums();

    return true;

}

void TableModel::saveGame()
{
    m_settings->clear();

    m_settings->setValue("field", toString(m_table));
    m_settings->setValue("flags", toString(m_flags));
    m_settings->setValue("marks", toString(m_currentMark));
}

void TableModel::newGame()
{
    m_table.clear();
    m_flags.clear();
    m_currentMark.clear();
    m_rowSums.clear();
    m_colSums.clear();

    m_table.reserve(COUNT);
    // Инициализация таблицы данными
    for (int i = 0; i < ROWS; ++i) {
        quint16 mask = QRandomGenerator::global()->bounded(0, (1 << COLUMNS) - 1);
        for (int j = 0; j < COLUMNS; ++j)
        {
            int val = 0;
            while (val == 0)
                val = QRandomGenerator::global()->bounded(VALUERANGE);

            m_table.append(val);
            if ((mask & (1 << j)) > 0)
                m_flags.append(1);
            else
                m_flags.append(0);

            m_currentMark.append(0);
        }
    }

    saveGame();

    calculateRowSums();

    emit dataChanged(index(0,0), index(ROWS - 1,COLUMNS - 1));
}

void TableModel::resetGame()
{
    for (auto &a : m_currentMark)
    {
        a = 0;
    }

    saveGame();

    emit dataChanged(index(0,0), index(ROWS - 1,COLUMNS - 1));
}
