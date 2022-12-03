
#include "tilitapahtumat.h"
#include "myurl.h"
#include <QStringList>

Tilitapahtumat::Tilitapahtumat(QString id_kortti,QObject *parent)
    : QObject{parent}
{
    kortti=id_kortti; //alustetaan käytettävä kortti heti samalla kun luodaan koosteyhteys
}

void Tilitapahtumat::tilitapahtumatSlot(QNetworkReply *reply)
{
    //response_data=reply->readAll();
    //qDebug()<<response_data;

     QByteArray response_data=reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray json_array = json_doc.array();

         QStringList tapahtumat = {""};
         tapahtumat.clear();

    //siirretään haetut tiedot muuttujiin
        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();
            tilinOmistaja=json_obj["tilin omistaja"].toString();
            saldo=QString::number(json_obj["saldo"].toInt());
            tapahtumat+="Tapahtuma: "+json_obj["tapahtuma"].toString()+" "+"Paiva: "+json_obj["paiva"].toString()+" "+"Aika: "+json_obj["aika"].toString()+" "+"Summa: "+QString::number(json_obj["summa"].toInt())+"\r";
        }

        qDebug()<<"\n"<<tapahtumat;
        qDebug()<<"lahetan nayta signal";
        qDebug()<<"TAPAHTUMAT TULOSTEETTUUUUUUU  \n";
        emit tilitapahtumat_nayta(tapahtumat,tilinOmistaja,saldo,tilinumero); //lähetetään haetut tiedot tilitapahtumien tulostus slottiin korttiwindowille.

        reply->deleteLater();
        tilitapahtumaManager->deleteLater();
}

void Tilitapahtumat::tilitapahtumat_clicked(QByteArray webToken, QString tili)
{
   qDebug()<<"vastaanotettiin tili signaali";
   wb=webToken; //alustetaan webtoken
   qDebug()<<wb;

   tilinumero=tili;
   QString site_url=MyUrl::getBaseUrl()+"/selaa_tilitapahtumia/"+tilinumero;
   QNetworkRequest request((site_url));
   //WEBTOKEN ALKU
   QByteArray myToken=wb;
   request.setRawHeader(QByteArray("Authorization"),(myToken));
   //WEBTOKEN LOPPU
   tilitapahtumaManager = new QNetworkAccessManager(this);

   connect(tilitapahtumaManager, SIGNAL(finished (QNetworkReply*)), this, SLOT(tilitapahtumatSlot(QNetworkReply*)));

   reply = tilitapahtumaManager->get(request);
}
