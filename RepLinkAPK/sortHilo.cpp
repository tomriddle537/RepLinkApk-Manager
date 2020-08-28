#include "sortHilo.h"

SortHilo::SortHilo(QObject *parent): QThread(parent){
    Q_UNUSED(parent);

}


void SortHilo::run()
{
    int cant=appsView->count();
    AppInfo aux1;
    AppInfo aux2;
    QListWidgetItem *temp1;
    QListWidgetItem *temp2;

    if(asc){
        for (int i = 0; i < cant-1 ; ++i) {

            emit Porciento(int(i*100/cant));
            this->msleep(5+ (i==0? 150:0));

            for (int j = i+1; j < cant ; ++j) {
                aux1=qvariant_cast<AppInfo>(appsView->item(i)->data(6));
                aux2=qvariant_cast<AppInfo>(appsView->item(j)->data(6));

                if(i!=j && aux1.ObtenerNoPackages().toInt() < aux2.ObtenerNoPackages().toInt()){
                    temp1=appsView->item(i)->clone();
                    temp2=appsView->item(j)->clone();

                    //std::cout<< "  "+ aux1.ObtenerName().toStdString() <<std::endl;
                    //std::cout<< "  "+ aux2.ObtenerName().toStdString() <<std::endl;



                    appsView->takeItem(i);
                    appsView->insertItem(i,temp2);

                    appsView->takeItem(j);
                    appsView->insertItem(j, temp1);

                }
            }
        }
    }else{
        for (int i = 0; i < cant-1 ; ++i) {
            emit Porciento(int(i*100/cant));
            this->msleep(5+ (i==0? 150:0));

            for (int j = i+1; j < cant ; ++j) {
                aux1=qvariant_cast<AppInfo>(appsView->item(i)->data(6));
                aux2=qvariant_cast<AppInfo>(appsView->item(j)->data(6));

                if(i!=j && aux1.ObtenerNoPackages().toInt() > aux2.ObtenerNoPackages().toInt()){
                    temp1=appsView->item(i)->clone();
                    temp2=appsView->item(j)->clone();

                    //std::cout<< "  "+ aux1.ObtenerName().toStdString() <<std::endl;
                    //std::cout<< "  "+ aux2.ObtenerName().toStdString() <<std::endl;

                    appsView->takeItem(i);
                    appsView->insertItem(i,temp2);

                    appsView->takeItem(j);
                    appsView->insertItem(j, temp1);
                 }
            }
        }
    }
    emit Porciento(100);

}

void SortHilo::Cerrar()
{
    this->terminate();
}

void SortHilo::SetParametros(QListWidget *&appsView, bool asc)
{
    this->appsView=appsView;
    this->asc=asc;
}

