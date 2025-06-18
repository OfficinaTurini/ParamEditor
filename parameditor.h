/*
 * Copyright (c) 2025 Manuele Turini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file parameditor.h
 * @brief A Qt-based parameter editor dialog for managing various parameter types.
 * @author Manuele Turini
 * @copyright (C) 2025 Manuele Turini. All Rights Reserved.
 */

#ifndef PARAMEDITOR_H
#define PARAMEDITOR_H

#include <QtWidgets>
#include <QObject>
#include <QMetaProperty>
#include <QMetaEnum>
#include <QDebug>
#include <QMap>
#include <climits>
#include <cfloat>

 /**
  * @class ParamBase
  * @brief Abstract base class for parameter types in the ParamsEditor.
  *
  * This class defines the interface for all parameter types, providing virtual methods
  * for applying, resetting, saving, and loading parameter values. It inherits from QWidget
  * to support GUI integration.
  */
class ParamBase : public QWidget {
    Q_OBJECT
public:
    QString     name; ///< Parameter name displayed in the GUI.
    QWidget     * widget = nullptr; ///< Widget for user input (e.g., QSpinBox, QLineEdit).
    QPushButton * defButton = nullptr; ///< Button to reset to default value.
    QPushButton * browseButton = nullptr; ///< Button for file/directory browsing (used by FilePathParam and DirParam).

    /**
     * @brief Apply the current widget value to the referenced variable.
     */
    virtual void apply() = 0;

    /**
     * @brief Reset the widget to its default value.
     */
    virtual void reset() = 0;

    /**
     * @brief Save the parameter value to an XML stream.
     * @param w The QXmlStreamWriter to write to.
     */
    virtual void save(QXmlStreamWriter& w) const = 0;

    /**
     * @brief Load the parameter value from an XML stream.
     * @param r The QXmlStreamReader to read from.
     */
    virtual void load(QXmlStreamReader& r) = 0;

    /**
     * @brief Virtual destructor for proper cleanup.
     */
    virtual ~ParamBase() {}

protected:
    /**
     * @brief Protected constructor to initialize the QWidget parent.
     * @param parent The parent widget (default: nullptr).
     */
    ParamBase(QWidget * parent = nullptr) : QWidget(parent) {}
};

/**
 * @class DoubleParam
 * @brief Parameter class for handling double-precision floating-point values.
 */
class DoubleParam : public ParamBase {
    Q_OBJECT
        double* ptr; ///< Pointer to the double value.
    double defVal; ///< Default value.
    QDoubleSpinBox* spin; ///< Spin box for user input.
public:
    /**
     * @brief Constructor for DoubleParam.
     * @param name Parameter name.
     * @param p Pointer to the double value.
     * @param min Minimum allowed value.
     * @param max Maximum allowed value.
     * @param step Step size for the spin box.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    DoubleParam(QString name, double* p, double min, double max, double step, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = *p;
        spin = new QDoubleSpinBox(this);
        spin->setRange(min, max);
        spin->setSingleStep(step);
        spin->setValue(*p);
        spin->setToolTip(tip);
        spin->setAlignment(Qt::AlignRight);
        widget = spin;
    }
    void apply() override { *ptr = spin->value(); }
    void reset() override { spin->setValue(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", QString::number(spin->value()));
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value"))
            spin->setValue(r.attributes().value("value").toDouble());
        r.readNext();
    }
};

/**
 * @class IntParam
 * @brief Parameter class for handling integer values.
 */
class IntParam : public ParamBase {
    Q_OBJECT
        int* ptr; ///< Pointer to the integer value.
    int defVal; ///< Default value.
    QSpinBox* spin; ///< Spin box for user input.
public:
    /**
     * @brief Constructor for IntParam.
     * @param name Parameter name.
     * @param p Pointer to the integer value.
     * @param min Minimum allowed value.
     * @param max Maximum allowed value.
     * @param step Step size for the spin box.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    IntParam(QString name, int* p, int min, int max, int step, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = *p;
        spin = new QSpinBox(this);
        spin->setRange(min, max);
        spin->setSingleStep(step);
        spin->setValue(*p);
        spin->setToolTip(tip);
        spin->setAlignment(Qt::AlignRight);
        widget = spin;
    }
    void apply() override { *ptr = spin->value(); }
    void reset() override { spin->setValue(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", QString::number(spin->value()));
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value"))
            spin->setValue(r.attributes().value("value").toInt());
        r.readNext();
    }
};

/**
 * @class StringParam
 * @brief Parameter class for handling string values.
 */
class StringParam : public ParamBase {
    Q_OBJECT
protected:  // Cambiato da private a protected
    QString     * ptr; ///< Pointer to the string value.
    QString     defVal; ///< Default value.
    QLineEdit   * edit; ///< Line edit for user input.
public:
    /**
     * @brief Constructor for StringParam.
     * @param name Parameter name.
     * @param p Pointer to the string value.
     * @param def Default string value.
     * @param hints Input method hints for the line edit.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    StringParam(QString name, QString* p, QString def, Qt::InputMethodHints hints, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        edit = new QLineEdit(*p, this);
        edit->setInputMethodHints(hints);
        edit->setToolTip(tip);
        widget = edit;
    }
    void apply() override { *ptr = edit->text(); }
    void reset() override { edit->setText(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", edit->text());
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value"))
            edit->setText(r.attributes().value("value").toString());
        r.readNext();
    }
};


/**
 * @class ComboParam
 * @brief Parameter class for handling combo box selections.
 */
class ComboParam : public ParamBase {
    Q_OBJECT
    QStringList * options; ///< Pointer to the list of options.
    int         * ptr; ///< Pointer to the selected index.
    int         defVal; ///< Default index.
    QComboBox   * combo; ///< Combo box for user selection.
public:
    /**
     * @brief Constructor for ComboParam.
     * @param name Parameter name.
     * @param opts Pointer to the list of options.
     * @param p Pointer to the selected index.
     * @param defIndex Default index.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    ComboParam(QString name, QStringList* opts, int* p, int defIndex, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        options = opts;
        ptr = p;
        defVal = defIndex;
        combo = new QComboBox(this);
        combo->addItems(*opts);
        combo->setCurrentIndex(*p);
        combo->setToolTip(tip);
        widget = combo;
    }
    void apply() override { *ptr = combo->currentIndex(); }
    void reset() override { combo->setCurrentIndex(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("index", QString::number(combo->currentIndex()));
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("index"))
            combo->setCurrentIndex(r.attributes().value("index").toInt());
        r.readNext();
    }
};

/**
 * @class ColorParam
 * @brief Parameter class for handling color selections.
 */
class ColorParam : public ParamBase {
    Q_OBJECT
    QColor      * ptr; ///< Pointer to the color value.
    QColor      defVal; ///< Default color.
    QPushButton * btn; ///< Button to open the color dialog.
public:
    /**
     * @brief Constructor for ColorParam.
     * @param name Parameter name.
     * @param p Pointer to the color value.
     * @param def Default color.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    ColorParam(QString name, QColor* p, QColor def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        btn = new QPushButton(this);
        updateButton(*ptr);
        btn->setToolTip(tip);
        QObject::connect(btn, &QPushButton::clicked, [this, name]() {
            QColor c = QColorDialog::getColor(*ptr, nullptr, name);
            if (c.isValid()) {
                updateButton(c);
                *ptr = c;
            }
            });
        widget = btn;
    }
    void apply() override { *ptr = btn->palette().button().color(); }
    void reset() override { updateButton(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("color", btn->palette().button().color().name());
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("color"))
            updateButton(QColor(r.attributes().value("color").toString()));
        r.readNext();
    }
private:
    /**
     * @brief Update the button's background color.
     * @param c The color to set.
     */
    void updateButton(const QColor& c) {
        btn->setStyleSheet(QString("QPushButton { background-color: %1; border: 1px solid black; }").arg(c.name()));
        btn->setAutoFillBackground(true);
        btn->update();
    }
};

/**
 * @class FilePathParam
 * @brief Parameter class for handling file path selections.
 */
class FilePathParam : public ParamBase {
    Q_OBJECT
    QString     * ptr; ///< Pointer to the file path.
    QString     defVal; ///< Default file path.
    QLineEdit   * edit; ///< Line edit for file path input.
public:
    /**
     * @brief Constructor for FilePathParam.
     * @param name Parameter name.
     * @param p Pointer to the file path.
     * @param def Default file path.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    FilePathParam(QString name, QString* p, QString def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        edit = new QLineEdit(*p, this);
        edit->setToolTip(tip);
        widget = edit;
        browseButton = nullptr;
    }
    void apply() override { *ptr = edit->text(); }
    void reset() override { edit->setText(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("path", edit->text());
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("path"))
            edit->setText(r.attributes().value("path").toString());
        r.readNext();
    }
public slots:
    /**
     * @brief Open a file dialog to select a file.
     */
    void onBrowseClicked() {
        QString file = QFileDialog::getOpenFileName(widget, "Select File", edit->text());
        if (!file.isEmpty())
            edit->setText(file);
    }
};

/**
 * @class DirParam
 * @brief Parameter class for handling directory path selections.
 */
class DirParam : public ParamBase {
    Q_OBJECT
    QString     * ptr; ///< Pointer to the directory path.
    QString     defVal; ///< Default directory path.
    QLineEdit   * edit; ///< Line edit for directory path input.
public:
    /**
     * @brief Constructor for DirParam.
     * @param name Parameter name.
     * @param p Pointer to the directory path.
     * @param def Default directory path.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    DirParam(QString name, QString* p, QString def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        edit = new QLineEdit(*p, this);
        edit->setToolTip(tip);
        widget = edit;
        browseButton = nullptr;
    }
    void apply() override { *ptr = edit->text(); }
    void reset() override { edit->setText(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("path", edit->text());
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("path"))
            edit->setText(r.attributes().value("path").toString());
        r.readNext();
    }
public slots:
    /**
     * @brief Open a directory dialog to select a directory.
     */
    void onBrowseClicked() {
        QString dir = QFileDialog::getExistingDirectory(widget, "Select Directory", edit->text());
        if (!dir.isEmpty())
            edit->setText(dir);
    }
};

/**
 * @class BoolParam
 * @brief Parameter class for handling boolean values.
 */
class BoolParam : public ParamBase {
    Q_OBJECT
    bool        * ptr; ///< Pointer to the boolean value.
    bool        defVal; ///< Default boolean value.
    QCheckBox   * checkBox; ///< Checkbox for user input.
public:
    /**
     * @brief Constructor for BoolParam.
     * @param name Parameter name.
     * @param p Pointer to the boolean value.
     * @param def Default boolean value.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    BoolParam(QString name, bool* p, bool def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        checkBox = new QCheckBox(this);
        checkBox->setChecked(*p);
        checkBox->setToolTip(tip);
        widget = checkBox;
    }
    void apply() override { *ptr = checkBox->isChecked(); }
    void reset() override { checkBox->setChecked(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", checkBox->isChecked() ? "true" : "false");
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value"))
            checkBox->setChecked(r.attributes().value("value").toString() == "true");
        r.readNext();
    }
};

/**
 * @class FontParam
 * @brief Parameter class for handling font selections.
 */
class FontParam : public ParamBase {
    Q_OBJECT
    QFont       * ptr; ///< Pointer to the font value.
    QFont       defVal; ///< Default font.
    QPushButton * btn; ///< Button to open the font dialog.
    QFont       currentFont; ///< Current selected font.

public:
    /**
     * @brief Constructor for FontParam.
     * @param name Parameter name.
     * @param p Pointer to the font value.
     * @param def Default font.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    FontParam(QString name, QFont* p, QFont def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        currentFont = *p;
        btn = new QPushButton(this);
        updateButton(currentFont);
        btn->setToolTip(tip);
        QObject::connect(btn, &QPushButton::clicked, [this, name]() {
            bool ok;
            QFont font = QFontDialog::getFont(&ok, currentFont, nullptr, name);
            if (ok) {
                currentFont = font;
                updateButton(font);
            }
            });
        widget = btn;
    }
    void apply() override { *ptr = currentFont; }
    void reset() override {
        currentFont = defVal;
        updateButton(defVal);
    }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", currentFont.toString());
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value")) {
            currentFont.fromString(r.attributes().value("value").toString());
            updateButton(currentFont);
        }
        r.readNext();
    }
private:
    /**
     * @brief Update the button's text to show font details.
     * @param f The font to display.
     */
    void updateButton(const QFont& f) {
        btn->setText(f.family() + " " + QString::number(f.pointSize()));
    }
};

/**
 * @class PasswordParam
 * @brief Parameter class for handling password strings with hidden input.
 */
class PasswordParam : public StringParam {
    Q_OBJECT
public:
    /**
     * @brief Constructor for PasswordParam.
     * @param name Parameter name.
     * @param p Pointer to the password string.
     * @param def Default password.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    PasswordParam(QString name, QString* p, QString def, QString tip, QWidget* parent = nullptr)
        : StringParam(name, p, def, Qt::ImhHiddenText, tip, parent) {
        edit->setEchoMode(QLineEdit::Password);
    }
};

/**
 * @class DateTimeParam
 * @brief Parameter class for handling date-time values.
 */
class DateTimeParam : public ParamBase {
    Q_OBJECT
    QDateTime       * ptr; ///< Pointer to the date-time value.
    QDateTime       defVal; ///< Default value.
    QDateTimeEdit   * dateTimeEdit; ///< DateTime edit widget.

public:
    /**
     * @brief Constructor for DateTimeParam.
     * @param name Parameter name.
     * @param p Pointer to the date-time value.
     * @param def Default date-time.
     * @param tip Tooltip text.
     * @param format Display format string (default: "dd/MM/yyyy hh:mm").
     * @param parent Parent widget (default: nullptr).
     */
    DateTimeParam(QString name, QDateTime* p, QDateTime def,
        QString tip = "", QString format = "dd/MM/yyyy hh:mm",
        QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        dateTimeEdit = new QDateTimeEdit(*p, this);
        dateTimeEdit->setDisplayFormat(format);
        dateTimeEdit->setToolTip(tip);
        dateTimeEdit->setCalendarPopup(true);
        widget = dateTimeEdit;
    }
    void apply() override { *ptr = dateTimeEdit->dateTime(); }
    void reset() override { dateTimeEdit->setDateTime(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", dateTimeEdit->dateTime().toString(Qt::ISODate));
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value")) {
            QDateTime dt = QDateTime::fromString(r.attributes().value("value").toString(), Qt::ISODate);
            if (dt.isValid()) dateTimeEdit->setDateTime(dt);
        }
        r.readNext();
    }
};

/**
 * @class RangeParam
 * @brief Parameter class for handling numeric ranges.
 */
class RangeParam : public ParamBase {
    Q_OBJECT
    QPair<double, double>   * ptr; ///< Pointer to the range values.
    QPair<double, double>   defVal; ///< Default range.
    QDoubleSpinBox          * minSpin; ///< Spin box for min value.
    QDoubleSpinBox          * maxSpin; ///< Spin box for max value.

public:
    /**
     * @brief Constructor for RangeParam.
     * @param name Parameter name.
     * @param p Pointer to the range value.
     * @param min Global minimum value.
     * @param max Global maximum value.
     * @param step Step size.
     * @param def Default range.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    RangeParam(QString name, QPair<double, double>* p,
        double globalMin, double globalMax, double step,
        QPair<double, double> def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;

        QWidget* container = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);

        minSpin = new QDoubleSpinBox;
        minSpin->setRange(globalMin, globalMax);
        minSpin->setSingleStep(step);
        minSpin->setValue(p->first);

        maxSpin = new QDoubleSpinBox;
        maxSpin->setRange(globalMin, globalMax);
        maxSpin->setSingleStep(step);
        maxSpin->setValue(p->second);

        layout->addWidget(minSpin);
        layout->addWidget(new QLabel("to"));
        layout->addWidget(maxSpin);

        widget = container;
    }

    void apply() override {
        ptr->first = minSpin->value();
        ptr->second = maxSpin->value();
    }

    void reset() override {
        minSpin->setValue(defVal.first);
        maxSpin->setValue(defVal.second);
    }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("min", QString::number(minSpin->value()));
        w.writeAttribute("max", QString::number(maxSpin->value()));
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("min") && r.attributes().hasAttribute("max")) {
            minSpin->setValue(r.attributes().value("min").toDouble());
            maxSpin->setValue(r.attributes().value("max").toDouble());
        }
        r.readNext();
    }
};

/**
 * @class StringListParam
 * @brief Parameter class for handling string list values.
 */
class StringListParam : public ParamBase {
    Q_OBJECT
    QStringList * ptr; ///< Pointer to the string list value.
    QStringList defVal; ///< Default string list value.
    QComboBox   * combo; ///< Combo box for user selection.

public:
    /**
     * @brief Constructor for StringListParam.
     * @param name Parameter name.
     * @param p Pointer to the string list value.
     * @param def Default string list.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    StringListParam(QString name, QStringList* p, QStringList def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        combo = new QComboBox(this);
        combo->setEditable(true); // Permette di aggiungere nuovi elementi
        combo->addItems(*p);
        combo->setToolTip(tip);
        widget = combo;
    }

    void apply() override {
        *ptr = combo->currentText().split(",", Qt::SkipEmptyParts);
    }

    void reset() override {
        combo->clear();
        combo->addItems(defVal);
    }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", combo->currentText());
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value")) {
            combo->clear();
            QStringList items = r.attributes().value("value").toString().split(",", Qt::SkipEmptyParts);
            combo->addItems(items);
        }
        r.readNext();
    }
};

/**
 * @class DateParam
 * @brief Parameter class for handling date values.
 */
class DateParam : public ParamBase {
    Q_OBJECT
    QDate       * ptr; ///< Pointer to the date value.
    QDate       defVal; ///< Default date value.
    QDateEdit   * dateEdit; ///< Date edit widget.

public:
    /**
     * @brief Constructor for DateParam.
     * @param name Parameter name.
     * @param p Pointer to the date value.
     * @param def Default date.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    DateParam(QString name, QDate* p, QDate def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        dateEdit = new QDateEdit(*p, this);
        dateEdit->setDisplayFormat("dd/MM/yyyy");
        dateEdit->setToolTip(tip);
        dateEdit->setCalendarPopup(true);
        widget = dateEdit;
    }

    void apply() override { *ptr = dateEdit->date(); }
    void reset() override { dateEdit->setDate(defVal); }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", dateEdit->date().toString(Qt::ISODate));
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value")) {
            QDate date = QDate::fromString(r.attributes().value("value").toString(), Qt::ISODate);
            if (date.isValid()) dateEdit->setDate(date);
        }
        r.readNext();
    }
};

/**
 * @class TimeParam
 * @brief Parameter class for handling time values.
 */
class TimeParam : public ParamBase {
    Q_OBJECT
    QTime       * ptr; ///< Pointer to the time value.
    QTime       defVal; ///< Default time value.
    QTimeEdit   * timeEdit; ///< Time edit widget.

public:
    /**
     * @brief Constructor for TimeParam.
     * @param name Parameter name.
     * @param p Pointer to the time value.
     * @param def Default time.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    TimeParam(QString name, QTime* p, QTime def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        timeEdit = new QTimeEdit(*p, this);
        timeEdit->setDisplayFormat("hh:mm:ss");
        timeEdit->setToolTip(tip);
        widget = timeEdit;
    }

    void apply() override { *ptr = timeEdit->time(); }
    void reset() override { timeEdit->setTime(defVal); }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", timeEdit->time().toString(Qt::ISODate));
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value")) {
            QTime time = QTime::fromString(r.attributes().value("value").toString(), Qt::ISODate);
            if (time.isValid()) timeEdit->setTime(time);
        }
        r.readNext();
    }
};

/**
 * @class PointParam
 * @brief Parameter class for handling 2D point values.
 */
class PointParam : public ParamBase {
    Q_OBJECT
    QPoint      * ptr; ///< Pointer to the point value.
    QPoint      defVal; ///< Default point value.
    QSpinBox    * xSpin; ///< Spin box for x coordinate.
    QSpinBox    * ySpin; ///< Spin box for y coordinate.

public:
    /**
     * @brief Constructor for PointParam.
     * @param name Parameter name.
     * @param p Pointer to the point value.
     * @param def Default point.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    PointParam(QString name, QPoint* p, QPoint def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;

        QWidget* container = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);

        xSpin = new QSpinBox;
        xSpin->setRange(INT_MIN, INT_MAX);
        xSpin->setValue(p->x());
        xSpin->setToolTip(tip + " (X coordinate)");

        ySpin = new QSpinBox;
        ySpin->setRange(INT_MIN, INT_MAX);
        ySpin->setValue(p->y());
        ySpin->setToolTip(tip + " (Y coordinate)");

        layout->addWidget(new QLabel("X:"));
        layout->addWidget(xSpin);
        layout->addWidget(new QLabel("Y:"));
        layout->addWidget(ySpin);

        widget = container;
    }

    void apply() override {
        ptr->setX(xSpin->value());
        ptr->setY(ySpin->value());
    }

    void reset() override {
        xSpin->setValue(defVal.x());
        ySpin->setValue(defVal.y());
    }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("x", QString::number(xSpin->value()));
        w.writeAttribute("y", QString::number(ySpin->value()));
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("x") && r.attributes().hasAttribute("y")) {
            xSpin->setValue(r.attributes().value("x").toInt());
            ySpin->setValue(r.attributes().value("y").toInt());
        }
        r.readNext();
    }
};

/**
 * @class SizeParam
 * @brief Parameter class for handling size values.
 */
class SizeParam : public ParamBase {
    Q_OBJECT
    QSize       * ptr; ///< Pointer to the size value.
    QSize       defVal; ///< Default size value.
    QSpinBox    * widthSpin; ///< Spin box for width.
    QSpinBox    * heightSpin; ///< Spin box for height.

public:
    /**
     * @brief Constructor for SizeParam.
     * @param name Parameter name.
     * @param p Pointer to the size value.
     * @param def Default size.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    SizeParam(QString name, QSize* p, QSize def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;

        QWidget* container = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);

        widthSpin = new QSpinBox;
        widthSpin->setRange(0, INT_MAX);
        widthSpin->setValue(p->width());
        widthSpin->setToolTip(tip + " (Width)");

        heightSpin = new QSpinBox;
        heightSpin->setRange(0, INT_MAX);
        heightSpin->setValue(p->height());
        heightSpin->setToolTip(tip + " (Height)");

        layout->addWidget(new QLabel("Width:"));
        layout->addWidget(widthSpin);
        layout->addWidget(new QLabel("Height:"));
        layout->addWidget(heightSpin);

        widget = container;
    }

    void apply() override {
        ptr->setWidth(widthSpin->value());
        ptr->setHeight(heightSpin->value());
    }

    void reset() override {
        widthSpin->setValue(defVal.width());
        heightSpin->setValue(defVal.height());
    }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("width", QString::number(widthSpin->value()));
        w.writeAttribute("height", QString::number(heightSpin->value()));
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("width") && r.attributes().hasAttribute("height")) {
            widthSpin->setValue(r.attributes().value("width").toInt());
            heightSpin->setValue(r.attributes().value("height").toInt());
        }
        r.readNext();
    }
};

/**
 * @class RectParam
 * @brief Parameter class for handling rectangle values.
 */
class RectParam : public ParamBase {
    Q_OBJECT
    QRect       * ptr; ///< Pointer to the rectangle value.
    QRect       defVal; ///< Default rectangle value.
    QSpinBox    * xSpin; ///< Spin box for x coordinate.
    QSpinBox    * ySpin; ///< Spin box for y coordinate.
    QSpinBox    * widthSpin; ///< Spin box for width.
    QSpinBox    * heightSpin; ///< Spin box for height.

public:
    /**
     * @brief Constructor for RectParam.
     * @param name Parameter name.
     * @param p Pointer to the rectangle value.
     * @param def Default rectangle.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    RectParam(QString name, QRect* p, QRect def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;

        QWidget* container = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);

        xSpin = new QSpinBox;
        xSpin->setRange(INT_MIN, INT_MAX);
        xSpin->setValue(p->x());
        xSpin->setToolTip(tip + " (X coordinate)");

        ySpin = new QSpinBox;
        ySpin->setRange(INT_MIN, INT_MAX);
        ySpin->setValue(p->y());
        ySpin->setToolTip(tip + " (Y coordinate)");

        widthSpin = new QSpinBox;
        widthSpin->setRange(0, INT_MAX);
        widthSpin->setValue(p->width());
        widthSpin->setToolTip(tip + " (Width)");

        heightSpin = new QSpinBox;
        heightSpin->setRange(0, INT_MAX);
        heightSpin->setValue(p->height());
        heightSpin->setToolTip(tip + " (Height)");

        layout->addWidget(new QLabel("X:"));
        layout->addWidget(xSpin);
        layout->addWidget(new QLabel("Y:"));
        layout->addWidget(ySpin);
        layout->addWidget(new QLabel("W:"));
        layout->addWidget(widthSpin);
        layout->addWidget(new QLabel("H:"));
        layout->addWidget(heightSpin);

        widget = container;
    }

    void apply() override {
        *ptr = QRect(xSpin->value(), ySpin->value(), widthSpin->value(), heightSpin->value());
    }

    void reset() override {
        xSpin->setValue(defVal.x());
        ySpin->setValue(defVal.y());
        widthSpin->setValue(defVal.width());
        heightSpin->setValue(defVal.height());
    }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("x", QString::number(xSpin->value()));
        w.writeAttribute("y", QString::number(ySpin->value()));
        w.writeAttribute("width", QString::number(widthSpin->value()));
        w.writeAttribute("height", QString::number(heightSpin->value()));
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("x") && r.attributes().hasAttribute("y") &&
            r.attributes().hasAttribute("width") && r.attributes().hasAttribute("height")) {
            xSpin->setValue(r.attributes().value("x").toInt());
            ySpin->setValue(r.attributes().value("y").toInt());
            widthSpin->setValue(r.attributes().value("width").toInt());
            heightSpin->setValue(r.attributes().value("height").toInt());
        }
        r.readNext();
    }
};

/**
 * @class VariantParam
 * @brief Parameter class for handling QVariant values (fallback to string).
 */
class VariantParam : public ParamBase {
    Q_OBJECT
    QVariant    * ptr; ///< Pointer to the variant value.
    QVariant    defVal; ///< Default variant value.
    QLineEdit   * edit; ///< Line edit for user input.

public:
    /**
     * @brief Constructor for VariantParam.
     * @param name Parameter name.
     * @param p Pointer to the variant value.
     * @param def Default variant.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    VariantParam(QString name, QVariant* p, QVariant def, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = def;
        edit = new QLineEdit(p->toString(), this);
        edit->setToolTip(tip);
        widget = edit;
    }

    void apply() override { *ptr = edit->text(); }
    void reset() override { edit->setText(defVal.toString()); }

    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", edit->text());
        w.writeEndElement();
    }

    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value"))
            edit->setText(r.attributes().value("value").toString());
        r.readNext();
    }
};

/**
 * @class FloatParam
 * @brief Parameter class for handling single-precision floating-point values.
 */
class FloatParam : public ParamBase {
    Q_OBJECT
        float* ptr; ///< Pointer to the float value.
    float defVal; ///< Default value.
    QDoubleSpinBox* spin; ///< Spin box for user input.
public:
    /**
     * @brief Constructor for FloatParam.
     * @param name Parameter name.
     * @param p Pointer to the float value.
     * @param min Minimum allowed value.
     * @param max Maximum allowed value.
     * @param step Step size for the spin box.
     * @param tip Tooltip text.
     * @param parent Parent widget (default: nullptr).
     */
    FloatParam(QString name, float* p, float min, float max, float step, QString tip, QWidget* parent = nullptr)
        : ParamBase(parent) {
        this->name = name;
        ptr = p;
        defVal = *p;
        spin = new QDoubleSpinBox(this);
        spin->setRange(min, max);
        spin->setSingleStep(step);
        spin->setValue(*p);
        spin->setToolTip(tip);
        spin->setAlignment(Qt::AlignRight);
        widget = spin;
    }
    void apply() override { *ptr = static_cast<float>(spin->value()); }
    void reset() override { spin->setValue(defVal); }
    void save(QXmlStreamWriter& w) const override {
        w.writeStartElement(name);
        w.writeAttribute("value", QString::number(spin->value(), 'f', 6));
        w.writeEndElement();
    }
    void load(QXmlStreamReader& r) override {
        if (r.attributes().hasAttribute("value"))
            spin->setValue(r.attributes().value("value").toFloat());
        r.readNext();
    }
};

/* -------------------------------------
   Main Editor Dialog Class
   ------------------------------------- */

/**
* @class ParamsEditor
* @brief Main parameter editor dialog container
*
* Features:
* - Tab-based organization
* - Apply/Cancel semantics
* - XML import/export
* - Integrated help system
* - Automatic UI layout
*
* Usage workflow:
* 1. Create ParamsEditor instance
* 2. Add tabs with addTab()
* 3. Add parameters to tabs with addParam()
* 4. Optionally set help content
* 5. Call show() to display
*
* Serialization example:
* @code
* // Save configuration
* editor.saveToFile("settings.xml");
*
* // Load configuration
* editor.loadFromFile("settings.xml");
* @endcode
*/

class ParamsEditor : public QDialog {
    Q_OBJECT
    QTabWidget      * tabs; ///< Tab widget for organizing parameters.
    QTextBrowser    * helpBrowser = nullptr; ///< Browser for displaying help text.
    QVector<QVector<ParamBase*>> allParams; ///< List of parameters per tab.
    QPushButton     * applyBtn; ///< Button to apply changes.
    QPushButton     * cancelBtn; ///< Button to cancel changes.
    bool helpTabCreated = false; ///< Flag if help tab was created.

public:
    ParamsEditor(QWidget* parent = nullptr) : QDialog(parent) {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        tabs = new QTabWidget(this);
        mainLayout->addWidget(tabs);

        QHBoxLayout* btnLayout = new QHBoxLayout;
        applyBtn = new QPushButton("APPLY", this);
        cancelBtn = new QPushButton("CANCEL", this);
        btnLayout->addStretch();
        btnLayout->addWidget(applyBtn);
        btnLayout->addWidget(cancelBtn);
        mainLayout->addLayout(btnLayout);

        connect(applyBtn, &QPushButton::clicked, this, &ParamsEditor::onApplyClicked);
        connect(cancelBtn, &QPushButton::clicked, this, &ParamsEditor::onCancelClicked);
    }

    /**
     * @brief Add a new tab to the editor.
     * @param title Tab title.
     * @param icon Tab icon (default: empty).
     * @return Index of the new tab.
     */
    int addTab(const QString& title, const QIcon& icon = QIcon()) {
        QWidget* page = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(page);
        layout->addStretch();
        int index = tabs->count();
        tabs->addTab(page, icon, title);
        allParams.append(QVector<ParamBase*>()); // Initialize parameter vector for this tab
        return index;
    }

    /**
     * @brief Add a parameter to a tab.
     * @param tabIndex Index of the tab.
     * @param param Pointer to the parameter object.
     */
    void addParam(int tabIndex, ParamBase* param) {
        if (tabIndex < 0 || tabIndex >= allParams.size()) return;

        QWidget* tabWidget = tabs->widget(tabIndex);
        QVBoxLayout* layout = static_cast<QVBoxLayout*>(tabWidget->layout());
        QHBoxLayout* row = new QHBoxLayout;
        row->addStretch(); // Spinge tutto verso destra

        QLabel* label = new QLabel(param->name);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setFixedWidth(120); // Larghezza fissa per allineare le etichette
        row->addWidget(label);

        // Imposta una larghezza fissa per il widget del parametro per uniformità
        if (param->widget) {
            param->widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            param->widget->setMinimumWidth(200); // Larghezza minima per i widget
        }
        row->addWidget(param->widget);

        if (dynamic_cast<FilePathParam*>(param) || dynamic_cast<DirParam*>(param)) {
            QPushButton* browseBtn = new QPushButton("BROWSE");
            browseBtn->setFixedWidth(60);
            browseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            row->addWidget(browseBtn);
            param->browseButton = browseBtn;
            if (FilePathParam* fileParam = dynamic_cast<FilePathParam*>(param)) {
                QObject::connect(browseBtn, &QPushButton::clicked, fileParam, &FilePathParam::onBrowseClicked);
            }
            else if (DirParam* dirParam = dynamic_cast<DirParam*>(param)) {
                QObject::connect(browseBtn, &QPushButton::clicked, dirParam, &DirParam::onBrowseClicked);
            }
        }

        QPushButton* defBtn = new QPushButton("DEF");
        defBtn->setFixedWidth(40);
        defBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        defBtn->setToolTip("Set default value");
        row->addWidget(defBtn);
        param->defButton = defBtn;
        QObject::connect(defBtn, &QPushButton::clicked, [param]() { param->reset(); });

        layout->insertLayout(layout->count() - 1, row);
        allParams[tabIndex].append(param);
    }

    /**
    * @brief Set the main help text for the Help tab.
    * @param htmlText HTML-formatted help text.
    */
    void setMainHelp(const QString& htmlText) {
        if (!helpTabCreated) {
            helpBrowser = new QTextBrowser(this);
            QWidget* helpTab = new QWidget;
            QVBoxLayout* helpLayout = new QVBoxLayout(helpTab);
            helpLayout->addWidget(helpBrowser);
            tabs->addTab(helpTab, QIcon(), "Help");
            helpTabCreated = true;
        }
        helpBrowser->setHtml(htmlText);
    }

    /**
     * @brief Load parameters from an XML file.
     * @param filename Path to the XML file.
     */
    void loadFromFile(const QString& filename) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return;
        QXmlStreamReader reader(&file);
        while (!reader.atEnd()) {
            reader.readNext();
            if (reader.isStartElement()) {
                for (auto& tab : allParams)
                    for (auto* param : tab)
                        if (param->name == reader.name())
                            param->load(reader);
            }
        }
    }

    /**
     * @brief Save parameters to an XML file.
     * @param filename Path to the XML file.
     */
    void saveToFile(const QString& filename) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) return;
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("Params");
        for (const auto& tab : allParams)
            for (const auto* param : tab)
                param->save(writer);
        writer.writeEndElement();
        writer.writeEndDocument();
    }

    /**
     * @brief Show the dialog with a custom title and icon.
     * @param windowTitle Title of the dialog (default: "Params Editor").
     * @param icon Icon for the dialog (default: empty).
     */
    void show(const QString& windowTitle = "Params Editor", const QIcon& icon = QIcon()) {
        setWindowTitle(windowTitle);
        setWindowIcon(icon);
        if (tabs->count() > 1) {
            tabs->setCurrentIndex(0);
        }
        QDialog::show();
    }

private slots:
    /**
     * @brief Handle the Apply button click.
     */
    void onApplyClicked() {
        for (auto& tab : allParams)
            for (auto* param : tab)
                param->apply();
        accept();
    }

    /**
     * @brief Handle the Cancel button click.
     */
    void onCancelClicked() {
        reject();
    }
};

   /* -------------------------------------
   Property Binding System
   ------------------------------------- */

   /**
    * @class AdvancedPropertyAdapter
    * @brief Binds QObject properties to parameter editor
    *
    * Automatically creates appropriate parameter types based on:
    * - QMetaType of properties
    * - Supplemental metadata properties
    *
    * Supported metadata properties (for property "width"):
    * - widthDisplay: Display name (QString)
    * - widthTooltip: Tooltip text (QString)
    * - widthCategory: Tab category (QString)
    * - widthMin: Minimum value (double/int)
    * - widthMax: Maximum value (double/int)
    * - widthStep: Value step (double/int)
    * - widthEnumNames: Enum names (QStringList for enum properties)
    *
    * Usage:
    * @code
    * class AppConfig : public QObject {
    *     Q_OBJECT
    *     Q_PROPERTY(int threshold MEMBER m_threshold)
    *     Q_PROPERTY(QString title MEMBER m_title)
    *
    *     // Metadata for 'threshold' property
    *     Q_PROPERTY(int thresholdMin READ dummyConst STORED false)
    *     Q_PROPERTY(int thresholdMax READ dummyConst STORED false)
    *     Q_PROPERTY(QString thresholdTooltip READ dummyConst STORED false)
    * public:
    *     AppConfig() :
    *         m_threshold(50),
    *         m_title("Default Title")
    *     {
    *         setProperty("thresholdMin", 0);
    *         setProperty("thresholdMax", 100);
    *         setProperty("thresholdTooltip", "Processing sensitivity");
    *     }
    * private:
    *     int m_threshold;
    *     QString m_title;
    * };
    *
    * // In application code:
    * AppConfig config;
    * ParamsEditor editor;
    * AdvancedPropertyAdapter::bindObjectToEditor(&editor, &config);
    * @endcode
    */
class AdvancedPropertyAdapter : public QObject {
    Q_OBJECT
public:
    /// Property metadata container
    struct PropertyInfo {
        QString name;         ///< Original property name
        QString displayName;  ///< Display name
        QString tooltip;      ///< Help text
        QString category;     ///< Grouping category
        double min = 0;       ///< Minimum value
        double max = 0;       ///< Maximum value
        double step = 0;      ///< Value step
        QStringList enumNames;///< Enumeration options
    };

    /**
     * @brief Bind an object's properties to the editor, organizing by category.
     * @param editor The ParamsEditor instance.
     * @param obj The QObject with properties to bind.
     * @param defaultTabName Default tab name if no category is specified.
     */
    static void bindObjectToEditor(
        ParamsEditor* editor,
        QObject* obj,
        const QString& defaultTabName = "Properties"
    ) {
        const QMetaObject* meta = obj->metaObject();
        QMap<QString, int> categoryToTabIndex;

        // Create tabs based on categories
        for (int i = 0; i < meta->propertyCount(); ++i) {
            QMetaProperty prop = meta->property(i);
            if (!prop.isWritable()) continue;

            PropertyInfo info = extractPropertyInfo(obj, prop);
            QString category = info.category.isEmpty() ? defaultTabName : info.category;

            if (!categoryToTabIndex.contains(category)) {
                int tabIndex = editor->addTab(category);
                categoryToTabIndex[category] = tabIndex;
            }

            createParamForProperty(editor, categoryToTabIndex[category], obj, prop, info);
        }

        // Connessione per salvare i valori
        QObject::connect(editor, &ParamsEditor::accepted, [obj]() {
            qDebug() << "Properties updated";
            });
    }

private:
    static PropertyInfo extractPropertyInfo(QObject* obj, QMetaProperty prop) {
        PropertyInfo info;
        info.name = QString::fromUtf8(prop.name());

        // Rimuovi il prefisso 'm_' per il nome visualizzato
        QString cleanName = info.name;
        if (cleanName.startsWith("m_")) {
            cleanName = cleanName.mid(2);
        }
        cleanName.replace('_', ' ');

        // Estrai metadati
        QByteArray displayPropName = QByteArray(prop.name()) + "Display";
        QVariant displayVar = obj->property(displayPropName);
        info.displayName = displayVar.isValid() ? displayVar.toString() : cleanName;

        QByteArray tooltipPropName = QByteArray(prop.name()) + "Tooltip";
        QVariant tooltipVar = obj->property(tooltipPropName);
        info.tooltip = tooltipVar.isValid() ? tooltipVar.toString() : "";

        QByteArray categoryPropName = QByteArray(prop.name()) + "Category";
        QVariant categoryVar = obj->property(categoryPropName);
        info.category = categoryVar.isValid() ? categoryVar.toString() : "";

        // Estrai range per valori numerici
        QByteArray minPropName = QByteArray(prop.name()) + "Min";
        info.min = obj->property(minPropName).toDouble();

        QByteArray maxPropName = QByteArray(prop.name()) + "Max";
        info.max = obj->property(maxPropName).toDouble();

        QByteArray stepPropName = QByteArray(prop.name()) + "Step";
        info.step = obj->property(stepPropName).toDouble();

        // Gestione enumerazioni
        if (prop.isEnumType()) {
            QMetaEnum e = prop.enumerator();
            for (int j = 0; j < e.keyCount(); j++) {
                info.enumNames << e.key(j);
            }
        }

        return info;
    }

    static void createParamForProperty(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        int type = prop.userType();

        if (type == QMetaType::Int) {
            createIntParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::Double) {
            createDoubleParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::Float) {
            createFloatParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::Bool) {
            createBoolParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QString) {
            createStringParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QColor) {
            createColorParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QStringList) {
            createStringListParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QDate) {
            createDateParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QTime) {
            createTimeParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QPoint) {
            createPointParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QSize) {
            createSizeParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QRect) {
            createRectParam(editor, tabIndex, obj, prop, info);
        }
        else if (type == QMetaType::QVariant) {
            createVariantParam(editor, tabIndex, obj, prop, info);
        }
        else if (prop.isEnumType()) {
            createEnumParam(editor, tabIndex, obj, prop, info);
        }
        else {
            qWarning() << "Unsupported type:" << prop.typeName() << "for property" << info.name;
        }
    }

    static void createIntParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        int* valuePtr = new int(prop.read(obj).toInt());
        int min = (info.min != 0 || info.max != 0) ? info.min : INT_MIN;
        int max = (info.min != 0 || info.max != 0) ? info.max : INT_MAX;
        int step = info.step != 0 ? info.step : 1;

        IntParam* param = new IntParam(
            info.displayName,
            valuePtr,
            min, max, step,
            info.tooltip
        );

        setupParamConnections<IntParam, int>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createDoubleParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        double* valuePtr = new double(prop.read(obj).toDouble());
        double min = (info.min != 0 || info.max != 0) ? info.min : -DBL_MAX;
        double max = (info.min != 0 || info.max != 0) ? info.max : DBL_MAX;
        double step = info.step != 0 ? info.step : 0.1;

        DoubleParam* param = new DoubleParam(
            info.displayName,
            valuePtr,
            min, max, step,
            info.tooltip
        );

        setupParamConnections<DoubleParam, double>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createFloatParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        float* valuePtr = new float(prop.read(obj).toFloat());
        float min = (info.min != 0 || info.max != 0) ? static_cast<float>(info.min) : -FLT_MAX;
        float max = (info.min != 0 || info.max != 0) ? static_cast<float>(info.max) : FLT_MAX;
        float step = info.step != 0 ? static_cast<float>(info.step) : 0.1f;

        FloatParam* param = new FloatParam(
            info.displayName,
            valuePtr,
            min, max, step,
            info.tooltip
        );

        setupParamConnections<FloatParam, float>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createBoolParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        bool* valuePtr = new bool(prop.read(obj).toBool());
        bool defVal = *valuePtr;

        BoolParam* param = new BoolParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<BoolParam, bool>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createStringParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QString* valuePtr = new QString(prop.read(obj).toString());
        QString defVal = *valuePtr;

        StringParam* param = new StringParam(
            info.displayName,
            valuePtr,
            defVal,
            Qt::ImhNone,
            info.tooltip
        );

        setupParamConnections<StringParam, QString>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createColorParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QColor* valuePtr = new QColor(prop.read(obj).value<QColor>());
        QColor defVal = *valuePtr;

        ColorParam* param = new ColorParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<ColorParam, QColor>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createStringListParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QStringList* valuePtr = new QStringList(prop.read(obj).toStringList());
        QStringList defVal = *valuePtr;

        StringListParam* param = new StringListParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<StringListParam, QStringList>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createDateParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QDate* valuePtr = new QDate(prop.read(obj).toDate());
        QDate defVal = *valuePtr;

        DateParam* param = new DateParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<DateParam, QDate>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createTimeParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QTime* valuePtr = new QTime(prop.read(obj).toTime());
        QTime defVal = *valuePtr;

        TimeParam* param = new TimeParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<TimeParam, QTime>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createPointParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QPoint* valuePtr = new QPoint(prop.read(obj).toPoint());
        QPoint defVal = *valuePtr;

        PointParam* param = new PointParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<PointParam, QPoint>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createSizeParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QSize* valuePtr = new QSize(prop.read(obj).toSize());
        QSize defVal = *valuePtr;

        SizeParam* param = new SizeParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<SizeParam, QSize>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createRectParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QRect* valuePtr = new QRect(prop.read(obj).toRect());
        QRect defVal = *valuePtr;

        RectParam* param = new RectParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<RectParam, QRect>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createVariantParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        QVariant* valuePtr = new QVariant(prop.read(obj));
        QVariant defVal = *valuePtr;

        VariantParam* param = new VariantParam(
            info.displayName,
            valuePtr,
            defVal,
            info.tooltip
        );

        setupParamConnections<VariantParam, QVariant>(editor, tabIndex, param, obj, prop, valuePtr);
    }

    static void createEnumParam(
        ParamsEditor* editor,
        int tabIndex,
        QObject* obj,
        QMetaProperty prop,
        const PropertyInfo& info
    ) {
        int* valuePtr = new int(prop.read(obj).toInt());
        QStringList* options = new QStringList(info.enumNames);
        int defIndex = *valuePtr;

        ComboParam* param = new ComboParam(
            info.displayName,
            options,
            valuePtr,
            defIndex,
            info.tooltip
        );

        QObject::connect(param, &ComboParam::destroyed, [valuePtr, options]() {
            delete valuePtr;
            delete options;
            });

        QComboBox* combo = static_cast<QComboBox*>(param->widget);
        QObject::connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [obj, prop, valuePtr](int index) {
                *valuePtr = index;
                prop.write(obj, *valuePtr);
            });

        editor->addParam(tabIndex, param);
    }

    template<typename ParamType, typename ValueType>
    static void setupParamConnections(
        ParamsEditor* editor,
        int tabIndex,
        ParamType* param,
        QObject* obj,
        QMetaProperty prop,
        ValueType* valuePtr
    ) {
        QObject::connect(param, &ParamType::destroyed, [valuePtr]() {
            delete valuePtr;
            });

        QObject::connect(editor, &ParamsEditor::accepted, [obj, prop, valuePtr]() {
            prop.write(obj, *valuePtr);
            });

        editor->addParam(tabIndex, param);
    }
};

#endif // PARAMEDITOR_H
