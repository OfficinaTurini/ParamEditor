#include "parameditor.h"
#include "test.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Example variables for parameters
    double          doubleVal = 3.1415;
    int             intVal = 42;
    QString         stringVal = "Hello World";
    QStringList     comboOptions = { "Option 1", "Option 2", "Option 3" };
    int             comboIndex = 1;
    QColor          colorVal = Qt::blue;
    QString         filePath = "C:/test.txt";
    QString         dirPath = "C:/Documents";
    bool            boolVal = true;
    QFont           fontVal = QApplication::font();
    QString         password = "secret";
    QDateTime       dateTimeVal = QDateTime::currentDateTime();
    
    // Example class
    ExtendedConfig config;
    // Create parameter editor
    ParamsEditor editor;

    // Add tabs
    int generalTab = editor.addTab("Simple");
    int fileTab = editor.addTab("File Settings");

    // Add parameters to the General tab
    editor.addParam(generalTab, new DoubleParam("Pi", &doubleVal, 0.0, 10.0, 0.01, "Approximation of Pi"));
    editor.addParam(generalTab, new IntParam("Answer", &intVal, 0, 100, 1, "The answer to everything"));
    editor.addParam(generalTab, new StringParam("Message", &stringVal, "Default", Qt::ImhNone, "Test message"));
    editor.addParam(generalTab, new ComboParam("Options", &comboOptions, &comboIndex, 0, "Select an option"));
    editor.addParam(generalTab, new ColorParam("Color", &colorVal, Qt::red, "Background color"));
    editor.addParam(generalTab, new BoolParam("Enable", &boolVal, false, "Enable feature"));
    editor.addParam(generalTab, new FontParam("Font", &fontVal, QFont("Arial", 10), "Text font"));
    editor.addParam(generalTab, new PasswordParam("Admin Password", &password, "", "Enter admin password"));
    editor.addParam(generalTab, new DateTimeParam("Appointment", &dateTimeVal, QDateTime::currentDateTime(), "Meeting time", "dd/MM/yyyy hh:mm"));

    // Add parameters to the File Settings tab
    editor.addParam(fileTab, new FilePathParam("Config File", &filePath, "config.ini", "Configuration file"));
    editor.addParam(fileTab, new DirParam("Data Dir", &dirPath, "data/", "Data directory"));

    AdvancedPropertyAdapter::bindObjectToEditor(&editor, &config, "Class");

    // Set the help text
    editor.setMainHelp(
        "<h1>Parameters Editor Help</h1>"
        "<p>This is a test application for the <b>ParamsEditor</b> component.</p>"
        "<p>You can modify parameters in different tabs and see the results.</p>"
        "<ul>"
        "<li><b>DEF</b> - Reset to default value</li>"
        "<li><b>BROWSE</b> - Open file/directory dialog</li>"
        "<li><b>APPLY</b> - Save changes</li>"
        "<li><b>CANCEL</b> - Discard changes</li>"
        "</ul>"
    );

    // Show editor and manage result
    editor.setWindowTitle("Test Parameters Editor");
    int result = editor.exec();  // Run the dialog modally

    // After closing the window, print the changed values
    if (result == QDialog::Accepted) {
        qDebug() << "=== Modified Values ===";
        qDebug() << "Pi:" << doubleVal;
        qDebug() << "Answer:" << intVal;
        qDebug() << "Message:" << stringVal;
        qDebug() << "Selected Option:" << comboOptions[comboIndex] << "(" << comboIndex << ")";
        qDebug() << "Color:" << colorVal.name();
        qDebug() << "Config File:" << filePath;
        qDebug() << "Data Dir:" << dirPath;
        qDebug() << "Enabled:" << boolVal;
        qDebug() << "Font:" << fontVal.toString();
        qDebug() << "Password:" << password;
        qDebug() << "Appointment:" << dateTimeVal.toString("dd/MM/yyyy hh:mm");
    }
    else {
        qDebug() << "Changes canceled";
    }

    return 0;
}