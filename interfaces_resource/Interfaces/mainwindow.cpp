#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "resource.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <Qstring>
#include <QDate>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QTableView>
#include <QInputDialog>
#include <QBuffer>
#include <QFileDialog>
#include <QPixmap>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    /*, ui(new Ui::MainWindow)
    , selectedResourceId(-1)
    , isDarkTheme(false)
    , imageData(QByteArray())*/
    , ui(new Ui::MainWindow)          // Matches declaration order
    , selectedResourceId(-1)          // Matches declaration order
    , imageData(QByteArray())         // Matches declaration order
    , isDarkTheme(false)              // Matches declaration order
    , sidebarWidget(nullptr)
{
    ui->setupUi(this);

    applyLightTheme(); // Set default theme

    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClick);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &MainWindow::on_cancelFormButton_clicked);

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(60);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabWidget->setTabText(0, "Add Resources");
    ui->tabWidget->setTabText(1, "Manage Resources");
    ui->tabWidget->setTabText(2, "New Calendar Name");
    ui->tabWidget->setTabText(3, "Statistics");
    //loadResourcesIntoTable();
        refreshTableWidget();
   //connect(ui->confirmFormButton, &QPushButton::clicked, this, &MainWindow::on_confirmFormButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//EL S7I7A
/*void MainWindow::on_btnSelectImage_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap image(filePath);
        ui->lblImagePreview->setPixmap(image.scaled(ui->lblImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}*/

void MainWindow::on_btnSelectImage_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        // Load the image into a QPixmap for preview
        QPixmap image;
        if (!image.load(filePath)) {
            QMessageBox::warning(this, "Image Load Error", "Failed to load the image. Please select a valid image file.");
            qDebug() << "Failed to load image into QPixmap:" << filePath;
            return;
        }

        // Scale and set the preview
        ui->lblImagePreview->setPixmap(image.scaled(ui->lblImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Convert the image to QByteArray for database storage
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            imageData = file.readAll(); // Store the raw image data
            file.close();

            if (imageData.isEmpty()) {
                QMessageBox::warning(this, "Image Error", "The selected image file is empty or could not be read.");
                qDebug() << "Image data is empty after reading:" << filePath;
                ui->lblImagePreview->clear();
                imageData.clear();
                return;
            }

            qDebug() << "Image loaded successfully, size:" << imageData.size() << "bytes";
        } else {
            QMessageBox::warning(this, "File Error", "Failed to open the image file: " + file.errorString());
            qDebug() << "Failed to open image file:" << file.errorString();
            ui->lblImagePreview->clear();
            return;
        }
    } else {
        qDebug() << "No file selected by user.";
    }
}


void MainWindow::on_cancelFormButton_clicked()
{
    // Clear all form fields
    ui->nameLineEdit->clear();
    ui->typeComboBox->setCurrentIndex(0); // Reset to first item (assumed default)
    ui->brandLineEdit->clear();
    ui->quantityLineEdit->clear();
    ui->purchaseDateEdit->setDate(QDate::currentDate()); // Reset to today
    imageData.clear(); // Clear image data
    ui->lblImagePreview->clear(); // Clear image preview

    qDebug() << "Form cleared by Cancel button.";
}

///EL S7I7A
/*void MainWindow::on_confirmFormButton_clicked() {
    // Retrieve values from the form
    QString name = ui->nameLineEdit->text();
    QString type = ui->typeComboBox->currentText();
    QString brand = ui->brandLineEdit->text();
    int quantity = ui->quantityLineEdit->text().toInt(); // Ensure conversion is correct
    QString purchase_date = ui->purchaseDateEdit->date().toString("yyyy-MM-dd");

    // Debugging output
    qDebug() << "Converted quantity:" << quantity;
    qDebug() << "Formatted date:" << purchase_date;

    // Prepare and execute your SQL query here
    QSqlQuery query;
    query.prepare("INSERT INTO resources (name, type, brand, quantity, purchase_date) "
                  "VALUES (:name, :type, :brand, :quantity, TO_DATE(:purchase_date, 'YYYY-MM-DD'))");

    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":brand", brand);
    query.bindValue(":quantity", quantity);
    query.bindValue(":purchase_date", purchase_date);

    if (query.exec()) {
        qDebug() << "Resource added successfully!";
        QMessageBox::information(this, "Success", "Resource added successfully.");

        // Clear form fields after successful insertion
        ui->nameLineEdit->clear();
        ui->typeComboBox->setCurrentIndex(0); // Reset to default selection
        ui->brandLineEdit->clear();
        ui->quantityLineEdit->clear();
        ui->purchaseDateEdit->setDate(QDate::currentDate()); // Reset to current date

        refreshTableWidget();
    } else {
        qDebug() << "Failed to add resource:" << query.lastError().text();
        QMessageBox::warning(this, "Error", "Failed to add Resource.");
    }
}
*/

/*void MainWindow::on_confirmFormButton_clicked() {
    // Retrieve values from the form
    QString name = ui->nameLineEdit->text();
    QString type = ui->typeComboBox->currentText();
    QString brand = ui->brandLineEdit->text();
    bool ok;
    int quantity = ui->quantityLineEdit->text().toInt(&ok);
    QDate purchase_date = ui->purchaseDateEdit->date();

    // Validation
    if (name.isEmpty() || !ok) {
        QMessageBox::warning(this, "Input Error", "Please fill all required fields correctly (Name and Quantity are mandatory).");
        return;
    }

    // Create a Resource object and set its properties
    Resource resource;
    resource.setName(name);
    resource.setType(type);
    resource.setBrand(brand);
    resource.setQuantity(quantity);
    resource.setPurchaseDate(purchase_date);
    resource.setImage(imageData); // Set the image data

    // Add the resource to the database
    if (resource.addResource()) {
        qDebug() << "Resource added successfully!";
        QMessageBox::information(this, "Success", "Resource added successfully!");

        // Clear form fields after successful insertion
        ui->nameLineEdit->clear();
        ui->typeComboBox->setCurrentIndex(0);
        ui->brandLineEdit->clear();
        ui->quantityLineEdit->clear();
        ui->purchaseDateEdit->setDate(QDate::currentDate());
        imageData.clear(); // Clear the image data
        ui->lblImagePreview->clear(); // Clear the preview

        refreshTableWidget();
    } else {
        qDebug() << "Failed to add resource:" << QSqlQuery().lastError().text();
        QMessageBox::warning(this, "Error", "Failed to add resource: " + QSqlQuery().lastError().text());
    }
}*/


#include <QMessageBox>
#include <QDate>
#include <QDebug>

void MainWindow::on_confirmFormButton_clicked()
{
    // Retrieve values from the form
    QString name = ui->nameLineEdit->text().trimmed(); // Remove leading/trailing whitespace
    QString type = ui->typeComboBox->currentText().trimmed();
    QString brand = ui->brandLineEdit->text().trimmed();
    bool ok;
    int quantity = ui->quantityLineEdit->text().toInt(&ok);
    QDate purchase_date = ui->purchaseDateEdit->date();
    QDate currentDate = QDate::currentDate(); // Current date: March 09, 2025
   // purchase_date.toString("yyyy-MM-dd")

    // Validation checks
    // 1. Check if any field is empty
    if (name.isEmpty() || type.isEmpty() || brand.isEmpty() || !ok) {
        QMessageBox::warning(this, "Input Error", "All fields (Name, Type, Brand, Quantity) must be filled correctly.");
        return;
    }

    // 2. Check if fields start with uppercase
    auto startsWithUpper = [](const QString& str) {
        return !str.isEmpty() && str[0].isUpper();
    };
    if (!startsWithUpper(name) || !startsWithUpper(type) || !startsWithUpper(brand)) {
        QMessageBox::warning(this, "Input Error", "Name, Type, and Brand must start with an uppercase letter.");
        return;
    }

    // 3. Check quantity: must be > 0
    if (!ok || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "Quantity must be a positive number greater than zero.");
        return;
    }

    // 4. Check purchase date: must not be in the future
    if (purchase_date > currentDate) {
        QMessageBox::warning(this, "Input Error", "Purchase date cannot be in the future.");
        return;
    }

    // 5. Check if an image is selected (assuming imageData is a QByteArray or similar)
    if (imageData.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select an image.");
        return;
    }

    // If all validations pass, create and populate the Resource object
    Resource resource;
    resource.setName(name);
    resource.setType(type);
    resource.setBrand(brand);
    resource.setQuantity(quantity);
    resource.setPurchaseDate(purchase_date);
    resource.setImage(imageData); // Set the image data

    // Add the resource to the database
    if (resource.addResource()) {
        qDebug() << "Resource added successfully!";
        QMessageBox::information(this, "Success", "Resource added successfully!");

        // Clear form fields after successful insertion
        ui->nameLineEdit->clear();
        ui->typeComboBox->setCurrentIndex(0);
        ui->brandLineEdit->clear();
        ui->quantityLineEdit->clear();
        ui->purchaseDateEdit->setDate(QDate::currentDate());
        imageData.clear(); // Clear the image data
        ui->lblImagePreview->clear(); // Clear the preview

        refreshTableWidget();
    } else {
        qDebug() << "Failed to add resource:" << QSqlQuery().lastError().text();
        QMessageBox::warning(this, "Error", "Failed to add resource: " + QSqlQuery().lastError().text());
    }
}



/*void MainWindow::refreshTableWidget() {
    Resource R;
    QSqlQueryModel* model = R.afficher();

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows
    ui->tableWidget->setColumnCount(7); // Match the number of columns (ID, Name, Type, Brand, Quantity, Purchase Date, Image)
    QStringList headers;
    headers << "ID" << "Name" << "Type" << "Brand" << "Quantity" << "Purchase Date" << "Image";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Copy data from model to tableWidget
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem();
            if (col == 6) { // Image column (index 6)
                QByteArray imageData = model->data(model->index(row, col)).toByteArray();
                if (!imageData.isEmpty()) {
                    QPixmap pixmap;
                    pixmap.loadFromData(imageData);
                    if (!pixmap.isNull()) {
                        item->setData(Qt::DecorationRole, pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Display as icon
                    } else {
                        item->setText("Image Load Failed");
                    }
                } else {
                    item->setText("No Image");
                }
            } else {
                item->setText(model->data(model->index(row, col)).toString());
            }
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->setColumnWidth(120, 120); // Adjust width for the image column
    delete model;

    ui->tableWidget->setFocus();
}*/

void MainWindow::refreshTableWidget() {
    Resource R;
    QSqlQueryModel* model = R.afficher();

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows
    ui->tableWidget->setColumnCount(7); // Match the number of columns (ID, Name, Type, Brand, Quantity, Purchase Date, Image)
    QStringList headers;
    headers << "ID" << "Name" << "Type" << "Brand" << "Quantity" << "Purchase Date" << "Image";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Copy data from model to tableWidget
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem();
            if (col == 5) { // Purchase Date column (index 5)
                // Get the date from the model and format it
                QDate date = model->data(model->index(row, col)).toDate();
                if (date.isValid()) {
                    item->setText(date.toString("yyyy-MM-dd")); // Format as "yyyy-MM-dd"
                } else {
                    item->setText("Invalid Date");
                }
            } else if (col == 6) { // Image column (index 6)
                QByteArray imageData = model->data(model->index(row, col)).toByteArray();
                if (!imageData.isEmpty()) {
                    QPixmap pixmap;
                    pixmap.loadFromData(imageData);
                    if (!pixmap.isNull()) {
                        item->setData(Qt::DecorationRole, pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Display as icon
                    } else {
                        item->setText("Image Load Failed");
                    }
                } else {
                    item->setText("No Image");
                }
            } else {
                item->setText(model->data(model->index(row, col)).toString());
            }
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->setColumnWidth(6, 120); // Adjust width for the image column (fixed typo: was 120 instead of column index 6)
    delete model;

    ui->tableWidget->setFocus();
}

void MainWindow::handleDeleteButtonClick() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to delete.");
        return;
    }

    // Get the ID of the selected meeting
    int resource_id = ui->tableWidget->item(row, 0)->text().toInt();

    // Delete the meeting from the database
    Resource R;
    if (R.delet(resource_id)) {
        QMessageBox::information(this, "Success", "Resource deleted successfully.");

        // Refresh the table to remove the deleted meeting
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete Resource. Check the database connection.");
    }
}





//EL S7I7A
/*void MainWindow::on_updateButton_clicked()
{
    qDebug() << "on_updateButton_clicked called. Current row:" << ui->tableWidget->currentRow();

    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a row to update.");
        qDebug() << "No row selected, exiting.";
        return;
    }

    qDebug() << "Selected row:" << row << "with ID:" << ui->tableWidget->item(row, 0)->text();

    // Disconnect the button to prevent multiple clicks during the process
    disconnect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);

    // Create a dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Update Resource");

    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Create form fields
    QLabel *nameLabel = new QLabel("Name:");
    QLineEdit *nameEdit = new QLineEdit();
    QLabel *typeLabel = new QLabel("Type:");
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems(QStringList() << "Electronics " << "Cleaning Supplies" << "Kitchen Supplies" << "Stationery");
    QLabel *brandLabel = new QLabel("Brand:");
    QLineEdit *brandEdit = new QLineEdit();
    QLabel *quantityLabel = new QLabel("Quantity:");
    QLineEdit *quantityEdit = new QLineEdit();
    QLabel *purchaseDateLabel = new QLabel("Purchase Date:");
    QDateEdit *purchaseDateEdit = new QDateEdit();
    purchaseDateEdit->setDisplayFormat("yyyy-MM-dd");
    purchaseDateEdit->setCalendarPopup(true);

    // Add image selection fields
    QLabel *imageLabel = new QLabel("Image:");
    QPushButton *selectImageButton = new QPushButton("Select Image");
    QLabel *imagePreview = new QLabel();
    imagePreview->setFixedSize(100, 100); // Adjust size as needed
    imagePreview->setScaledContents(true);

    // Local variable to store the selected image data
    QByteArray updatedImageData;

    // Pre-fill with selected row data
    selectedResourceId = ui->tableWidget->item(row, 0)->text().toInt();
    nameEdit->setText(ui->tableWidget->item(row, 1)->text());
    typeCombo->setCurrentText(ui->tableWidget->item(row, 2)->text());
    brandEdit->setText(ui->tableWidget->item(row, 3)->text());
    quantityEdit->setText(ui->tableWidget->item(row, 4)->text());
    purchaseDateEdit->setDate(QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd"));

    // Connect the image selection button
    connect(selectImageButton, &QPushButton::clicked, &dialog, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&dialog, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!filePath.isEmpty()) {
            QPixmap image;
            if (!image.load(filePath)) {
                QMessageBox::warning(&dialog, "Image Load Error", "Failed to load the image. Please select a valid image file.");
                qDebug() << "Failed to load image into QPixmap:" << filePath;
                return;
            }

            imagePreview->setPixmap(image.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                updatedImageData = file.readAll();
                file.close();

                if (updatedImageData.isEmpty()) {
                    QMessageBox::warning(&dialog, "Image Error", "The selected image file is empty or could not be read.");
                    qDebug() << "Image data is empty after reading:" << filePath;
                    imagePreview->clear();
                    updatedImageData.clear();
                    return;
                }

                qDebug() << "Updated image loaded successfully, size:" << updatedImageData.size() << "bytes";
            } else {
                QMessageBox::warning(&dialog, "File Error", "Failed to open the image file: " + file.errorString());
                qDebug() << "Failed to open image file:" << file.errorString();
                imagePreview->clear();
                return;
            }
        } else {
            qDebug() << "No image selected for update.";
        }
    });

    // Add fields to layout
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(typeLabel);
    layout->addWidget(typeCombo);
    layout->addWidget(brandLabel);
    layout->addWidget(brandEdit);
    layout->addWidget(quantityLabel);
    layout->addWidget(quantityEdit);
    layout->addWidget(purchaseDateLabel);
    layout->addWidget(purchaseDateEdit);
    layout->addWidget(imageLabel);
    layout->addWidget(selectImageButton);
    layout->addWidget(imagePreview);

    // Add OK and Cancel buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    // Connect buttons
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, [&]() {
        qDebug() << "Dialog accepted. Selected Resource ID:" << selectedResourceId;
        Resource resource;
        resource.setResourceId(selectedResourceId);
        resource.setName(nameEdit->text());
        resource.setType(typeCombo->currentText());
        resource.setBrand(brandEdit->text());
        resource.setQuantity(quantityEdit->text().toInt());
        resource.setPurchaseDate(purchaseDateEdit->date());
        resource.setImage(updatedImageData); // Set the updated image data

        if (resource.updateResource()) {
            qDebug() << "Resource updated successfully!";
            refreshTableWidget();
            QMessageBox::information(this, "Success", "Resource updated successfully.");
            dialog.accept();
        } else {
            qDebug() << "Failed to update resource:" << QSqlQuery().lastError().text();
            QMessageBox::warning(this, "Update Error", "Failed to update resource: " + QSqlQuery().lastError().text());
        }
    });
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    qDebug() << "Showing dialog...";
    dialog.exec();
    qDebug() << "Dialog closed. Current row after exec:" << ui->tableWidget->currentRow();
    qDebug() << "Selected Resource ID after exec:" << selectedResourceId;

    // Reconnect the button after the dialog closes
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    selectedResourceId = -1;
    qDebug() << "Reset selectedResourceId to -1. Current row after reset:" << ui->tableWidget->currentRow();
}
*/



void MainWindow::on_updateButton_clicked()
{
    qDebug() << "on_updateButton_clicked called. Current row:" << ui->tableWidget->currentRow();

    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a row to update.");
        qDebug() << "No row selected, exiting.";
        return;
    }

    qDebug() << "Selected row:" << row << "with ID:" << ui->tableWidget->item(row, 0)->text();

    // Disconnect the button to prevent multiple clicks during the process
    disconnect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);

    // Create a dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Update Resource");

    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Create form fields
    QLabel *nameLabel = new QLabel("Name:");
    QLineEdit *nameEdit = new QLineEdit();
    QLabel *typeLabel = new QLabel("Type:");
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems(QStringList() << "Electronics" << "Cleaning Supplies" << "Kitchen Supplies" << "Stationery");
    QLabel *brandLabel = new QLabel("Brand:");
    QLineEdit *brandEdit = new QLineEdit();
    QLabel *quantityLabel = new QLabel("Quantity:");
    QLineEdit *quantityEdit = new QLineEdit();
    QLabel *purchaseDateLabel = new QLabel("Purchase Date:");
    QDateEdit *purchaseDateEdit = new QDateEdit();
    purchaseDateEdit->setDisplayFormat("yyyy-MM-dd");
    purchaseDateEdit->setCalendarPopup(true);

    // Add image selection fields
    QLabel *imageLabel = new QLabel("Image:");
    QPushButton *selectImageButton = new QPushButton("Select Image");
    QLabel *imagePreview = new QLabel();
    imagePreview->setFixedSize(100, 100); // Adjust size as needed
    imagePreview->setScaledContents(true);

    // Local variable to store the selected image data
    QByteArray updatedImageData;

    // Pre-fill with selected row data
    selectedResourceId = ui->tableWidget->item(row, 0)->text().toInt();
    nameEdit->setText(ui->tableWidget->item(row, 1)->text());
    typeCombo->setCurrentText(ui->tableWidget->item(row, 2)->text());
    brandEdit->setText(ui->tableWidget->item(row, 3)->text());
    quantityEdit->setText(ui->tableWidget->item(row, 4)->text());
    purchaseDateEdit->setDate(QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd"));

    // Connect the image selection button
    connect(selectImageButton, &QPushButton::clicked, &dialog, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&dialog, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!filePath.isEmpty()) {
            QPixmap image;
            if (!image.load(filePath)) {
                QMessageBox::warning(&dialog, "Image Load Error", "Failed to load the image. Please select a valid image file.");
                qDebug() << "Failed to load image into QPixmap:" << filePath;
                return;
            }

            imagePreview->setPixmap(image.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                updatedImageData = file.readAll();
                file.close();

                if (updatedImageData.isEmpty()) {
                    QMessageBox::warning(&dialog, "Image Error", "The selected image file is empty or could not be read.");
                    qDebug() << "Image data is empty after reading:" << filePath;
                    imagePreview->clear();
                    updatedImageData.clear();
                    return;
                }

                qDebug() << "Updated image loaded successfully, size:" << updatedImageData.size() << "bytes";
            } else {
                QMessageBox::warning(&dialog, "File Error", "Failed to open the image file: " + file.errorString());
                qDebug() << "Failed to open image file:" << file.errorString();
                imagePreview->clear();
                return;
            }
        } else {
            qDebug() << "No image selected for update.";
        }
    });

    // Add fields to layout
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(typeLabel);
    layout->addWidget(typeCombo);
    layout->addWidget(brandLabel);
    layout->addWidget(brandEdit);
    layout->addWidget(quantityLabel);
    layout->addWidget(quantityEdit);
    layout->addWidget(purchaseDateLabel);
    layout->addWidget(purchaseDateEdit);
    layout->addWidget(imageLabel);
    layout->addWidget(selectImageButton);
    layout->addWidget(imagePreview);

    // Add OK and Cancel buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    // Connect buttons with validation
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, [&]() {
        // Retrieve values from the form
        QString name = nameEdit->text().trimmed();
        QString type = typeCombo->currentText().trimmed();
        QString brand = brandEdit->text().trimmed();
        bool ok;
        int quantity = quantityEdit->text().toInt(&ok);
        QDate purchase_date = purchaseDateEdit->date();
        QDate currentDate = QDate::currentDate(); // Current date: March 09, 2025

        // Validation checks
        // 1. Check if any field is empty
        if (name.isEmpty() || type.isEmpty() || brand.isEmpty() || !ok) {
            QMessageBox::warning(&dialog, "Input Error", "All fields (Name, Type, Brand, Quantity) must be filled correctly.");
            return;
        }

        // 2. Check if fields start with uppercase
        auto startsWithUpper = [](const QString& str) {
            return !str.isEmpty() && str[0].isUpper();
        };
        if (!startsWithUpper(name) || !startsWithUpper(type) || !startsWithUpper(brand)) {
            QMessageBox::warning(&dialog, "Input Error", "Name, Type, and Brand must start with an uppercase letter.");
            return;
        }

        // 3. Check quantity: must be > 0
        if (!ok || quantity <= 0) {
            QMessageBox::warning(&dialog, "Input Error", "Quantity must be a positive number greater than zero.");
            return;
        }

        // 4. Check purchase date: must not be in the future
        if (purchase_date > currentDate) {
            QMessageBox::warning(&dialog, "Input Error", "Purchase date cannot be in the future.");
            return;
        }

        // 5. Check if an image is selected
        if (updatedImageData.isEmpty()) {
            QMessageBox::warning(&dialog, "Input Error", "Please select an image.");
            return;
        }

        // If all validations pass, update the resource
        Resource resource;
        resource.setResourceId(selectedResourceId);
        resource.setName(name);
        resource.setType(type);
        resource.setBrand(brand);
        resource.setQuantity(quantity);
        resource.setPurchaseDate(purchase_date);
        resource.setImage(updatedImageData); // Set the updated image data

        if (resource.updateResource()) {
            qDebug() << "Resource updated successfully!";
            refreshTableWidget();
            QMessageBox::information(this, "Success", "Resource updated successfully.");
            dialog.accept();
        } else {
            qDebug() << "Failed to update resource:" << QSqlQuery().lastError().text();
            QMessageBox::warning(this, "Update Error", "Failed to update resource: " + QSqlQuery().lastError().text());
        }
    });
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    qDebug() << "Showing dialog...";
    dialog.exec();
    qDebug() << "Dialog closed. Current row after exec:" << ui->tableWidget->currentRow();
    qDebug() << "Selected Resource ID after exec:" << selectedResourceId;

    // Reconnect the button after the dialog closes
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    selectedResourceId = -1;
    qDebug() << "Reset selectedResourceId to -1. Current row after reset:" << ui->tableWidget->currentRow();
}

void MainWindow::toggleTheme() {
    isDarkTheme = !isDarkTheme;
    if (isDarkTheme) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}


void MainWindow::applyLightTheme() {
    // Blueish white gradient (unchanged)
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #FFFFFF, stop:1 #A1B8E6);
            color: #333333;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        /* Buttons with rounded corners and subtle shadow (remove box-shadow if not supported) */
        QPushButton {
            background-color: #3A5DAE;
            color: white;
            border: 1px solid #2A4682;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
            /* box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1); */
        }
        QPushButton:hover {
            background-color: #4A70C2;
            /* box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.2); */
        }
        QPushButton:pressed {
            background-color: #2A4682;
        }
        /* Input fields with smooth borders */
        QLineEdit, QComboBox, QDateTimeEdit {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
            padding: 4px;
            color: #333333;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus {
            border: 2px solid #3A5DAE;
        }
        /* Table view styling */
        QTableView {
            background-color: #FFFFFF;
            border: 1px solid #D3DCE6;
            border-radius: 4px;
            selection-background-color: #A1B8E6;
            selection-color: #333333;
        }
        QHeaderView::section {
            background-color: #3A5DAE;
            color: white;
            padding: 5px;
            border: none;
            border-radius: 2px;
        }
        /* Calendar styling */
        QCalendarWidget {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
        }
        QCalendarWidget QToolButton {
            background-color: #3A5DAE;
            color: white;
            border-radius: 3px;
        }
        /* Tooltip */
        QToolTip {
            color: #333333;
            background-color: #E6ECF5;
            border: 1px solid #3A5DAE;
            border-radius: 3px;
        }
        /* Tab widget */
        QTabWidget::pane {
            border: 1px solid #3A5DAE;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #D3DCE6;
            color: #333333;
            padding: 6px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #3A5DAE;
            color: white;
        }
        /* General Label Styling (all labels) */
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        /* Form Labels (e.g., Name:, Sector:) - Using custom property or specific names */
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #3A5DAE;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        /* Main Title ("Client Management System") */
        #label { /* Specific to ui->label in frame_5 */
            font-size: 18pt;
            font-weight: bold;
            color: #3A5DAE;
            /* text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2); */
            qproperty-alignment: AlignCenter;
        }
        /* Frames for distinguishing areas */
        QFrame#header {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            /* box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.1); */
        }
        QFrame#sideMenu {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            /* box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.1); */
            background-color: #E6ECF5; /* Slightly darker blueish background for contrast */
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #D3DCE6;
            border-radius: 5px;
            /* box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.1); */
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::applyDarkTheme() {
    // Lighter orange to soft dark gray gradient
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #F28C6F, stop:1 #5C5C5C);
            color: #F0F0F0;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        /* Buttons with rounded corners and subtle shadow */
        QPushButton {
            background-color: #F28C6F;
            color: white;
            border: 1px solid #D96C53;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
            box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
        }
        QPushButton:hover {
            background-color: #F5A38A;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.4);
        }
        QPushButton:pressed {
            background-color: #D96C53;
        }
        /* Input fields with smooth borders */
        QLineEdit, QComboBox, QDateTimeEdit {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
            padding: 4px;
            color: #F0F0F0;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus {
            border: 2px solid #F28C6F;
        }
        /* Table view styling */
        QTableView {
            background-color: #6A6A6A;
            border: 1px solid #4A4A4A;
            border-radius: 4px;
            selection-background-color: #F28C6F;
            selection-color: #F0F0F0;
        }
        QHeaderView::section {
            background-color: #F28C6F;
            color: white;
            padding: 5px;
            border: none;
            border-radius: 2px;
        }
        /* Calendar styling */
        QCalendarWidget {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
        }
        QCalendarWidget QToolButton {
            background-color: #F28C6F;
            color: white;
            border-radius: 3px;
        }
        /* Tooltip */
        QToolTip {
            color: #F0F0F0;
            background-color: #F28C6F;
            border: 1px solid #D96C53;
            border-radius: 3px;
        }
        /* Tab widget */
        QTabWidget::pane {
            border: 1px solid #F28C6F;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #7A7A7A;
            color: #F0F0F0;
            padding: 6px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #F28C6F;
            color: white;
        }
        /* General Label Styling (all labels) */
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        /* Form Labels (e.g., Name:, Sector:) - Using custom property or specific names */
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #F28C6F;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        /* Main Title ("Client Management System") */
        #label { /* Specific to ui->label in frame_5 */
            font-size: 18pt;
            font-weight: bold;
            color: #F28C6F;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);
            qproperty-alignment: AlignCenter;
        }
        /* Frames for distinguishing areas */
        QFrame#header {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
        }
        QFrame#sideMenu {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
            background-color: #7A7A7A; /* Slightly lighter gray for contrast */
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #4A4A4A;
            border-radius: 5px;
            box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.3);
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::toggleSidebar() {
    // Check if the sidebar is currently visible
    bool isVisible = ui->sideMenu->isVisible();

    // Toggle visibility
    ui->sideMenu->setVisible(!isVisible);
}
