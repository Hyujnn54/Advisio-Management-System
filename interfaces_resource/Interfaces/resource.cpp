#include "resource.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>
//#include <QDate>
//#include <QByteArray>


Resource::Resource() :  name(""), type(""), brand(""), quantity(0), purchase_date(QDate::currentDate()), image(QByteArray()) {}

Resource::Resource( QString& name, QString& type, QString& brand, int& quantity, QDate& purchase_date, QByteArray& image)
    :  name(name), type(type), brand(brand), quantity(quantity), purchase_date(purchase_date), image(image) {}


int Resource::getResourceId() const { return resource_id; }
QString Resource::getName() const { return name; }
QString Resource::getType() const { return type; }
QString Resource::getBrand() const { return brand; }
int Resource::getQuantity() const { return quantity; }
QDate Resource::getPurchaseDate() const { return purchase_date; }
QByteArray Resource::getImage() const { return image; }


//void Resource::setResourceId(int id) { resource_id = id; }
void Resource::setName(const QString& name) { this->name = name; }
void Resource::setType(const QString& type) { this->type = type; }
void Resource::setBrand(const QString& brand) { this->brand = brand; }
void Resource::setQuantity(const int& quantity) { this->quantity = quantity; }
void Resource::setPurchaseDate(const QDate& purchaseDate) { this->purchase_date = purchaseDate; }
void Resource::setImage(const QByteArray& image) { this->image = image; }


bool Resource::addResource() {
    QSqlQuery query;

    qDebug() << "Adding resource with values: ";
    qDebug() << "Name: " << name;
    qDebug() << "Type: " << type;
    qDebug() << "Brand: " << brand;
    qDebug() << "Quantity: " << quantity;
    qDebug() << "Purchase Date: " << purchase_date.toString("yyyy-MM-dd");
    qDebug() << "Image Size: " << image.size();

    query.prepare("INSERT INTO RESOURCES (NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE) "
                  "VALUES (:name, :type, :brand, :quantity, :purchase_date, :image)");

    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":brand", brand);
    query.bindValue(":quantity", quantity);
    query.bindValue(":purchase_date", purchase_date.toString("yyyy-MM-dd"));
    query.bindValue(":image", image);

    return query.exec();

    if (query.exec()) {
        qDebug() << "Resource added successfully!";
        return true;
    } else {
        qDebug() << "Error adding resource: " << query.lastError().text();
        return false;
    }

}

/*QSqlQueryModel *Resource::loadResource(){
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT RESOURCE_ID, NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE FROM RESOURCES");

    if (!query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("RESOURCE_ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("NAME"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("TYPE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("BRAND"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("QUANTITY"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("PURCHASE_DATE"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("IMAGE"));
    } else {
        qDebug() << "Error loading resource list:" << query.lastError().text();
    }

    return model;

}*/


QSqlQueryModel* Resource::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESOURCES");

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error when displaying a resource:" << model->lastError().text();
    }

    return model;
}
