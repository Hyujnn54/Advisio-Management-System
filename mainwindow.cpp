#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
#include "client.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QInputDialog>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QHoverEvent>
#include <QToolTip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isDarkTheme(true)
{
    ui->setupUi(this);

    // Set up the datetime edit widget for consultations
    QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(this);
    dateTimeEdit->setObjectName("consultation_datetime");
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->ClientForm->layout());
    if (gridLayout) {
        // Find the consultation_date widget position
        int row = 0, column = 0;
        for (int i = 0; i < gridLayout->count(); ++i) {
            int r, c, rs, cs;
            gridLayout->getItemPosition(i, &r, &c, &rs, &cs);
            if (gridLayout->itemAtPosition(r, c) &&
                gridLayout->itemAtPosition(r, c)->widget() == ui->searchDateButton_2) {
                row = r;
                column = c;
                break;
            }
        }

        // Replace the widget
        gridLayout->removeWidget(ui->searchDateButton_2);
        ui->searchDateButton_2->deleteLater();
        gridLayout->addWidget(dateTimeEdit, row, column);
    }

    // Update search date widget to search date time
    QDateTimeEdit *searchDateTimeEdit = new QDateTimeEdit(this);
    searchDateTimeEdit->setObjectName("searchDateTimeEdit");
    searchDateTimeEdit->setCalendarPopup(true);
    searchDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    searchDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    // Replace the searchDateEdit with searchDateTimeEdit
    if (ui->searchDateEdit) {
        QGridLayout *searchLayout = qobject_cast<QGridLayout*>(ui->searchDateEdit->parentWidget()->layout());
        if (searchLayout) {
            int row = 0, column = 0;
            for (int i = 0; i < searchLayout->count(); ++i) {
                int r, c, rs, cs;
                searchLayout->getItemPosition(i, &r, &c, &rs, &cs);
                if (searchLayout->itemAtPosition(r, c) &&
                    searchLayout->itemAtPosition(r, c)->widget() == ui->searchDateEdit) {
                    row = r;
                    column = c;
                    break;
                }
            }

            searchLayout->removeWidget(ui->searchDateEdit);
            ui->searchDateEdit->deleteLater();
            searchLayout->addWidget(searchDateTimeEdit, row, column);
        }
    }

    // Rename the search date button to reflect datetime search
    if (ui->searchDateButton) {
        ui->searchDateButton->setText("Search by Date & Time");
    }

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Connect existing buttons
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->add, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    connect(ui->Delet, &QPushButton::clicked, this, &MainWindow::on_removeButtonClicked);
    connect(ui->update, &QPushButton::clicked, this, &MainWindow::on_updateButtonClicked);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);

    // Connect search buttons
    connect(ui->searchNameButton, &QPushButton::clicked, this, &MainWindow::on_searchNameButton_clicked);
    connect(ui->searchSectorButton, &QPushButton::clicked, this, &MainWindow::on_searchSectorButton_clicked);
    connect(ui->searchDateButton, &QPushButton::clicked, this, &MainWindow::on_searchDateTimeButton_clicked);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButton_clicked);

    // Connect table header click for sorting
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &MainWindow::tableViewHeaderClicked);

    // Setup calendar functionality
    setupCalendarView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButtonclicked() {
    // Validate inputs first
    if (!validateInputs()) {
        return;
    }

    QString Name = ui->Name->text().trimmed();
    QString Sector = ui->Sector->text().trimmed();
    QString Contact = ui->Contact_info->text().trimmed();

    // Get the QDateTimeEdit widget we created programmatically
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime Cdate;

    if (dateTimeEdit) {
        Cdate = dateTimeEdit->dateTime();
    } else {
        // Fallback to just the date from the original widget if our replacement wasn't found
        QMessageBox::critical(this, "Error", "DateTime widget not found.");
        return;
    }

    int Consultant = ui->Consultant->text().toInt();

    // Create Client object
    Client c(Name, Sector, Contact, Cdate, Consultant);

    if (c.ajouter()) {
        QMessageBox::information(this, "Success", "Client added successfully!");
        QSqlQueryModel *model = Etmp.afficher(); // Fetch updated data
        ui->tableView->setModel(model); // Update the table view with new model

        // Clear inputs after successful addition
        ui->Name->clear();
        ui->Sector->clear();
        ui->Contact_info->clear();
        if (dateTimeEdit) {
            dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        }
        ui->Consultant->clear();

        // Update calendar view with new consultation
        updateCalendarConsultations();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add client.");
    }
}

void MainWindow::on_removeButtonClicked() {
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    qDebug() << "Selected Rows Count:" << selectedRows.count(); // Debug output

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a client to remove.");
        return;
    }

    int row = selectedRows.first().row();
    QString clientName = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();

    Client c;
    if (c.removeByName(clientName)) {
        QMessageBox::information(this, "Success", "Client removed successfully!");

        // Refresh the table view
        QSqlQueryModel *model = Etmp.afficher();
        ui->tableView->setModel(model);

        // Update calendar view after removal
        updateCalendarConsultations();
    } else {
        QMessageBox::critical(this, "Error", "Failed to remove the client.");
    }
}

void MainWindow::toggleSidebar() {
    // Check if the sidebar is currently visible
    bool isVisible = ui->sideMenu->isVisible();

    // Toggle visibility
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::on_updateButtonClicked() {
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a client to update.");
        return;
    }

    int row = selectedRows.first().row();
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());

    if (!model) {
        QMessageBox::critical(this, "Error", "Failed to get model data.");
        return;
    }

    // Get the data from the selected row
    QString name = model->data(model->index(row, 0)).toString();
    QString sector = model->data(model->index(row, 1)).toString();
    QString contact = model->data(model->index(row, 2)).toString();
    QDateTime dateTime = model->data(model->index(row, 3)).toDateTime();
    int consultant = model->data(model->index(row, 4)).toInt();

    // Create and show the update dialog
    UpdateClientDialog updateDialog(this);
    updateDialog.setClientData(name, sector, contact, dateTime, consultant);

    if (updateDialog.exec() == QDialog::Accepted) {
        // User confirmed the update
        QString newName = updateDialog.getName();
        QString newSector = updateDialog.getSector();
        QString newContact = updateDialog.getContactInfo();
        QDateTime newDateTime = updateDialog.getConsultationDateTime();
        int newConsultant = updateDialog.getConsultant();

        Client c;
        if (c.updateClient(name, newName, newSector, newContact, newDateTime, newConsultant)) {
            QMessageBox::information(this, "Success", "Client updated successfully!");

            // Refresh the table view
            QSqlQueryModel *updatedModel = Etmp.afficher();
            ui->tableView->setModel(updatedModel);

            // Update calendar after client update
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update the client.");
        }
    }
}

void MainWindow::toggleTheme() {
    isDarkTheme = !isDarkTheme;

    if (isDarkTheme) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

void MainWindow::applyDarkTheme() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void MainWindow::applyLightTheme() {
    qApp->setPalette(QApplication::style()->standardPalette());
    qApp->setStyleSheet("");
}

void MainWindow::on_searchNameButton_clicked() {
    QString searchText = ui->searchNameEdit->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Search", "Please enter a name to search.");
        return;
    }

    QSqlQueryModel* searchModel = Etmp.searchByName(searchText);
    ui->tableView->setModel(searchModel);

    // Update status
    ui->statusBar->showMessage(QString("Found %1 client(s) matching name: %2")
                                   .arg(searchModel->rowCount())
                                   .arg(searchText));
}

void MainWindow::on_searchSectorButton_clicked() {
    QString searchText = ui->searchSectorEdit->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Search", "Please enter a sector to search.");
        return;
    }

    QSqlQueryModel* searchModel = Etmp.searchBySector(searchText);
    ui->tableView->setModel(searchModel);

    // Update status
    ui->statusBar->showMessage(QString("Found %1 client(s) in sector: %2")
                                   .arg(searchModel->rowCount())
                                   .arg(searchText));
}

void MainWindow::on_searchDateTimeButton_clicked() {
    QDateTimeEdit *searchDateTimeEdit = findChild<QDateTimeEdit*>("searchDateTimeEdit");
    if (!searchDateTimeEdit) {
        QMessageBox::critical(this, "Error", "DateTime widget not found.");
        return;
    }
    
    QDateTime searchDateTime = searchDateTimeEdit->dateTime();
    QSqlQueryModel* searchModel = Etmp.searchByDateTime(searchDateTime);
    
    ui->tableView->setModel(searchModel);

    // Update status - use searchDateTime instead of searchDate
    ui->statusBar->showMessage(QString("Found %1 client(s) with date & time: %2")
                                   .arg(searchModel->rowCount())
                                   .arg(searchDateTime.toString("yyyy-MM-dd HH:mm")));
}

void MainWindow::on_resetSearchButton_clicked() {
    // Clear search fields
    ui->searchNameEdit->clear();
    ui->searchSectorEdit->clear();

    // Reset to show all clients
    QSqlQueryModel* model = Etmp.afficher();
    ui->tableView->setModel(model);

    // Update status
    ui->statusBar->showMessage("Showing all clients");
}

void MainWindow::tableViewHeaderClicked(int logicalIndex) {
    // Toggle sort order
    static Qt::SortOrder currentOrder = Qt::AscendingOrder;
    currentOrder = (currentOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;

    // Apply sorting
    QSqlQueryModel* sortedModel = Etmp.sortByColumn(logicalIndex, currentOrder);
    ui->tableView->setModel(sortedModel);

    // Update status message
    QString column;
    switch (logicalIndex) {
    case 0: column = "Name"; break;
    case 1: column = "Sector"; break;
    case 2: column = "Contact"; break;
    case 3: column = "Date"; break;
    case 4: column = "Consultant"; break;
    default: column = "Unknown"; break;
    }

    QString order = (currentOrder == Qt::AscendingOrder) ? "ascending" : "descending";
    ui->statusBar->showMessage(QString("Sorted by %1 (%2)").arg(column, order));
}

bool MainWindow::validateInputs() {
    // Validate name
    QString name = ui->Name->text().trimmed();
    if (!isValidName(name)) {
        QMessageBox::warning(this, "Input Error", "Name cannot be empty or contain numbers.");
        ui->Name->setFocus();
        return false;
    }

    // Validate sector
    QString sector = ui->Sector->text().trimmed();
    if (sector.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Sector cannot be empty.");
        ui->Sector->setFocus();
        return false;
    }

    // Validate contact info
    QString contact = ui->Contact_info->text().trimmed();
    if (contact.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Contact information cannot be empty.");
        ui->Contact_info->setFocus();
        return false;
    }

    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    if (!dateTimeEdit) {
        QMessageBox::critical(this, "Error", "DateTime widget not found.");
        return false;
    }
    
    QDateTime dateTime = dateTimeEdit->dateTime();
    if (!isValidDateTime(dateTime)) {
        QMessageBox::warning(this, "Input Error", "Date and time must be current or in the future.");
        dateTimeEdit->setFocus();
        return false;
    }

    // Validate consultant ID
    QString consultant = ui->Consultant->text().trimmed();
    if (!isValidConsultant(consultant)) {
        QMessageBox::warning(this, "Input Error", "Consultant ID must be a valid number.");
        ui->Consultant->setFocus();
        return false;
    }

    return true;
}

bool MainWindow::isValidName(const QString &name) {
    if (name.isEmpty()) return false;

    // Check if name contains any digits
    for (const QChar &ch : name) {
        if (ch.isDigit()) return false;
    }

    return true;
}

bool MainWindow::isValidDateTime(const QDateTime &dateTime) {
    // Check if dateTime is current or in the future
    return dateTime >= QDateTime::currentDateTime();
}

bool MainWindow::isValidDate(const QDate &date) {
    // Check if date is current date or in the future
    return date >= QDate::currentDate();
}

bool MainWindow::isValidConsultant(const QString &consultant) {
    if (consultant.isEmpty()) return false;

    bool ok;
    consultant.toInt(&ok);
    return ok;
}

void MainWindow::setupCalendarView()
{
    // Connect calendar signals
    connect(ui->consultationCalendar, &QCalendarWidget::selectionChanged,
            this, &MainWindow::on_consultationCalendar_selectionChanged);
    connect(ui->consultationCalendar, &QCalendarWidget::activated,
            this, &MainWindow::on_consultationCalendar_activated);

    // Create and install event filter for hover tooltips
    CalendarHoverItem *hoverItem = new CalendarHoverItem(ui->consultationCalendar, &consultationCountMap);
    ui->consultationCalendar->installEventFilter(hoverItem);

    // Set first day of week to Monday
    ui->consultationCalendar->setFirstDayOfWeek(Qt::Monday);

    // Load consultations and update calendar
    updateCalendarConsultations();

    // Initialize selected date info
    updateSelectedDateInfo(ui->consultationCalendar->selectedDate());
}

void MainWindow::updateCalendarConsultations()
{
    QMap<QDateTime, int> dtMap = Etmp.getConsultationCountsByDateTime();
    consultationCountMap.clear();
    
    // Group by date (ignore time)
    for (auto it = dtMap.begin(); it != dtMap.end(); ++it) {
        QDate date = it.key().date();
        consultationCountMap[date] += it.value();
    }
    
    // Highlight dates with consultations on the calendar
    highlightDatesWithConsultations();
}

void MainWindow::highlightDatesWithConsultations()
{
    // Clear any existing formatting
    ui->consultationCalendar->setDateTextFormat(QDate(), QTextCharFormat());

    // Format for highlighting dates with consultations
    QTextCharFormat consultationFormat;
    consultationFormat.setBackground(QColor(100, 150, 255, 100)); // Light blue background

    // Apply formatting to all dates with consultations
    QMapIterator<QDate, int> i(consultationCountMap);
    while (i.hasNext()) {
        i.next();
        ui->consultationCalendar->setDateTextFormat(i.key(), consultationFormat);
    }
}

void MainWindow::on_consultationCalendar_selectionChanged()
{
    // Update the information for the newly selected date
    QDate selectedDate = ui->consultationCalendar->selectedDate();
    updateSelectedDateInfo(selectedDate);
}

void MainWindow::on_consultationCalendar_activated(const QDate &date)
{
    // Handle double-click on date
    // This could be used to add a new consultation on this date
    QDateTime currentDateTime = QDateTime(date, QTime::currentTime());

    // Find the consultation_datetime widget and set its value
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    if (dateTimeEdit) {
        dateTimeEdit->setDateTime(currentDateTime);
    }

    // Switch to the Add Client tab
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::updateSelectedDateInfo(const QDate &date) {
    // Update labels
    ui->selectedDateLabel->setText(QString("Selected date: %1").arg(date.toString("yyyy-MM-dd")));

    int count = consultationCountMap.value(date, 0);
    ui->consultationCountLabel->setText(QString("Consultations: %1").arg(count));

    // Get consultations for this date
    QSqlQueryModel *model = Etmp.getConsultationsForDate(date);
    ui->dateConsultationsView->setModel(model);

    // Resize columns to contents
    ui->dateConsultationsView->resizeColumnsToContents();
}

// Event filter implementation for calendar hover tooltips
bool CalendarHoverItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == calendar) {
        if (event->type() == QEvent::HoverMove) {
            QHoverEvent *hoverEvent = static_cast<QHoverEvent*>(event);
            QPoint pos = hoverEvent->position().toPoint();

            // Access the underlying table view of the calendar widget
            QTableView* tableView = calendar->findChild<QTableView*>();
            QDate hoverDate;

            if (tableView) {
                QModelIndex index = tableView->indexAt(pos);
                if (index.isValid()) {
                    // Convert the model index to a date
                    int row = index.row();
                    int col = index.column();

                    // Calculate the date based on row and column
                    QDate firstDayOfMonth = calendar->selectedDate().addDays(-calendar->selectedDate().day() + 1);
                    int offset = firstDayOfMonth.dayOfWeek() - 1; // Adjust for first day of week

                    int day = (row * 7) + col - offset + 1;
                    if (day > 0 && day <= firstDayOfMonth.daysInMonth()) {
                        hoverDate = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), day);
                    } else {
                        // Fallback to selected date if calculation fails
                        hoverDate = calendar->selectedDate();
                    }
                }
            } else {
                // Fallback to last activated date if table view isn't accessible
                hoverDate = calendar->selectedDate();
            }

            if (hoverDate.isValid()) {
                // Show consultation count in tooltip
                int count = consultationCounts->value(hoverDate, 0);

                if (count > 0) {
                    QString tooltipText = QString("%1: %2 consultation(s)")
                    .arg(hoverDate.toString("yyyy-MM-dd"))
                        .arg(count);
                    QToolTip::showText(calendar->mapToGlobal(hoverEvent->position().toPoint()),
                                       tooltipText, calendar);
                } else {
                    QToolTip::hideText();
                }
            }
        }
    }

    return QObject::eventFilter(watched, event);
}
