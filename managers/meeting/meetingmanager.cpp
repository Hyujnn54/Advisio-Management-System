#include "meetingmanager.h"
#include "ui_mainwindow.h"
#include "../../dialog/updatemeeting/updatemeeting.h" // Keep this
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFontMetrics>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QSqlQuery>
#include <QSqlError>

MeetingManager::MeetingManager(bool dbConnected, QObject *parent)
    : QObject(parent), m_dbConnected(dbConnected), ui(nullptr), notificationManager(nullptr)
{
}

MeetingManager::~MeetingManager()
{
}

void MeetingManager::setNotificationManager(NotificationManager *manager)
{
    notificationManager = manager;
}

void MeetingManager::initialize(Ui::MainWindow *ui)
{
    qDebug() << "Entering MeetingManager::initialize";
    this->ui = ui;
    if (!ui) {
        qDebug() << "Error: ui is null";
        return;
    }

    // Block signals during initialization
    ui->meetingTableWidget->blockSignals(true);

    // Connect signals and slots
    connect(ui->meetingAddButton, &QPushButton::clicked, this, &MeetingManager::handleAddButtonClick);
    connect(ui->meetingDeleteButton, &QPushButton::clicked, this, &MeetingManager::handleDeleteButtonClick);
    connect(ui->meetingUpdateButton, &QPushButton::clicked, this, &MeetingManager::handleUpdateButtonClick);
    connect(ui->meetingResetSearchButton, &QPushButton::clicked, this, &MeetingManager::handleSearchButtonClick);
    connect(ui->meetingSearchInput, &QLineEdit::textChanged, this, &MeetingManager::handleSearchTextChanged);
    connect(ui->meetingTableWidget, &QTableWidget::itemSelectionChanged, this, &MeetingManager::updateInputFields);
    connect(ui->meetingGenerateQRCodeButton, &QPushButton::clicked, this, &MeetingManager::handleGenerateQRCodeButtonClick);
    connect(ui->meetingExportPdfButton, &QPushButton::clicked, this, &MeetingManager::handleExportPdfButtonClick);
    connect(ui->meetingTabWidget, &QTabWidget::currentChanged, this, &MeetingManager::handleTabChanged);
    connect(ui->meetingSearchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MeetingManager::handleSortCriteriaChanged);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MeetingManager::handleStatisticsButtonClick);

    // Set up table properties
    ui->meetingTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->meetingTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->meetingTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Refresh table
    refreshTableWidget();

    // Apply theme styles
    applyThemeStyles();

    // Re-enable signals
    ui->meetingTableWidget->blockSignals(false);
    qDebug() << "Exiting MeetingManager::initialize";
}

void MeetingManager::handleAddButtonClick()
{
    QString title = ui->meetingTitleInput->text();
    QString organiser = ui->meetingOrganiserInput->text();
    QString participant = ui->meetingParticipantInput->text();
    QString agenda = ui->meetingAgendaComboBox->currentText();
    int duration = ui->meetingDurationInput->text().toInt();
    QDateTime datem = ui->meetingDateTimeEdit->dateTime();

    if (title.isEmpty() || organiser.isEmpty() || participant.isEmpty() || agenda.isEmpty() || duration <= 0) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in all fields correctly.");
        return;
    }

    meeting m(title, organiser, participant, agenda, duration, datem);
    if (m.add()) {
        QMessageBox::information(nullptr, "Success", "Meeting added successfully!");
        refreshTableWidget();
        if (notificationManager) {
            notificationManager->addNotification("Meeting Added", "Title: " + title, "Organiser: " + organiser, -1);
        } else {
            qDebug() << "Warning: notificationManager is null in handleAddButtonClick";
        }
    } else {
        QMessageBox::critical(nullptr, "Error", "Failed to add meeting.");
    }
}

void MeetingManager::handleDeleteButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to delete.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    QString title = ui->meetingTableWidget->item(row, 1)->text();

    meeting m;
    if (m.deleteMeeting(id)) {
        QMessageBox::information(nullptr, "Success", "Meeting deleted successfully.");
        refreshTableWidget();
        if (notificationManager) {
            notificationManager->addNotification("Meeting Deleted", "Meeting: " + title, "Deleted meeting with ID " + QString::number(id), -1);
        }
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to delete meeting. Check the database connection.");
    }
}

void MeetingManager::handleUpdateButtonClick()
{
    if (!m_dbConnected) {
        QMessageBox::warning(ui->centralwidget, "Database Error", "Cannot update meeting: Database is not connected.");
        return;
    }
    // Get the selected row from the table widget
    QTableWidget *table = ui->meetingTableWidget;
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(ui->centralwidget, "Error", "Please select a meeting to update.");
        return;
    }

    // Assuming the meeting ID is stored in the first column
    bool ok;
    int meetingId = table->item(row, 0)->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(ui->centralwidget, "Error", "Invalid meeting ID.");
        return;
    }

    // Fetch the meeting from the database
    meeting m;
    QSqlQuery query;
    query.prepare("SELECT * FROM MEETING WHERE ID = :id");
    query.bindValue(":id", meetingId);
    if (query.exec() && query.next()) {
        m.setId(query.value("ID").toInt());
        m.setTitle(query.value("TITLE").toString());
        m.setOrganiser(query.value("ORGANISER").toString());
        m.setParticipant(query.value("PARTICIPANT").toString());
        m.setAgenda(query.value("AGENDA").toString());
        m.setDuration(query.value("DURATION").toInt());
        m.setDatem(query.value("DATEM").toDateTime());
        m.setEmployeeId(query.value("EMPLOYEE_ID").toInt());
        m.setClientId(query.value("CLIENT_ID").toInt());
        m.setResourceId(query.value("RESOURCE_ID").toInt());
    } else {
        QMessageBox::warning(ui->centralwidget, "Error", "Failed to load meeting data: " + query.lastError().text());
        return;
    }

    // Show the update dialog with ui->centralwidget as parent
    UpdateMeeting dialog(ui->centralwidget, &m);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTableWidget();
        QMessageBox::information(ui->centralwidget, "Success", "Meeting updated successfully.");
        if (notificationManager) {
            notificationManager->addNotification("Meeting Updated", "Title: " + m.getTitle(), "Meeting ID: " + QString::number(meetingId), -1);
        }
    }
}

void MeetingManager::handleSearchButtonClick()
{
    QString searchText = ui->meetingSearchInput->text();
    if (searchText.isEmpty()) {
        refreshTableWidget();
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);
    if (model->rowCount() == 0) {
        QMessageBox::information(nullptr, "No Results", "No meetings found with the given title.");
        delete model;
        return;
    }

    ui->meetingTableWidget->setRowCount(0);
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->meetingTableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->meetingTableWidget->setItem(row, col, item);
        }
    }
    delete model;
}

void MeetingManager::handleSearchTextChanged(const QString &searchText)
{
    if (searchText.isEmpty()) {
        refreshTableWidget();
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);
    ui->meetingTableWidget->setRowCount(0);
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->meetingTableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->meetingTableWidget->setItem(row, col, item);
        }
    }
    delete model;
}

void MeetingManager::handleGenerateQRCodeButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to generate a QR code.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    meeting m(ui->meetingTableWidget->item(row, 1)->text(),
              ui->meetingTableWidget->item(row, 2)->text(),
              ui->meetingTableWidget->item(row, 3)->text(),
              ui->meetingTableWidget->item(row, 4)->text(),
              ui->meetingTableWidget->item(row, 5)->text().replace(" min", "").toInt(),
              QDateTime::fromString(ui->meetingTableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm"));
    m.setId(id);

    QPixmap qrCode = m.generateQRCode();
    if (qrCode.isNull()) {
        QMessageBox::critical(nullptr, "Error", "Failed to generate QR code.");
        return;
    }

    QMessageBox qrDialog(nullptr);
    qrDialog.setWindowTitle("Meeting QR Code");
    qrDialog.setText("Scan this QR code for full meeting details:");
    qrDialog.setIconPixmap(qrCode.scaled(200, 200, Qt::KeepAspectRatio));
    qrDialog.exec();

    // Optional: Save QR code to file
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save QR Code", "Meeting_" + QString::number(id) + ".png", "PNG Files (*.png)");
    if (!filePath.isEmpty()) {
        qrCode.save(filePath);
        if (notificationManager) {
            notificationManager->addNotification("QR Code Generated", "Meeting ID: " + QString::number(id),
                                                 "QR code saved to " + filePath, -1);
        }
        QMessageBox::information(nullptr, "Success", "QR code saved successfully!");
    }
}

void MeetingManager::handleExportPdfButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to export.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    meeting m(ui->meetingTableWidget->item(row, 1)->text(),
              ui->meetingTableWidget->item(row, 2)->text(),
              ui->meetingTableWidget->item(row, 3)->text(),
              ui->meetingTableWidget->item(row, 4)->text(),
              ui->meetingTableWidget->item(row, 5)->text().replace(" min", "").toInt(),
              QDateTime::fromString(ui->meetingTableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm"));
    m.setId(id);

    QPixmap qrCode = m.generateQRCode();
    if (qrCode.isNull()) {
        QMessageBox::critical(nullptr, "Error", "Failed to generate QR code for PDF.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Export Meeting to PDF", "Meeting_" + QString::number(id) + ".pdf", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    QPainter painter(&printer);
    QRect rect = painter.viewport();
    int side = qMin(rect.width(), rect.height());

    painter.setViewport(0, 0, side, side + 200);
    painter.setWindow(0, 0, side, side + 200);

    painter.drawText(10, 20, QString("Meeting Details"));
    painter.drawText(10, 40, QString("ID: %1").arg(id));
    painter.drawText(10, 60, QString("Title: %1").arg(m.getTitle()));
    painter.drawText(10, 80, QString("Organiser: %1").arg(m.getOrganiser()));
    painter.drawText(10, 100, QString("Participant: %1").arg(m.getParticipant()));
    painter.drawText(10, 120, QString("Agenda: %1").arg(m.getAgenda()));
    painter.drawText(10, 140, QString("Duration: %1 min").arg(m.getDuration()));
    painter.drawText(10, 160, QString("Date: %1").arg(m.getDatem().toString("yyyy-MM-dd hh:mm")));

    QPixmap scaledQrCode = qrCode.scaled(200, 200, Qt::KeepAspectRatio);
    painter.drawPixmap(10, 180, scaledQrCode);

    painter.end();

    if (notificationManager) {
        notificationManager->addNotification("PDF Exported", "Meeting ID: " + QString::number(id),
                                             "PDF saved to " + filePath, -1);
    }
    QMessageBox::information(nullptr, "Success", "Meeting exported to PDF successfully!");
}

void MeetingManager::handleTabChanged(int index)
{
    if (index == 0) { // Add Meeting tab
        ui->meetingTitleInput->clear();
        ui->meetingOrganiserInput->clear();
        ui->meetingParticipantInput->clear();
        ui->meetingAgendaComboBox->setCurrentIndex(0);
        ui->meetingDurationInput->clear();
        ui->meetingDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }
}

void MeetingManager::handleSortCriteriaChanged(int index)
{
    if (!m_dbConnected) return;

    QString column;
    switch (index) {
    case 0: column = "id"; break;
    case 1: column = "title"; break;
    case 2: column = "organiser"; break;
    case 3: column = "participant"; break;
    case 4: column = "agenda"; break;
    case 5: column = "duration"; break;
    case 6: column = "datem"; break;
    default: return;
    }

    QString queryStr = QString("SELECT id, title, organiser, participant, agenda, duration, datem FROM meeting ORDER BY %1").arg(column);
    QSqlQuery query(queryStr);

    ui->meetingTableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->meetingTableWidget->insertRow(row);
        ui->meetingTableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->meetingTableWidget->setItem(row, 1, new QTableWidgetItem(query.value("title").toString()));
        ui->meetingTableWidget->setItem(row, 2, new QTableWidgetItem(query.value("organiser").toString()));
        ui->meetingTableWidget->setItem(row, 3, new QTableWidgetItem(query.value("participant").toString()));
        ui->meetingTableWidget->setItem(row, 4, new QTableWidgetItem(query.value("agenda").toString()));
        ui->meetingTableWidget->setItem(row, 5, new QTableWidgetItem(query.value("duration").toString() + " min"));
        ui->meetingTableWidget->setItem(row, 6, new QTableWidgetItem(query.value("datem").toDateTime().toString("yyyy-MM-dd hh:mm")));
        row++;
    }

    ui->meetingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->meetingTableWidget->setColumnWidth(6, 150);
}

void MeetingManager::handleStatisticsButtonClick()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    // Defer to MainWindow's ChartWindow
    QMessageBox::information(nullptr, "Statistics", "Statistics are available via the main Statistics button.");
}

void MeetingManager::handleRefreshStatsButtonClick()
{
    // Placeholder: meetingRefreshStatsButton not in mainwindow.ui
    QMessageBox::information(nullptr, "Refresh Stats", "Statistics refresh is currently disabled.");
}

void MeetingManager::updateInputFields()
{
    qDebug() << "Entering updateInputFields";
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        qDebug() << "No row selected in updateInputFields";
        return;
    }

    auto getItemText = [&](int col, const QString& defaultValue = "") {
        QTableWidgetItem* item = ui->meetingTableWidget->item(row, col);
        return item ? item->text() : defaultValue;
    };

    ui->meetingTitleInput->setText(getItemText(1));
    ui->meetingOrganiserInput->setText(getItemText(2));
    ui->meetingParticipantInput->setText(getItemText(3));
    ui->meetingAgendaComboBox->setCurrentText(getItemText(4));
    ui->meetingDurationInput->setText(getItemText(5).replace(" min", ""));
    qDebug() << "Exiting updateInputFields";
}

void MeetingManager::refreshTableWidget()
{
    qDebug() << "Entering refreshTableWidget, m_dbConnected =" << m_dbConnected;
    if (!ui || !ui->meetingTableWidget) {
        qDebug() << "Error: ui or meetingTableWidget is null";
        return;
    }

    if (!m_dbConnected) {
        qDebug() << "Error: Database is not connected";
        ui->meetingTableWidget->setRowCount(0);
        ui->meetingTableWidget->setColumnCount(7);
        ui->meetingTableWidget->setHorizontalHeaderLabels({"ID", "Title", "Organiser", "Participant", "Agenda", "Duration", "Date and Time"});
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.afficher();
    if (!model) {
        qDebug() << "Error: Failed to create QSqlQueryModel";
        ui->meetingTableWidget->setRowCount(0);
        return;
    }

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error in refreshTableWidget:" << model->lastError().text();
        delete model;
        return;
    }

    ui->meetingTableWidget->setColumnCount(7);
    ui->meetingTableWidget->setHorizontalHeaderLabels({"ID", "Title", "Organiser", "Participant", "Agenda", "Duration", "Date and Time"});
    ui->meetingTableWidget->setRowCount(0);

    qDebug() << "refreshTableWidget: Model has" << model->rowCount() << "rows";
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->meetingTableWidget->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            QVariant data = model->data(model->index(row, col));
            QString displayData;
            if (data.isNull()) {
                qDebug() << "Null data at row" << row << "col" << col;
                displayData = "N/A";
            } else if (col == 5) {
                displayData = data.toString() + " min";
            } else if (col == 6) {
                QDateTime dateTime = data.toDateTime();
                if (dateTime.isValid()) {
                    displayData = dateTime.toString("yyyy-MM-dd hh:mm");
                } else {
                    qDebug() << "Invalid DateTime at row" << row << "col" << col << "data:" << data.toString();
                    displayData = "Invalid Date";
                }
            } else {
                displayData = data.toString();
            }
            QTableWidgetItem* item = new QTableWidgetItem(displayData);
            if (item) {
                ui->meetingTableWidget->setItem(row, col, item);
            } else {
                qDebug() << "Error: Failed to create QTableWidgetItem at row" << row << "col" << col;
            }
        }
    }

    ui->meetingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->meetingTableWidget->setColumnWidth(6, 150);

    delete model;
    qDebug() << "Exiting refreshTableWidget";
}

void MeetingManager::applyThemeStyles()
{
    ui->meetingTableWidget->setStyleSheet("QTableView { background-color: #FFFFFF; border: 1px solid #D3DCE6; selection-background-color: #A1B8E6; }"
                                          "QHeaderView::section { background-color: #3A5DAE; color: white; }");
}
