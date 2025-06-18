#pragma once
#include <QtWidgets>
#include <QColor>
#include <QDate>
#include <QTime>
#include <QPoint>
#include <QSize>
#include <QRect>

class ExtendedConfig : public QObject {
    Q_OBJECT
        // Proprietà leggibili e scrivibili di diversi tipi
        Q_PROPERTY(int integerValue READ integerValue WRITE setIntegerValue NOTIFY integerValueChanged)
        Q_PROPERTY(double doubleValue READ doubleValue WRITE setDoubleValue NOTIFY doubleValueChanged)
        Q_PROPERTY(QString stringValue READ stringValue WRITE setStringValue NOTIFY stringValueChanged)
        Q_PROPERTY(QColor colorValue READ colorValue WRITE setColorValue NOTIFY colorValueChanged)
        Q_PROPERTY(QStringList stringListValue READ stringListValue WRITE setStringListValue NOTIFY stringListValueChanged)
        Q_PROPERTY(QDate dateValue READ dateValue WRITE setDateValue NOTIFY dateValueChanged)
        Q_PROPERTY(QTime timeValue READ timeValue WRITE setTimeValue NOTIFY timeValueChanged)
        Q_PROPERTY(QPoint pointValue READ pointValue WRITE setPointValue NOTIFY pointValueChanged)
        Q_PROPERTY(QSize sizeValue READ sizeValue WRITE setSizeValue NOTIFY sizeValueChanged)
        Q_PROPERTY(QRect rectValue READ rectValue WRITE setRectValue NOTIFY rectValueChanged)
        Q_PROPERTY(bool boolValue READ boolValue WRITE setBoolValue NOTIFY boolValueChanged)
        Q_PROPERTY(QVariant variantValue READ variantValue WRITE setVariantValue NOTIFY variantValueChanged)

        // Proprietà di metadati (costanti)
        Q_PROPERTY(QString integerDisplay READ integerDisplay CONSTANT)
        Q_PROPERTY(QString colorCategory READ colorCategory CONSTANT)

public:
    ExtendedConfig(QObject* parent = nullptr) : QObject(parent) {
        m_integerValue = 42;
        m_integerDisplay = "Integer Setting";
        m_doubleValue = 3.14159;
        m_stringValue = "Default Text";
        m_colorValue = QColor(Qt::blue);
        m_colorCategory = "Appearance";
        m_stringListValue = QStringList() << "Item1" << "Item2" << "Item3";
        m_dateValue = QDate::currentDate();
        m_timeValue = QTime::currentTime();
        m_pointValue = QPoint(100, 200);
        m_sizeValue = QSize(800, 600);
        m_rectValue = QRect(10, 10, 200, 100);
        m_boolValue = true;
        m_variantValue = QVariant("Initial Variant");
    }

    // Getter
    int integerValue() const { return m_integerValue; }
    QString integerDisplay() const { return m_integerDisplay; }
    double doubleValue() const { return m_doubleValue; }
    QString stringValue() const { return m_stringValue; }
    QColor colorValue() const { return m_colorValue; }
    QString colorCategory() const { return m_colorCategory; }
    QStringList stringListValue() const { return m_stringListValue; }
    QDate dateValue() const { return m_dateValue; }
    QTime timeValue() const { return m_timeValue; }
    QPoint pointValue() const { return m_pointValue; }
    QSize sizeValue() const { return m_sizeValue; }
    QRect rectValue() const { return m_rectValue; }
    bool boolValue() const { return m_boolValue; }
    QVariant variantValue() const { return m_variantValue; }

public slots:
    // Setter
    void setIntegerValue(int value) {
        if (m_integerValue != value) {
            m_integerValue = value;
            emit integerValueChanged();
        }
    }

    void setDoubleValue(double value) {
        if (m_doubleValue != value) {
            m_doubleValue = value;
            emit doubleValueChanged();
        }
    }

    void setStringValue(const QString& value) {
        if (m_stringValue != value) {
            m_stringValue = value;
            emit stringValueChanged();
        }
    }

    void setColorValue(const QColor& value) {
        if (m_colorValue != value) {
            m_colorValue = value;
            emit colorValueChanged();
        }
    }

    void setStringListValue(const QStringList& value) {
        if (m_stringListValue != value) {
            m_stringListValue = value;
            emit stringListValueChanged();
        }
    }

    void setDateValue(const QDate& value) {
        if (m_dateValue != value) {
            m_dateValue = value;
            emit dateValueChanged();
        }
    }

    void setTimeValue(const QTime& value) {
        if (m_timeValue != value) {
            m_timeValue = value;
            emit timeValueChanged();
        }
    }

    void setPointValue(const QPoint& value) {
        if (m_pointValue != value) {
            m_pointValue = value;
            emit pointValueChanged();
        }
    }

    void setSizeValue(const QSize& value) {
        if (m_sizeValue != value) {
            m_sizeValue = value;
            emit sizeValueChanged();
        }
    }

    void setRectValue(const QRect& value) {
        if (m_rectValue != value) {
            m_rectValue = value;
            emit rectValueChanged();
        }
    }

    void setBoolValue(bool value) {
        if (m_boolValue != value) {
            m_boolValue = value;
            emit boolValueChanged();
        }
    }

    void setVariantValue(const QVariant& value) {
        if (m_variantValue != value) {
            m_variantValue = value;
            emit variantValueChanged();
        }
    }

signals:
    void integerValueChanged();
    void doubleValueChanged();
    void stringValueChanged();
    void colorValueChanged();
    void stringListValueChanged();
    void dateValueChanged();
    void timeValueChanged();
    void pointValueChanged();
    void sizeValueChanged();
    void rectValueChanged();
    void boolValueChanged();
    void variantValueChanged();

private:
    int m_integerValue;
    QString m_integerDisplay;
    double m_doubleValue;
    QString m_stringValue;
    QColor m_colorValue;
    QString m_colorCategory;
    QStringList m_stringListValue;
    QDate m_dateValue;
    QTime m_timeValue;
    QPoint m_pointValue;
    QSize m_sizeValue;
    QRect m_rectValue;
    bool m_boolValue;
    QVariant m_variantValue;
};